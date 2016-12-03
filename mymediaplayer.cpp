#include "mymediaplayer.h"

MyMediaPlayer::MyMediaPlayer(QObject *parent) :
    QMediaPlayer(parent)
{
    state = QMediaPlayer::StoppedState;

    QObject::connect(this, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(getState(QMediaPlayer::State)));
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
