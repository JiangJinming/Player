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
}

void MyLRCWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::black);

    qreal centerLine = this->height() / 2;

    if (lrcVector.isEmpty())
        painter.drawText(QRectF(this->rect()), Qt::AlignCenter, QString(QObject::tr("No File")));
    else {
        //current line (first paint line)
        int currentLine = getCurrentLine();
        if (currentLine < 0)
            return;

        //qDebug() << "lrc start paint";

        //draw current line (test
        painter.setPen(Qt::red);
        painter.drawText(QRectF(0, centerLine, this->width(), LINESPACING),
                         Qt::AlignHCenter, lrcVector.at(currentLine).getSentence());

        painter.setPen(Qt::black);
        //draw up (test
        int i = 0;
        for (int index = currentLine - 1; index != -1; index--) {
            i++;

            painter.drawText(QRectF(0, centerLine - i * LINESPACING, this->width(), LINESPACING),
                             Qt::AlignHCenter, lrcVector.at(index).getSentence());
        }

        //draw down
        int j = 0;
        for (int index = currentLine + 1; index < lrcVector.size() - 1; index++) {
            j++;

            painter.drawText(QRectF(0, centerLine + j * LINESPACING, this->width(), LINESPACING),
                             Qt::AlignHCenter, lrcVector.at(index).getSentence());
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
                //qDebug() << position << " " << rx.cap(4);

                lrc.push_back(sent);
            }
        }
        qint64 positionEndMark = POSITIONENDMARK;
        QString sentenceEndMark = SENTENCEENDMARK;

        MyLRCSentence endMark;
        endMark.setPosition(positionEndMark);
        endMark.setSentence(sentenceEndMark);

        lrc.push_back(endMark);
    }
}

void MyLRCWidget::flashCurrentPosition(qint64 pos)
{
    currentPosition = pos;

    //qDebug() << "current position: " << currentPosition;

    this->update();
}

int MyLRCWidget::getCurrentLine()
{
    for (int i = 0; i < lrcVector.size(); i++) {

        //use 'end_mark' to avoid 'at(i + 1)' make error
        if (lrcVector.at(i).getPosition() == POSITIONENDMARK && lrcVector.at(i).getSentence() == SENTENCEENDMARK)
            break;

        if (lrcVector.at(i).getPosition() <= currentPosition && lrcVector.at(i + 1).getPosition() > currentPosition)
            return i;
    }

    return -1;
}
