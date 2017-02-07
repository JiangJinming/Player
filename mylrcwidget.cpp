#include "mylrcwidget.h"

#include <QString>
#include <QPaintEvent>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QDebug>
#include <QPainter>
#include <QRectF>

MyLRCWidget::MyLRCWidget(QWidget *parent)
    : QWidget(parent)
{
    loadFile("E:/Users/jiang/Desktop/项目相关/testmusic/Let Down.lrc");
}

void MyLRCWidget::loadFile(const QString &fileName)
{
    lrcFile = fileName;

    QFile file(lrcFile);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "open LRC file fail";

        lrcVector.clear();
    }
    else
        analysisLrc(lrcVector);

    this->update();
}

void MyLRCWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::black);

    qint64 currentDuration;
    QString currentSentence;
    
    if (lrcVector.isEmpty())
        painter.drawText(QRectF(this->rect()), Qt::AlignCenter, QString(QObject::tr("No File")));
    else {
        //test
        for (QVector<MyLRCSentence>::iterator iter = lrcVector.begin(); iter != lrcVector.end(); iter++) {
            currentSentence = iter->getSentence();
            painter.drawText(QRectF(this->rect()), Qt::AlignHCenter, currentSentence);
        }
    }
}

void MyLRCWidget::analysisLrc(QVector<MyLRCSentence> &lrc)
{
    MyLRCSentence sentence;
    QFile file(lrcFile);

    //test
    sentence.setSentence("test");
    lrc.push_back(sentence);


}
