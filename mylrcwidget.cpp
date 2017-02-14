#include "mylrcwidget.h"

#include <QString>
#include <QPaintEvent>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QDebug>
#include <QPainter>
#include <QRectF>
#include <QRegExp>

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
    else {
        file.close();
        analysisLrc(lrcVector);
    }

    this->update();
}

void MyLRCWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::black);

    int centerLine = (this->height() / LINESPACING) / 2;

    if (lrcVector.isEmpty())
        painter.drawText(QRectF(this->rect()), Qt::AlignCenter, QString(QObject::tr("No File")));
    else {
        //current line (first paint line)
        int currentLine = getCurrentLine();
        if (currentLine == -1)
            return;

        //draw current line (test
        painter.drawText(QRectF(0, centerLine * LINESPACING, this->width(), LINESPACING),
                         Qt::AlignHCenter, tr("center"));

        //draw up (test
        int i = currentLine;
        while (i != -1) {
            i--;
            painter.drawText(QRectF(0, i * LINESPACING, this->width(), LINESPACING),
                             Qt::AlignHCenter, tr("up"));
        }

        //draw down
        int j = currentLine;
        while (j < lrcVector.size()) {
            i++;
            painter.drawText(QRectF(0, j * LINESPACING, this->width(), LINESPACING),
                             Qt::AlignHCenter, tr("down"));
        }
    }
}

void MyLRCWidget::analysisLrc(QVector<MyLRCSentence> &lrc)
{
    MyLRCSentence sentence;
    QFile file(lrcFile);

    /**********test**********
    sentence.setSentence("test");
    lrc.push_back(sentence);
    ************************/

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "open LRC file fail (in analysisLrc function)";

        lrcVector.clear();
    }

    else {
        QTextStream in(&file);
        QString lineString;

        for (int i = 0; !in.atEnd(); i++) {
            lineString = in.readLine();

            QString pattern("\\[(\\d+):(\\d+(\\.\\d+)?)\\](.*)");
            QRegExp rx(pattern);

            int pos = lineString.indexOf(rx);
            //qDebug() << "pos: " << pos;
            if (pos < 0)
                continue;
            else {
                MyLRCSentence sent;
                qint64 position = (rx.cap(1).toInt() * 60 + rx.cap(2).toDouble()) * 1000;
                //qDebug() << rx.capturedTexts();
                //qDebug() << position;

                sent.setPosition(position);
                sent.setSentence(rx.cap(4));
                qDebug() << position << " " << rx.cap(4);

                lrc.push_back(sent);
            }
        }
    }
}

void MyLRCWidget::flashCurrentPosition(qint64 pos)
{
    currentPosition = pos;

    this->update();
}

int MyLRCWidget::getCurrentLine()
{
    int ret = 0;

    for (QVector<MyLRCSentence>::iterator iter = lrcVector.begin(); iter != lrcVector.end(); iter++) {
        ret++;
        if (iter->getPosition() == currentPosition)
            return ret;
    }

    return -1;
}
