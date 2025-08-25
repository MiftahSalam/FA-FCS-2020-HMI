#ifndef TESTSUIT_H
#define TESTSUIT_H

#include <QObject>

class TestSuit : public QObject
{
    Q_OBJECT
public:
    TestSuit();
    ~TestSuit();

    static std::vector<QObject*> &suits();

signals:

};

#endif // TESTSUIT_H
