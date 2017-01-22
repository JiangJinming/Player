#include "mymediaplayer.h"

#include <QVariant>
#include <QDebug>
#include <QImage>

MyMediaPlayer::MyMediaPlayer(QObject *parent) :
    QMediaPlayer(parent)
{
    state = QMediaPlayer::StoppedState;

    QObject::connect(this, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(getState(QMediaPlayer::State)));
    //send meta data
    QObject::connect(this, SIGNAL(durationChanged(qint64)), this, SLOT(sendMetaData()));
}

void MyMediaPlayer::setPlayerPositionValue(int pos)
{
    this->setPosition(static_cast<qint64>(pos));
}

void MyMediaPlayer::getState(QMediaPlayer::State sta)
{
    qDebug() << "current player state:" << sta;

    state = sta;
}

void MyMediaPlayer::setPlayerState()
{
    //qDebug() << state;

    if (state == QMediaPlayer::StoppedState || state == QMediaPlayer::PausedState)
        this->play();

    else if (state == QMediaPlayer::PlayingState)
        this->pause();

    else
        qDebug() << "set state error";
}

void MyMediaPlayer::sendMetaData()
{
    //qDebug() << metaData("Year").toInt();
    //qDebug() << metaData("Title").toString();

    data.coverArtImage = this->metaData("ThumbnailImage").value<QImage>();

    data.title = this->metaData("Title").toString();
    data.author = this->metaData("Author").toString();
    data.albumTitle = this->metaData("AlbumTitle").toString();
    data.year = this->metaData("Year").toString();

    emit flashMetaData(data);
}
