#ifndef FRAMEOSD_H
#define FRAMEOSD_H

#ifdef WIN32
#include <QtRedis>
#else
#include "qredis/redis++.h"
#endif
#include <QFrame>
#include <QDebug>
#include <QTimer>
#include <QSettings>

namespace Ui {
class FrameOSD;
}

class FrameOSD : public QFrame
{
    Q_OBJECT

public:
    explicit FrameOSD(QWidget *parent = 0);
    ~FrameOSD();
    void setConfig (QString Config);

    QString getHeading() const;

private slots:
    void on_osdTimerTimeOut();

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
        QString status;
    };

    struct GpsData
    {
        QString latitude;
        QString longitude;
        QString status;
    };

    struct WindData
    {
        QString dir;
        QString speed;
        QString status;
    };

    struct WeatherData
    {
        QString temperature;
        QString pressure;
        QString humidity;
        QString status;
    };

    struct SpeedData
    {
        QString sog;
        QString cog;
        QString status;
    };

    struct WaterSpeedData
    {
        QString speed;
        QString course;
        QString status;
    };

    Ui::FrameOSD *ui;
#ifdef WIN32
    QtRedis *redisClient;
#else
    Redis *redisClient;
#endif
    InersiaData inersiadata;
    GpsData gpsdata;
    WindData winddata;
    WeatherData weatherdata;
    SpeedData speeddata;
    WaterSpeedData waterspeeddata;
    QString Config;
    QString curStatusString;

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

    void reconnecRedis();

};

#endif // FRAMEOSD_H
