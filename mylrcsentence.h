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
    
private:
    qint64 duration;
    QString sentence;
};

#endif // MYLRCSENTENCE_H
