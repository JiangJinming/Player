#include "myplayer.h"
#include "ui_myplayer.h"

#include <QDebug>
#include <QMediaContent>
#include <QUrl>
#include <QString>
#include <QObject>
#include <QTime>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <QmessageBox>
#include <QPixmap>
#include <QFileDialog>
#include <QStandardPaths>
#include <QIcon>

MyPlayer::MyPlayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyPlayer)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);
    playList = new QMediaPlaylist(this);

    /************just test*************
    playList->addMedia(QUrl::fromLocalFile("E:/Users/jiang/Desktop/testmusic/All Along the Watchtower.mp3"));
    playList->addMedia(QUrl::fromLocalFile("E:/Users/jiang/Desktop/testmusic/Crazy Train.mp3"));
    ***********************************/
    loadMedia();
    player->setPlaylist(playList);
    playList->setCurrentIndex(0);
    playList->setPlaybackMode(QMediaPlaylist::Loop);

    //basic data
    QObject::connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(getDuration(qint64)));
    QObject::connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(getState(QMediaPlayer::State)));

    //control volume
    QObject::connect(ui->volumeHorizontalSlider, SIGNAL(valueChanged(int)), player, SLOT(setVolume(int)));
    QObject::connect(player, SIGNAL(volumeChanged(int)), ui->volumeHorizontalSlider, SLOT(setValue(int)));
    QObject::connect(player, SIGNAL(volumeChanged(int)), this, SLOT(setVolumeLabelValue(int)));

    //control position
    QObject::connect(ui->positionHorizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(setPlayerPositionValue(int)));
    QObject::connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(setSliderPostionValue(qint64)));
    QObject::connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(setPositionLabelValue(qint64)));
    
    //control state
    QObject::connect(ui->stopButton, SIGNAL(clicked(bool)), this, SLOT(setPlayerState()));

    //next and previous
    QObject::connect(ui->previousButton, SIGNAL(clicked(bool)), playList, SLOT(previous()));
    QObject::connect(ui->nextButton, SIGNAL(clicked(bool)), playList, SLOT(next()));

    //select music from fileList
    QObject::connect(ui->filesListWidget, SIGNAL(currentRowChanged(int)), playList, SLOT(setCurrentIndex(int)));

    //add directory
    QObject::connect(ui->addDirButton, SIGNAL(clicked(bool)), this, SLOT(addDir()));
    QObject::connect(ui->refreshButton, SIGNAL(clicked(bool)), this, SLOT(searchLocalFiles()));

    //set playbackMode
    QObject::connect(playList, SIGNAL(playbackModeChanged(QMediaPlaylist::PlaybackMode)), this, SLOT(getPlaybackMode(QMediaPlaylist::PlaybackMode)));
    QObject::connect(ui->playbackModeButton, SIGNAL(clicked(bool)), this, SLOT(setPlaybackMode()));

    //************just test**************
    player->setVolume(50);
    player->play();
}

MyPlayer::~MyPlayer()
{
    delete ui;
}

void MyPlayer::getDuration(qint64 dur)
{
    duration = dur;

    qDebug() << "duration:" << duration;

    //set range of silder
    ui->positionHorizontalSlider->setMinimum(0);
    ui->positionHorizontalSlider->setMaximum(duration);

    //show duration on label
    ui->durationLabel->setText(QTime::fromMSecsSinceStartOfDay(static_cast<int>(duration)).toString("hh:mm:ss"));
}

void MyPlayer::getState(QMediaPlayer::State sta)
{
    state = sta;

    //debug information
    switch (state) {
    case QMediaPlayer::StoppedState:
        qDebug() << "stopped state";
        break;
    case QMediaPlayer::PlayingState:
        qDebug() << "playing state";
        break;
    case QMediaPlayer::PausedState:
        qDebug() << "pause state";
        break;
    default:
        qDebug() << "error state";
        break;
    }
}

void MyPlayer::setVolumeLabelValue(int vol)
{
    ui->volumeLabel->setText(QString::number(vol));
}

void MyPlayer::setPlayerPositionValue(int pos)
{

    //qDebug() << "silder pos:" << pos;

    player->setPosition(static_cast<qint64>(pos));
}

void MyPlayer::setSliderPostionValue(qint64 pos)
{

    //qDebug() << "player pos:" << pos;

    ui->positionHorizontalSlider->setValue(static_cast<int>(pos));
}

