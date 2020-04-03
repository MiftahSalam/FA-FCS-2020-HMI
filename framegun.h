#ifndef FRAMEGUN_H
#define FRAMEGUN_H

#include <redis++.h>
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

private slots:
    void on_gunTimerTimeOut();

    void on_comboBoxGunBarControlMode_activated(const QString &arg1);

    void on_pushButtonGunBarControlApply_clicked();

private:
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
    };

    struct EngagementStatus
    {
        QString engagement;
        QString azimuth;
        QString elevation;
    };

    Ui::FrameGun *ui;
    Redis *redisClient;
    GunStatus gunstatus;
    OperationalStatus operationalstatus;
    EngagementStatus engagementstatus;
    QString Config;
    QString curStatusString;

};

#endif // FRAMEGUN_H
