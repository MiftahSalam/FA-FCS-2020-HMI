#ifndef GUNFEEDBACKSTREAM_H
#define GUNFEEDBACKSTREAM_H

#include <QObject>

class GunFeedbackStream : public QObject
{
    Q_OBJECT
public:
    explicit GunFeedbackStream(QObject *parent = nullptr);

signals:

};

#endif // GUNFEEDBACKSTREAM_H
