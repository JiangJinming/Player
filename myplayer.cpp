#include "myplayer.h"
#include "ui_myplayer.h"

#include <QDebug>
#include <QMediaPlaylist>
#include <QMediaContent>
#include <QUrl>
#include <QString>
#include <QObject>
#include <QTime>
#include <QStringList>
#include <QFileInfo>
#include <QDir>

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
    QObject::connect(ui->stopButton, SIGNAL(clicked(bool)), this, SLOT(changePlayerState()));

    //next and previous
    QObject::connect(ui->previousButton, SIGNAL(clicked(bool)), playList, SLOT(previous()));
    QObject::connect(ui->nextButton, SIGNAL(clicked(bool)), playList, SLOT(next()));

    //select file
    QObject::connect(ui->filesListWidget, SIGNAL(currentRowChanged(int)), playList, SLOT(setCurrentIndex(int)));

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

    qDebug() << "silder pos:" << pos;

    player->setPosition(static_cast<qint64>(pos));
}

void MyPlayer::setSliderPostionValue(qint64 pos)
{

    qDebug() << "player pos:" << pos;

    ui->positionHorizontalSlider->setValue(static_cast<int>(pos));
}

void MyPlayer::setPositionLabelValue(qint64 pos)
{
    ui->positionLabel->setText(QTime::fromMSecsSinceStartOfDay(static_cast<int>(pos)).toString("hh:mm:ss"));
}

void MyPlayer::changePlayerState()
{
    if (state == QMediaPlayer::StoppedState || state == QMediaPlayer::PausedState) {
        player->play();
        
        //change icon and other
    }
    else if (state == QMediaPlayer::PlayingState) {
        player->pause();
    }
    else
        qDebug() << "error state";
}

void MyPlayer::loadMedia()
{
    int i;
    int k;
    QStringList filesList;

    ui->dirListWidget->addItem("E:/Users/jiang/Desktop/testmusic");

    for (i = 0; i < ui->dirListWidget->count(); i++) {
        QString dirName(ui->dirListWidget->item(i)->text());
        QFileInfoList filesListInfo;
        qDebug() << dirName;

        QDir dir(dirName);
        QStringList filters;
        filters << "*.mp3";
        dir.setNameFilters(filters);

        filesListInfo = dir.entryInfoList();

        for (int j = 0; j < filesListInfo.size(); j++) {
            filesList << filesListInfo.at(j).filePath();
        }
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
