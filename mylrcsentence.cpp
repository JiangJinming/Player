#include "mylrcsentence.h"

#include <QString>

MyLRCSentence::MyLRCSentence()
{
}

void MyLRCSentence::setSentence(const QString &str)
{
    sentence = str;
}

QString MyLRCSentence::getSentence() const
{
    return sentence;
}

void MyLRCSentence::setPosition(const qint64 &pos)
{
    position = pos;
}

qint64 MyLRCSentence::getPosition() const
{
    return position;
}
