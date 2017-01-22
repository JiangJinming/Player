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
    coverArtImageLabel->setScaledContents(true);
    coverArtImageLabel->setPixmap(*defaultCoverIcon);
    coverArtImageLabel->setMaximumSize(200, 200);

    titleLabel = new QLabel(this);
    titleLabel->setFont(QFont("Microsoft Yahei", 14, -1, "italic"));
    titleLabel->setText(QObject::tr(("title")));

    authorLabel = new QLabel(this);
    authorLabel->setFont(QFont("Microsoft Yahei", 14));
    authorLabel->setText(QObject::tr(("author")));

    albumTitleLabel = new QLabel(this);
    albumTitleLabel->setFont(QFont("Microsoft Yahei", 14));
    albumTitleLabel->setText(QObject::tr(("album")));

    yearLabel = new QLabel(this);
    yearLabel->setFont(QFont("Microsoft Yahei", 14));
    yearLabel->setText(QObject::tr(("year")));

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

    //cover
    if (data.coverArtImage.isNull())
        this->coverArtImageLabel->setPixmap(QPixmap(":/icon/coverIcon.png"));
    else {
        QPixmap pixmap = QPixmap::fromImage(data.coverArtImage);
        this->coverArtImageLabel->setPixmap(pixmap);
    }

    //title
    if (data.title.isEmpty())
        this->titleLabel->setText(QObject::tr("title: null"));
    else
        this->titleLabel->setText(data.title);

    //author
    if (data.author.isEmpty())
        this->authorLabel->setText(QObject::tr("author: null"));
    else
        this->authorLabel->setText(data.author);

    //album
    if (data.albumTitle.isEmpty())
        this->albumTitleLabel->setText(QObject::tr("album: null"));
    else
        this->albumTitleLabel->setText(data.albumTitle);

    //year
    if (data.year.isEmpty())
        this->yearLabel->setText(QObject::tr("year: null"));
    else
        this->yearLabel->setText(data.year);
}
