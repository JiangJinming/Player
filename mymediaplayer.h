#ifndef MYMEDIAPLAYER_H
#define MYMEDIAPLAYER_H

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

private:
    QMediaPlayer::State state;
};

#endif // MYMEDIAPLAYER_H
