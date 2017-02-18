#ifndef MYPLAYER_H
#define MYPLAYER_H

#include "mytitlebar.h"

#include <QMainWindow>
#include <QMediaPlaylist>
#include <QSystemTrayIcon>

class MyMediaPlayer;
class QListWidgetItem;
class QCloseEvent;
class QPoint;
class QAction;
class QMenu;

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

    //volume
    void setVolumeLabelValue(int);
    void getVolume(int);

    void setSliderPostionValue(qint64);
    void setPositionLabelValue(qint64);
    
    void addDir();
    void searchLocalFiles();

    void getPlaybackMode(QMediaPlaylist::PlaybackMode);
    void setPlaybackMode();

    //titlebar control mainWindow
    void moveWindow(const QPoint &point);
    void setMainWindowState(MyTitleBar::WindowState state);

    void dealWithTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::MyPlayer *ui;

    //player core
    MyMediaPlayer *player;
    QMediaPlaylist *playList;
    void loadMedia();

    //media control
    qint64 duration;
    qint64 position;
    int volume;
    QMediaPlaylist::PlaybackMode playbackMode = QMediaPlaylist::Loop;

    //save player information
    void saveCurrentInfo();

    //init
    void init();

    //control window state
    void hideWindow();
    void closeWindow();

    //trayIcon
    QAction *showTheWindow;
    QAction *exitProgram;

    QMenu *trayMenu;
    QSystemTrayIcon *trayIcon;

protected:
    void closeEvent(QCloseEvent *);
};

#endif // MYPLAYER_H
