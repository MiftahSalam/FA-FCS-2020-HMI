#include "gun_firing_service.h"
#include "src/shared/common/errors/err_object_creation.h"

GunFiringService *GunFiringService::instance = nullptr;

GunFiringService::GunFiringService(
        QObject *parent,
        SerialMessagingOpts *serialConfig,
        GunFeedbackRepository *gunStatusREpo,
        WeaponAssignService *waService,
        WeaponTrackAssignService *wtaService
        )
    :
      QObject{parent},
      _feedbackStatusRepository(gunStatusREpo),
      _waService(waService),
      _wtaService(wtaService),
      _openFire(true)
{
    auto weapons = waService->getAvailableWeapons();
    foreach (auto weapon, weapons) {
        firingPorts.insert(weapon, nullptr);
    }
    firingPorts.insert("40mm", new SerialMessagingWrapper(this, serialConfig));

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GunFiringService::OnTimeout);
}

void GunFiringService::OnWeaponAssign(const QString &weapon, const bool &assign)
{
    if (assign) {
        if (!_weaponsReady.contains(weapon)) {
            _weaponsOpenFire.removeAll(weapon);
            _weaponsReady.append(weapon);
        }
    } else {
        _weaponsOpenFire.removeAll(weapon);
        _weaponsReady.removeAll(weapon);
    }

    if (_weaponsReady.size() > 0) {
        timer->start(1000);
    } else {
        timer->stop();
    }
}

void GunFiringService::OnTimeout()
{
    foreach (auto wOpenFire, _weaponsOpenFire) {
        auto port_serial = firingPorts.value(wOpenFire);
        port_serial->sendActivateButtonMessage("activate");
    }
}

void GunFiringService::setOpenFire(const QString &weapon, bool open)
{
    if (_weaponsReady.contains(weapon)) {
        if (open) {
            _weaponsOpenFire.append(weapon);

        } else {
            _weaponsOpenFire.removeAll(weapon);
        }
    }
}

GunFiringService *GunFiringService::getInstance(QObject *parent,
                                                MessagingSerialConfig *serialConfig,
                                                GunFeedbackRepository *gunStatusREpo,
                                                WeaponAssignService *waService,
                                                WeaponTrackAssignService *wtaService
                                                )
{
    if(instance == nullptr)
    {
        if(serialConfig == nullptr)
        {
            throw ErrObjectCreation();
        }

        if(gunStatusREpo == nullptr) {
            throw ErrObjectCreation();
        }

        if(waService == nullptr) {
            throw ErrObjectCreation();
        }

        if(wtaService == nullptr) {
            throw ErrObjectCreation();
        }
        SerialMessagingOpts *gunFiringStreamVal = serialConfig->getInstance("")->getContent().value("fire_button");


        instance = new GunFiringService(parent, gunFiringStreamVal, gunStatusREpo, waService, wtaService);
    }

    return instance;
}
