#ifndef GUNFIRINGSERVICE_H
#define GUNFIRINGSERVICE_H

#include "qtimer.h"
//#include "src/infra/messaging/serial/serial_messaging_wrapper.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/model/gun/cms/gun_mode_barrel_response.h"
#include "src/shared/config/messaging_serial_config.h"
#include "src/usecase/engagement/weapon_track_assign_service.h"
#include "src/usecase/gun/gun_manager_service.h"
#include "src/usecase/weapon_assign/weapon_assign_service.h"
#include <QObject>

class GunFiringService : public QObject
{
    Q_OBJECT
public:
    explicit GunFiringService(QObject *parent = nullptr);

    GunFiringService(GunFiringService &other) = delete;
    void operator=(const GunFiringService &) = delete;
    static GunFiringService *getInstance(
            QObject *parent = nullptr,
//            MessagingSerialConfig *serialConfig = nullptr,
            MessagingTcpConfig *msgConfig = nullptr,
            GunManagerService *gunService = nullptr,
            WeaponAssignService *waService = nullptr,
            WeaponTrackAssignService *wtaService = nullptr);


    void setOpenFire(const QString &weapon, bool open);

protected:
    GunFiringService(
            QObject *parent = nullptr,
//            SerialMessagingOpts *serialConfig = nullptr,
            TcpMessagingOpts *tcpConfig = nullptr,
            GunManagerService *gunService = nullptr,
            WeaponAssignService *waService = nullptr,
            WeaponTrackAssignService *wtaService = nullptr
            );

signals:
    void signal_FiringChange(const QString &weapon, const bool open);

private slots:
    void OnWeaponAssign(BaseResponse<TrackAssignResponse> resp, bool assign);
    void onAssignModeChange(const QString &weapon, const WeaponAssign::WeaponAssignMode &mode);
    void onGunModeChange(BaseResponse<GunModeBarrelResponse> resp, bool needConfirm);
    void checkGunOfflineMode();
    void OnTimeout();

private:
    static GunFiringService *instance;

    QTimer* timer;
    SerialMessagingOpts *_cmsConfig;
    GunManagerService *_gunService;
    WeaponAssignService *_waService;
    WeaponTrackAssignService *_wtaService;
    QMap<QString, TcpMessagingWrapper*> firingPorts;
//    QMap<QString, SerialMessagingWrapper*> firingPorts;

    QStringList _weaponsReady;
    QStringList _weapons;
    QStringList _weaponsOpenFire;
    bool _openFire;
    bool _isFiringAvailable;
};

#endif // GUNFIRINGSERVICE_H
