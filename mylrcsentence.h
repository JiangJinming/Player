#ifndef MYLRCSENTENCE_H
#define MYLRCSENTENCE_H

#include <QObject>

class QString;

class MyLRCSentence
{
public:
    MyLRCSentence();
    void setSentence(const QString &str);
    QString getSentence();

    void setPosition(const qint64 &pos);
    qint64 getPosition();
private:
    qint64 position;
    QString sentence;
};

#endif // MYLRCSENTENCE_H
