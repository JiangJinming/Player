#ifndef MYMETADATA_H
#define MYMETADATA_H

#include <QString>
#include <QImage>

class MyMetaData
{
public:
    MyMetaData();

    QImage coverArtImage;

    QString title;
    QString author;
    QString albumTitle;
    QString year;
};

#endif // MYMETADATA_H
