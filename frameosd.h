#ifndef FRAMEOSD_H
#define FRAMEOSD_H

//#include <QMessageBox>
#include <redis++.h>
#include <QFrame>
#include <QDebug>
#include <QTimer>
#include <QObject>

struct InersiaData
{
    QString heading;
    QString roll;
    QString picth;
};

struct GpsData
{
    QString latitude;
    QString longitude;
};

struct WindData
{
    QString dir;
    QString speed;
};

namespace Ui {
class FrameOSD;
}

class FrameOSD : public QFrame
{
    Q_OBJECT

public:
    explicit FrameOSD(QWidget *parent = 0);
    ~FrameOSD();

private slots:
    void on_osdGryoComboBox_activated(int index);
    void GyroTimerTimeOut();
    void on_pushButtonGyroApply_clicked();

    void on_comboBoxGPSMode_activated(int index);
    void GpsTimerTimeOut();
    void on_pushButtonGPSApply_clicked();

    void on_comboBoxWindMode_activated(int index);
    void WindTimerTimeOut();
    void on_pushButtonWindApply_clicked();

private:
    Ui::FrameOSD *ui;
    Redis *redisClient;
    InersiaData inersiadata;
    GpsData gpsdata;
    WindData winddata;

    void GyroAutoModeUi();
    void GyroManualModeUi();

    void GpsAutoModeUi();
    void GpsManualModeUi();

    void WindAutoModeUi();
    void WindManualModeUi();

};

#endif // FRAMEOSD_H
