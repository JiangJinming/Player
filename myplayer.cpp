#include "myplayer.h"
#include "ui_myplayer.h"
#include "mymediaplayer.h"
#include "mymetadata.h"

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
#include <QCloseEvent>
#include <QIODevice>
#include <QDataStream>
#include <QSettings>
#include <QPoint>

MyPlayer::MyPlayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyPlayer)
{
    ui->setupUi(this);

    player = new MyMediaPlayer(this);
    playList = new QMediaPlaylist(this);

    player->setPlaylist(playList);

    /************just test*************
    playList->addMedia(QUrl::fromLocalFile("E:/Users/jiang/Desktop/testmusic/All Along the Watchtower.mp3"));
    playList->addMedia(QUrl::fromLocalFile("E:/Users/jiang/Desktop/testmusic/Crazy Train.mp3"));
    ***********************************/

    //basic data
    QObject::connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(getDuration(qint64)));

    //control volume
    QObject::connect(ui->volumeHorizontalSlider, SIGNAL(valueChanged(int)), player, SLOT(setVolume(int)));
    QObject::connect(player, SIGNAL(volumeChanged(int)), ui->volumeHorizontalSlider, SLOT(setValue(int)));
    QObject::connect(player, SIGNAL(volumeChanged(int)), this, SLOT(setVolumeLabelValue(int)));

    //control position
    QObject::connect(ui->positionHorizontalSlider, SIGNAL(sliderMoved(int)), player, SLOT(setPlayerPositionValue(int)));
    QObject::connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(setSliderPostionValue(qint64)));
    QObject::connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(setPositionLabelValue(qint64)));
    
    //control state
    QObject::connect(ui->stopButton, SIGNAL(clicked(bool)), player, SLOT(setPlayerState()));

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

    //control mainWindow by titleBar
    QObject::connect(ui->titleBar, SIGNAL(dragWindow(QPoint)), this, SLOT(moveWindow(QPoint)));
    QObject::connect(ui->titleBar, SIGNAL(windowStateChanged(MyTitleBar::WindowState)), this, SLOT(setMainWindowState(MyTitleBar::WindowState)));

    //flash info widget
    QObject::connect(player, SIGNAL(flashMetaData(MyMetaData)), ui->infoWidget, SLOT(flashWidgetInfo(MyMetaData)));

    //flash lrc widget
    QObject::connect(player, SIGNAL(expectLyricsPath(QString)), ui->LRCWidget, SLOT(tryToLoadLRCFile(QString)));
    QObject::connect(player, SIGNAL(positionChanged(qint64)), ui->LRCWidget, SLOT(flashCurrentPosition(qint64)));

    /************just test**************
    player->setVolume(50);
    player->play();
    ***********************************/

    //init
    init();
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

void MyPlayer::setVolumeLabelValue(int vol)
{
    ui->volumeLabel->setText(QString::number(vol));
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
        qDebug() << "current item sequential";
        break;
    case QMediaPlaylist::Loop:
        qDebug() << "current item loop";
        break;
    case QMediaPlaylist::Random:
        qDebug() << "current item random";
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

void MyPlayer::closeEvent(QCloseEvent *event)
{
    qDebug() << "close";
    saveCurrentInfo();
    event->accept();
}

void MyPlayer::saveCurrentInfo()
{
    qDebug() << "save";

    //save dirList
    int dirListCount = ui->dirListWidget->count();

    QFile DirListData("DirList.dat");
    DirListData.open(QIODevice::WriteOnly);

    QDataStream out(&DirListData);
    out << dirListCount;
    for (int i = 0; i < dirListCount; i++) {
        out << ui->dirListWidget->item(i)->text();
    }
    DirListData.close();

    //save setting
    QSettings settings("sets.ini", QSettings::IniFormat);
    settings.setValue("duration", duration);
    //settings.setValue("playerstate", state);
    settings.setValue("position", position);
    settings.setValue("volume", volume);
    settings.setValue("playbackmode", playbackMode);
}

void MyPlayer::init()
{
    //set title bar size
    ui->titleBar->setGeometry(0, 0, this->width(), 75);


    int dirListCount;

    QFile DirListData("DirList.dat");
    DirListData.open(QIODevice::ReadOnly);

    QDataStream in(&DirListData);
    in >> dirListCount;
    qDebug() << "init list count:" << dirListCount;
    for(int i = 0; i < dirListCount; i++) {
        QString text;
        in >> text;
        qDebug() << text;

        ui->dirListWidget->addItem(text);
    }

    QSettings settings("sets.ini", "Player");
    //playList->setCurrentIndex(static_cast<qint64>(settings.value("duration").toInt()));

    //state = static_cast<MyMediaPlayer::State>(settings.value("playerstate").toInt());
    //this->setPlayerState();

    player->setPlayerPositionValue(settings.value("position").toInt());
    player->setVolume(settings.value("volume").toInt());

    playbackMode = static_cast<QMediaPlaylist::PlaybackMode>(settings.value("playbackmode").toInt());
    this->setPlaybackMode();

    loadMedia();
}

void MyPlayer::moveWindow(const QPoint &point)
{
    this->move(this->pos() + point);
}

void MyPlayer::setMainWindowState(MyTitleBar::WindowState state)
{
    switch (state) {
    case MyTitleBar::Minimize:
        this->setWindowState(Qt::WindowMinimized);
        break;

    case MyTitleBar::Maximize:
        this->setWindowState(Qt::WindowMaximized);
        break;

    case MyTitleBar::Close:
        this->closeWindow();
        break;

    default:
        qDebug() << "set 'Main Window State' error";
    }
}

void MyPlayer::closeWindow()
{
    this->close();
}
