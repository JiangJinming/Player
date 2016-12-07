#ifndef MYPLAYER_H
#define MYPLAYER_H

#include <QMainWindow>
#include <QMediaPlaylist>

#include "mymediaplayer.h"

class QListWidgetItem;
class QCloseEvent;
class QPoint;

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

    void setVolumeLabelValue(int);

    void setSliderPostionValue(qint64);
    void setPositionLabelValue(qint64);
    
    void addDir();
    void searchLocalFiles();

    void getPlaybackMode(QMediaPlaylist::PlaybackMode);
    void setPlaybackMode();

    void moveWindow(const QPoint &point);

private:
    Ui::MyPlayer *ui;

    //player core
    MyMediaPlayer *player;
    QMediaPlaylist *playList;
    void loadMedia();

    //control
    qint64 duration;
    qint64 position;
    int volume;
    QMediaPlaylist::PlaybackMode playbackMode = QMediaPlaylist::Loop;

    //save player information
    void saveCurrentInfo();

    //init
    void init();

protected:
    void closeEvent(QCloseEvent *);
};

#endif // MYPLAYER_H
