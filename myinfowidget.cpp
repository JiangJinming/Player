#include "myinfowidget.h"
#include "mymetadata.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>
#include <QFont>
#include <QString>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QVariant>
#include <QDebug>

MyInfoWidget::MyInfoWidget(QWidget *parent)
    : QWidget(parent)
{
    /*********abandon********
    WidgetTitle = new QLabel(this);
    WidgetTitle->setFont(QFont("Microsoft Yahei", 14));
    WidgetTitle->setText(QObject::tr("Info"));
    ***********************/

    //default cover icon
    defaultCoverIcon = new QPixmap(":/icon/coverIcon.png");

    coverArtImageLabel = new QLabel(this);
    coverArtImageLabel->setPixmap(*defaultCoverIcon);

    titleLabel = new QLabel(this);
    titleLabel->setFont(QFont("Microsoft Yahei", 14, -1, "italic"));
    titleLabel->setText("title");

    authorLabel = new QLabel(this);
    authorLabel->setFont(QFont("Microsoft Yahei", 14));
    authorLabel->setText("author");

    albumTitleLabel = new QLabel(this);
    albumTitleLabel->setFont(QFont("Microsoft Yahei", 14));
    albumTitleLabel->setText("album");

    yearLabel = new QLabel(this);
    yearLabel->setFont(QFont("Microsoft Yahei", 14));
    yearLabel->setText("year");

    mainLayout = new QVBoxLayout(this);
    spacer = new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding);
    //mainLayout->addWidget(WidgetTitle);
    mainLayout->addWidget(coverArtImageLabel);
    mainLayout->addSpacerItem(spacer);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(authorLabel);
    mainLayout->addWidget(albumTitleLabel);
    mainLayout->addWidget(yearLabel);

    this->setLayout(mainLayout);
}

void MyInfoWidget::flashWidgetInfo(const MyMetaData &data)
{
    qDebug() << "flashed info widget";

    if (data.title != 0)
        this->titleLabel->setText(data.title);
    if (data.author != 0)
        this->authorLabel->setText(data.author);
    if (data.albumTitle != 0)
        this->albumTitleLabel->setText(data.albumTitle);
    if (data.year != 0)
        this->yearLabel->setText(data.year);
}
