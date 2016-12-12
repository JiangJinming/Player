#ifndef MYTITLEBARBUTTON_H
#define MYTITLEBARBUTTON_H

#include <QWidget>

class QString;
class QMouseEvent;
class QPaintEvent;
class QPixmap;
class QColor;

class MyTitleBarButton : public QWidget
{
    Q_OBJECT

public:
    MyTitleBarButton(QWidget *parent = 0);

    void setDefaultIcon(const QString &ico);
    void setClickedIcon(const QString &ico);
    void setBackgroundColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void buttonClicked();

private:
    QPixmap *defaultIcon;
    QPixmap *clickIcon;
    QPixmap mainIcon;

    QColor *backgroundColor;

    bool buttonPressed;
};

#endif // MYTITLEBARBUTTON_H
