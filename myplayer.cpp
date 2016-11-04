#include "myplayer.h"
#include "ui_myplayer.h"

#include <QDebug>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMediaContent>
#include <QUrl>

MyPlayer::MyPlayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyPlayer)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);
    playList = new QMediaPlaylist(this);

    /**********just test init***********
    playList->addMedia(QUrl::fromLocalFile("E:/Users/jiang/Desktop/testmusic/The Call Of Ktulu.mp3"));
    playList->addMedia(QUrl::fromLocalFile("E:/Users/jiang/Desktop/testmusic/Crazy Train.mp3"));
    player->setPlaylist(playList);
    playList->setCurrentIndex(0);
    ***********************************/

    player->play();
}

MyPlayer::~MyPlayer()
{
    delete ui;
}
