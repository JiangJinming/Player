#ifndef MYMETADATA_H
#define MYMETADATA_H

#include <QString>
#include <QVariant>

class MyMetaData
{
public:
    MyMetaData();

    QVariant coverArtImage;

    QString title;
    QString author;
    QString albumTitle;
    QString year;
};

#endif // MYMETADATA_H
