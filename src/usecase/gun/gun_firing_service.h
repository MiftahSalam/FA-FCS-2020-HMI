#ifndef GUNFIRINGSERVICE_H
#define GUNFIRINGSERVICE_H

#include "qtimer.h"
#include "src/domain/gun/repository/gun_feedback_repository.h"
#include "src/infra/messaging/serial/serial_messaging_wrapper.h"
#include "src/shared/config/messaging_serial_config.h"
#include "src/usecase/engagement/weapon_track_assign_service.h"
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
            MessagingSerialConfig *serialConfig = nullptr,
            GunFeedbackRepository *gunStatusREpo = nullptr,
            WeaponTrackAssignService *wtaService = nullptr);


    void setOpenFire(const QString &weapon, bool open);

protected:
    GunFiringService(
            QObject *parent = nullptr,
            SerialMessagingOpts *serialConfig = nullptr,
            GunFeedbackRepository *gunStatusREpo = nullptr,
            WeaponTrackAssignService *wtaService = nullptr
            );

private slots:
    void OnWeaponAssign(const QString &weapon, const bool &assign);
    void OnTimeout();

private:
    static GunFiringService *instance;

    QTimer* timer;
    SerialMessagingOpts *_cmsConfig;
    GunFeedbackRepository *_feedbackStatusRepository;
    WeaponTrackAssignService *_wtaService;
    QMap<QString, SerialMessagingWrapper*> firingPorts;

    QStringList _weaponsReady;
    QStringList _weaponsOpenFire;
    bool _openFire;
    bool _isFiringAvailable;
};

#endif // GUNFIRINGSERVICE_H
