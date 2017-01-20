#ifndef MYINFOWIDGET_H
#define MYINFOWIDGET_H

#include <QWidget>

class QLabel;
class QVBoxLayout;
class QPixmap;
class QString;
class QSpacerItem;
class QVariant;
class MyMetaData;

class MyInfoWidget : public QWidget
{
    Q_OBJECT

public:
    MyInfoWidget(QWidget *parent = 0);

public slots:
    //void metaDataChanged(const QString &key, const QVariant &value)

    void flashWidgetInfo(const MyMetaData &data);

private:
    //QLabel *WidgetTitle;

    QLabel *coverArtImageLabel;
    QLabel *titleLabel;
    QLabel *authorLabel;
    QLabel *albumTitleLabel;
    QLabel *yearLabel;

    QPixmap *defaultCoverIcon;

    QVBoxLayout *mainLayout;
    QSpacerItem *spacer;
};

#endif // MYINFOWIDGET_H
