#ifndef MYMEDIAPLAYER_H
#define MYMEDIAPLAYER_H

#include "mymetadata.h"

#include <QMediaPlayer>

class QString;

class MyMediaPlayer : public QMediaPlayer
{
    Q_OBJECT

public:
    MyMediaPlayer(QObject *parent = 0);

public slots:
    void setPlayerPositionValue(int);

    //about player state
    void getState(QMediaPlayer::State);
    void setPlayerState();

private slots:
    void sendMetaData();
    void sendLyricsPath();

signals:
    void flashMetaData(const MyMetaData &);
    void expectLyricsPath(const QString &);

private:
    QMediaPlayer::State state;
    MyMetaData data;
    QString lyricsPath;
};

#endif // MYMEDIAPLAYER_H
