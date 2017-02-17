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

    minimizeButton = new MyTitleBarButton(this);
    minimizeButton->setFixedSize(20, 20);
    minimizeButton->setBackgroundColor(*titleColor);
    minimizeButton->setDefaultIcon(":/icon/titlebarIcons/minimizeDefaultIcon.png");
    minimizeButton->setClickedIcon(":/icon/titlebarIcons/minimizeClickIcon.png");

    maximizeButton = new MyTitleBarButton(this);
    maximizeButton->setFixedSize(20, 20);
    maximizeButton->setBackgroundColor(*titleColor);
    maximizeButton->setDefaultIcon(":/icon/titlebarIcons/maximizeDefaultIcon.png");
    maximizeButton->setClickedIcon(":/icon/titlebarIcons/maximizeClickIcon.png");

    closeButton = new MyTitleBarButton(this);
    closeButton->setFixedSize(20, 20);
    closeButton->setBackgroundColor(*titleColor);
    closeButton->setDefaultIcon(":/icon/titlebarIcons/closeDefaultIcon.png");
    closeButton->setClickedIcon(":/icon/titlebarIcons/closeClickIcon.png");

    QObject::connect(minimizeButton, SIGNAL(buttonClicked()), this, SLOT(minimizeButtonClicked()));
    QObject::connect(maximizeButton, SIGNAL(buttonClicked()), this, SLOT(maximizeButtonClicked()));
    QObject::connect(closeButton, SIGNAL(buttonClicked()), this, SLOT(closeButtonClicked()));

    mainHLayout = new QHBoxLayout(this);
    HLeftSpacerItem = new QSpacerItem(0, minimizeButton->height(),
                                  QSizePolicy::Expanding, QSizePolicy::Fixed);
    HRightSpacerItem = new QSpacerItem(15, minimizeButton->height(),
                                     QSizePolicy::Fixed, QSizePolicy::Fixed);

    mainHLayout->addItem(HLeftSpacerItem);
    mainHLayout->addWidget(minimizeButton);
    mainHLayout->addWidget(maximizeButton);
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

void MyTitleBar::mouseDoubleClickEvent(QMouseEvent *)
{
    emit windowStateChanged(MyTitleBar::Maximize);
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

void MyTitleBar::minimizeButtonClicked()
{
    //qDebug() << "emit 'Minimize' signal";

    emit windowStateChanged(MyTitleBar::Minimize);
}

void MyTitleBar::maximizeButtonClicked()
{
    //qDebug() << "emit 'Maximize' signal";

    emit windowStateChanged(MyTitleBar::Maximize);
}

void MyTitleBar::closeButtonClicked()
{
    //qDebug() << "emit 'Close' signal";

    emit windowStateChanged(MyTitleBar::Close);

}
