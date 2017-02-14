#ifndef MYLRCWIDGET_H
#define MYLRCWIDGET_H

#define LINESPACING 10

#include "mylrcsentence.h"

#include <QWidget>
#include <QVector>

class QString;
class QPaintEvent;
class MyLRCSentence;

class MyLRCWidget : public QWidget
{
    Q_OBJECT

public:
    MyLRCWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);

private:
    void loadFile(const QString &fileName);
    void analysisLrc(QVector<MyLRCSentence> &lrc);

    QString lrcFile;
    QVector<MyLRCSentence> lrcVector;

    //time
    qint64 currentPosition;

    int getCurrentLine();

public slots:
    void flashCurrentPosition(qint64);
};

#endif // MYLRCWIDGET_H
