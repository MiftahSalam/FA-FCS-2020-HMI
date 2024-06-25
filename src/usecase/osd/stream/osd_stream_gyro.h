#ifndef OSD_STREAM_GYRO_H
#define OSD_STREAM_GYRO_H

#include <QObject>
#include <QWidget>

class osd_stream_gyro : public QObject
{
    Q_OBJECT
public:
    explicit osd_stream_gyro(QObject *parent = nullptr);

signals:

};

#endif // OSD_STREAM_GYRO_H
