#ifndef IOSDSTREAM_H
#define IOSDSTREAM_H


#include <QByteArray>

template <typename Model> class IOSDStream
{
public:
    virtual void onDataReceived(QByteArray data) = 0;
    virtual void signalDataProcessed(Model data) = 0;
};


#endif // IOSDSTREAM_H
