#ifndef FRAMEGUN_H
#define FRAMEGUN_H

#ifdef WIN32
#include <QtRedis>
#else
#include "qredis/redis++.h"
#endif
#include <QFrame>
#include <QTimer>


namespace Ui {
class FrameGun;
}

class FrameGun : public QFrame
{
    Q_OBJECT

public:
    explicit FrameGun(QWidget *parent = 0);
    ~FrameGun();
    void setConfig (QString Config);

    QStringList getGundata() const;

    QString getAccessStatus() const;
    QString getReadyStatus() const;

private slots:
    void on_gunTimerTimeOut();

    void on_comboBoxGunBarControlMode_activated(const QString &arg1);

    void on_pushButtonGunBarControlApply_clicked();

private:
    struct GunBalistic
    {
        QString orientation;
        QString blind_arc;
        QString max_range;
    };

    struct GunStatus
    {
        QString technical_status;
        QString access_status;
        QString ready_to_start_status;
        QString gun_ready_status;
        QString gun_follow_status;
        QString azimuth;
        QString elevation;
    };

    struct OperationalStatus
    {
        QString operational;
        QString assign_mode;
    };

    struct EngagementStatus
    {
        QString engagement;
        QString azimuth;
        QString elevation;
    };

    Ui::FrameGun *ui;
#ifdef WIN32
    QtRedis *redisClient;
#else
    Redis *redisClient;
#endif
    GunBalistic gunbalistic;
    GunStatus gunstatus;
    OperationalStatus operationalstatus;
    EngagementStatus engagementstatus;
    QString Config;
    QString curStatusString;

    void reconnecRedis();

};

#endif // FRAMEGUN_H
