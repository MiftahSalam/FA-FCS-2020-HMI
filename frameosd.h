#ifndef FRAMEOSD_H
#define FRAMEOSD_H

//#include <QMessageBox>
#include <hiredis/hiredis.h>
#include <redis++.h>
#include <QFrame>
#include <QDebug>
#include <QTimer>

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

    void on_comboBoxWeatherMode_activated(int index);
    void WeatherTimerTimeOut();
    void on_pushButtonWeather_clicked();

    void on_comboBoxSpeedMode_activated(int index);
    void SpeedTimerTimeOut();
    void on_pushButtonSpeedApply_clicked();

    void on_comboBoxWaterMode_activated(int index);
    void WaterSpeedTimerTimeOut();
    void on_pushButtonWaterApply_clicked();

private:
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

    struct WeatherData
    {
        QString temperature;
        QString pressure;
        QString humidity;
    };

    struct SpeedData
    {
        QString sog;
        QString cog;
    };

    struct WaterSpeedData
    {
        QString speed;
        QString course;
    };

    Ui::FrameOSD *ui;
    Redis *redisClient;
    InersiaData inersiadata;
    GpsData gpsdata;
    WindData winddata;
    WeatherData weatherdata;
    SpeedData speeddata;
    WaterSpeedData waterspeeddata;

    void GyroAutoModeUi();
    void GyroManualModeUi();

    void GpsAutoModeUi();
    void GpsManualModeUi();

    void WindAutoModeUi();
    void WindManualModeUi();

    void WeatherAutoModeUi();
    void WeatherManualModeUi();

    void SpeedAutoModeUi();
    void SpeedManualModeUi();

    void WaterSpeedAutoModeUi();
    void WaterSpeedManualModeUi();

};

#endif // FRAMEOSD_H
