#include "mylrcsentence.h"

#include <QString>

MyLRCSentence::MyLRCSentence()
{
}

void MyLRCSentence::setSentence(const QString &str)
{
    sentence = str;
}

QString MyLRCSentence::getSentence()
{
    return sentence;
}
