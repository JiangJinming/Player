#include "mymediaplayer.h"

#include <QVariant>
#include <QDebug>
#include <QImage>
#include <QString>

MyMediaPlayer::MyMediaPlayer(QObject *parent) :
    QMediaPlayer(parent)
{
    state = QMediaPlayer::StoppedState;

    QObject::connect(this, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(getState(QMediaPlayer::State)));
    //send meta data
    QObject::connect(this, SIGNAL(metaDataChanged()), this, SLOT(sendMetaData()));
    //send a expect lyrics
    QObject::connect(this, SIGNAL(metaDataChanged()), this, SLOT(sendLyricsPath()));
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

void MyMediaPlayer::sendLyricsPath()
{
    QString lyricsPath = this->currentMedia().canonicalUrl().path();
    lyricsPath.remove(0, 1);

    int i = lyricsPath.size() - 1;
    while (lyricsPath[i] != '.')
        i--;
    lyricsPath.remove(i, lyricsPath.size() - i);

    lyricsPath.push_back(tr(".lrc"));

    emit expectLyricsPath(lyricsPath);
}
