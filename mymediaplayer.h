#ifndef MYMEDIAPLAYER_H
#define MYMEDIAPLAYER_H

#include "mymetadata.h"

#include <QMediaPlayer>

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

signals:
    void flashMetaData(const MyMetaData &);

private:
    QMediaPlayer::State state;
    MyMetaData data;
};

#endif // MYMEDIAPLAYER_H