void MyPlayer::setPositionLabelValue(qint64 pos)
{
    ui->positionLabel->setText(QTime::fromMSecsSinceStartOfDay(static_cast<int>(pos)).toString("hh:mm:ss"));
}

void MyPlayer::setPlayerState()
{
    if (state == QMediaPlayer::StoppedState || state == QMediaPlayer::PausedState) {
        player->play();
        
        //change icon and other
    }
    else if (state == QMediaPlayer::PlayingState) {
        player->pause();
    }
    else
        qDebug() << "set state error";
}

void MyPlayer::loadMedia()
{
    //creat a messagebox if on dir in dirList
    if (ui->dirListWidget->count() == 0) {
        QMessageBox msgBox;

        msgBox.setWindowIcon(QIcon(":/icon/dirMessageBoxIcon.png"));
        msgBox.setWindowTitle("Add Dir");
        msgBox.setIconPixmap(QPixmap(":/icon/addDir.png"));
        msgBox.setTextFormat(Qt::RichText);
        msgBox.setText(QObject::tr("<b>Add a music directory<b/>"));
        msgBox.setInformativeText(QObject::tr("Do you want add a directory?"));
        msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);

        int ret = msgBox.exec();
        if (ret == QMessageBox::Ok)
            addDir();
    }
}

void MyPlayer::addDir()
{
    //get a dir path by a QFileDialog and research all dir
    QString dir = QFileDialog::getExistingDirectory(this, QObject::tr("Select Directory"), "/home",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir != "") {
        //avoid add same dir
        for (int i = 0; i < ui->dirListWidget->count(); i++)
            if (dir == ui->dirListWidget->item(i)->text())
                return;

        //add new dir to dirList
        ui->dirListWidget->addItem(dir);

        //search files from dirList
        searchLocalFiles();
    }
}

void MyPlayer::searchLocalFiles()
{
    //clear all from playList and filesList
    playList->clear();
    ui->filesListWidget->clear();

    int i;
    int k;
    QStringList filesList;

    //ui->dirListWidget->addItem("E:/Users/jiang/Desktop/testmusic");

    for (i = 0; i < ui->dirListWidget->count(); i++) {
        QString dirName(ui->dirListWidget->item(i)->text());
        QFileInfoList filesListInfo;
        qDebug() << dirName;

        QDir dir(dirName);
        QStringList filters;
        filters << "*.mp3";
        dir.setNameFilters(filters);

        filesListInfo = dir.entryInfoList();

        for (int j = 0; j < filesListInfo.size(); j++)
            filesList << filesListInfo.at(j).filePath();
    }

    /************just test*************
    QStringList fileslist;
    filesList << "E:/Users/jiang/Desktop/testmusic/All Along the Watchtower.mp3"
         << "E:/Users/jiang/Desktop/testmusic/Crazy Train.mp3";
    ***********************************/

    for (k = 0; k < filesList.size(); k++) {
        playList->addMedia(QUrl::fromLocalFile(filesList.at(k)));
        QListWidgetItem *fileItem = new QListWidgetItem;
        fileItem->setText(QFileInfo(filesList.at(k)).baseName());
        ui->filesListWidget->addItem(fileItem);
    }
}

void MyPlayer::getPlaybackMode(QMediaPlaylist::PlaybackMode mode)
{
    playbackMode = mode;

    //debug information
    switch (mode) {
    case QMediaPlaylist::CurrentItemOnce:
        qDebug() << "current item once";
        break;
    case QMediaPlaylist::CurrentItemInLoop:
        qDebug() << "current item in loop";
        break;
    case QMediaPlaylist::Sequential:
        qDebug() << "sequential";
        break;
    case QMediaPlaylist::Loop:
        qDebug() << "loop";
        break;
    case QMediaPlaylist::Random:
        qDebug() << "random";
        break;
    default:
        qDebug() << "error playback mode";
        break;
    }
}

void MyPlayer::setPlaybackMode()
{
    switch (playbackMode) {
    case QMediaPlaylist::CurrentItemOnce:
        playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

        //change icon and other
        break;
    case QMediaPlaylist::CurrentItemInLoop:
        playList->setPlaybackMode(QMediaPlaylist::Sequential);

        break;
    case QMediaPlaylist::Sequential:
        playList->setPlaybackMode(QMediaPlaylist::Loop);

        break;
    case QMediaPlaylist::Loop:
        playList->setPlaybackMode(QMediaPlaylist::Random);

        break;

    case QMediaPlaylist::Random:
        playList->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);

        break;
    default:
        qDebug() << "set playback mode error";
        break;
    }
}
