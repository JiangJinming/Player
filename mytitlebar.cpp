#include "mytitlebar.h"
#include "mytitlebarbutton.h"

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
    titleColor = new QColor(240, 240, 240);

    trayIconButton = new MyTitleBarButton(this);
    trayIconButton->setFixedSize(20, 20);
    trayIconButton->setBackgroundColor(*titleColor);
    trayIconButton->setDefaultIcon(":/icon/titlebarIcons/maximizeDefaultIcon.png");
    trayIconButton->setClickedIcon(":/icon/titlebarIcons/maximizeClickIcon.png");

    minimizeButton = new MyTitleBarButton(this);
    minimizeButton->setFixedSize(20, 20);
    minimizeButton->setBackgroundColor(*titleColor);
    minimizeButton->setDefaultIcon(":/icon/titlebarIcons/minimizeDefaultIcon.png");
    minimizeButton->setClickedIcon(":/icon/titlebarIcons/minimizeClickIcon.png");

    closeButton = new MyTitleBarButton(this);
    closeButton->setFixedSize(20, 20);
    closeButton->setBackgroundColor(*titleColor);
    closeButton->setDefaultIcon(":/icon/titlebarIcons/closeDefaultIcon.png");
    closeButton->setClickedIcon(":/icon/titlebarIcons/closeClickIcon.png");

    QObject::connect(trayIconButton, SIGNAL(buttonClicked()), this, SLOT(trayIconButtonClicked()));
    QObject::connect(minimizeButton, SIGNAL(buttonClicked()), this, SLOT(minimizeButtonClicked()));
    QObject::connect(closeButton, SIGNAL(buttonClicked()), this, SLOT(closeButtonClicked()));

    mainHLayout = new QHBoxLayout(this);
    HLeftSpacerItem = new QSpacerItem(0, minimizeButton->height(),
                                  QSizePolicy::Expanding, QSizePolicy::Fixed);
    HRightSpacerItem = new QSpacerItem(15, minimizeButton->height(),
                                     QSizePolicy::Fixed, QSizePolicy::Fixed);

    mainHLayout->addItem(HLeftSpacerItem);
    mainHLayout->addWidget(trayIconButton);
    mainHLayout->addWidget(minimizeButton);
    mainHLayout->addWidget(closeButton);
    mainHLayout->addSpacerItem(HRightSpacerItem);
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

void MyTitleBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setBrush(QBrush(*titleColor));
    painter.drawRect(-1, -1, this->width() + 1, this->height() + 1);
}

void MyTitleBar::setTitleColor(const QColor &color)
{
    *titleColor = color;

    this->update();
}

void MyTitleBar::trayIconButtonClicked()
{
    //qDebug() << "emit 'Tray' signal";

    emit windowStateChanged(MyTitleBar::Tray);
}

void MyTitleBar::minimizeButtonClicked()
{
    //qDebug() << "emit 'Minimize' signal";

    emit windowStateChanged(MyTitleBar::Minimize);
}

void MyTitleBar::closeButtonClicked()
{
    //qDebug() << "emit 'Close' signal";

    emit windowStateChanged(MyTitleBar::Close);

}
