#ifndef MYLRCWIDGET_H
#define MYLRCWIDGET_H

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
};

#endif // MYLRCWIDGET_H
