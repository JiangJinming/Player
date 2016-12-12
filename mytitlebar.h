#ifndef MYTITLEBAR_H
#define MYTITLEBAR_H

#include <QWidget>

class MyTitleBarButton;
class QMouseEvent;
class QPaintEvent;
class QPushButton;
class QHBoxLayout;
class QSpacerItem;
class QColor;
class QPoint;

class MyTitleBar : public QWidget
{
    Q_OBJECT

public:
    MyTitleBar(QWidget *parent = 0);
    ~MyTitleBar();

    enum WindowState{Minimize, Maximize, Close};

public slots:
    void setTitleColor(const QColor &color);

signals:
    void windowStateChanged(MyTitleBar::WindowState state);
    void dragWindow(const QPoint &point);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

    MyTitleBarButton *minimizeButton;
    MyTitleBarButton *maximizeButton;
    MyTitleBarButton *closeButton;

    QHBoxLayout *mainHLayout;
    QSpacerItem *HLeftSpacerItem;
    QSpacerItem *HRightSpacerItem;

    QColor *titleColor;

    bool mousePressed;
    QPoint startPoint;
};

#endif // MYTITLEBAR_H
