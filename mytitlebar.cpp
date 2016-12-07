#include "mytitlebar.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QDebug>

MyTitleBar::MyTitleBar(QWidget *parent)
    : QWidget(parent)
{
    minimizeButton = new QPushButton(this);
    maximizeButton = new QPushButton(this);
    closeButton = new QPushButton(this);

    mainHLayout = new QHBoxLayout(this);

    HSpacerItem = new QSpacerItem(0, minimizeButton->height(),
                                  QSizePolicy::Expanding, QSizePolicy::Fixed);

    titleColor = new QColor(198, 47, 47);

    mainHLayout->addItem(HSpacerItem);
    mainHLayout->addWidget(minimizeButton);
    mainHLayout->addWidget(maximizeButton);
    mainHLayout->addWidget(closeButton);
}

MyTitleBar::~MyTitleBar()
{

}

void MyTitleBar::mouseMoveEvent(QMouseEvent *event)
{
    //button() returned value is always Qt::NoButton for mouse move events
    if (mousePressed)
        emit dragWindow(event->pos() - startPoint);

    QWidget::mouseMoveEvent(event);
}

void MyTitleBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mousePressed = true;
        startPoint = event->pos();
    }
}

void MyTitleBar::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void MyTitleBar::mouseDoubleClickEvent(QMouseEvent *)
{
    emit windowStateChanged(MyTitleBar::Maximize);
}

void MyTitleBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setBrush(QBrush(*titleColor));
    painter.drawRect(0, 0, this->width(), this->height());
}

void MyTitleBar::setTitleColor(const QColor &color)
{
    *titleColor = color;

    this->update();
}
