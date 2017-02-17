#include "mytitlebarbutton.h"

#include <QPixmap>
#include <QColor>
#include <QPainter>
#include <QBrush>
#include <QDebug>
#include <QString>
#include <QMouseEvent>

MyTitleBarButton::MyTitleBarButton(QWidget *parent)
    : QWidget(parent)
{
    defaultIcon = new QPixmap(":/icon/default.png");
    clickIcon = new QPixmap(":/icon/clicked.png");
    mainIcon = *defaultIcon;

    backgroundColor = new QColor(Qt::white);
}

void MyTitleBarButton::setDefaultIcon(const QString &ico)
{
    QPixmap icon(ico);

    *defaultIcon = icon;
    mainIcon =*defaultIcon;

    this->update();
}

void MyTitleBarButton::setClickedIcon(const QString &ico)
{
    QPixmap icon(ico);

    *clickIcon = icon;
}

void MyTitleBarButton::setBackgroundColor(const QColor &color)
{
    *backgroundColor = color;
    this->update();
}

void MyTitleBarButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setBrush(QBrush(*backgroundColor));
    painter.drawRect(-1, -1, this->width() + 1, this->height() + 1);

    painter.drawPixmap(0, 0, this->width(), this->height(), mainIcon);
}

void MyTitleBarButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        //qDebug() << "press";

        mainIcon = *clickIcon;
        this->update();
    }

    QWidget::mousePressEvent(event);
}

void MyTitleBarButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
    //qDebug() << "release";

    mainIcon = *defaultIcon;
    this->update();

    emit buttonClicked();
    }

    QWidget::mouseReleaseEvent(event);
}
