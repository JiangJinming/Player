#ifndef MYPLAYER_H
#define MYPLAYER_H

#include <QMainWindow>
#include <QMediaPlayer>

class QMediaPlaylist;
class QListWidgetItem;

namespace Ui {
class MyPlayer;
}

class MyPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyPlayer(QWidget *parent = 0);
    ~MyPlayer();

public slots:
    void getDuration(qint64);
    void getState(QMediaPlayer::State);

    void setVolumeLabelValue(int);

    void setPlayerPositionValue(int);
    void setSliderPostionValue(qint64);
    void setPositionLabelValue(qint64);
    
    void changePlayerState();

    void addDir();
    void searchLocalFiles();

private:
    Ui::MyPlayer *ui;

    //player core
    QMediaPlayer *player;
    QMediaPlaylist *playList;
    void loadMedia();

    //control
    qint64 duration;
    QMediaPlayer::State state = QMediaPlayer::StoppedState;
    qint64 position;
    int volume;
};

#endif // MYPLAYER_H
