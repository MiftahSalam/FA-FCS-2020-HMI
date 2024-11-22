#include "gun_firing_service.h"
#include "src/shared/common/errors/err_object_creation.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, GunFiringService)
#else
#include <QDebug>
#endif

const QString FIRE_CMD_TEMPLATE = "$FIRE,%1,%2*HH\r\n";

GunFiringService *GunFiringService::instance = nullptr;

GunFiringService::GunFiringService(QObject *parent,
                                   //        SerialMessagingOpts *serialConfig,
                                   TcpMessagingOpts *tcpConfig,
                                   GunManagerService *gunService,
                                   WeaponAssignService *waService,
                                   WeaponTrackAssignService *wtaService
                                   )
    :
      QObject{parent},
      _gunService(gunService),
      _waService(waService),
      _wtaService(wtaService),
      _openFire(true)
{
    _weapons = waService->getAvailableWeapons();
    foreach (auto weapon, _weapons) {
        firingPorts.insert(weapon, nullptr);
    }
    //    firingPorts.insert("40mm", new SerialMessagingWrapper(this, serialConfig));
    firingPorts.insert("40mm", new TcpMessagingWrapper(this, tcpConfig));

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GunFiringService::OnTimeout);

    timer->start(1000);
}

void GunFiringService::OnWeaponAssign(BaseResponse<TrackAssignResponse> resp, bool assign)
{
    if (resp.getHttpCode() != 0)
    {
#ifdef USE_LOG4QT
        logger()->debug() << Q_FUNC_INFO << " -> error resp code: " << resp.getHttpCode() << ", resp msg: " << resp.getMessage();
#else
        qDebug() << Q_FUNC_INFO << "error resp code:" << resp.getHttpCode() << "resp msg:" << resp.getMessage();
#endif
        return;
    }

    QString weapon = QString::fromStdString(resp.getData().getWeapon());

    if (assign) {
        if (!_weaponsReady.contains(weapon)) {
            _weaponsOpenFire.removeAll(weapon);
            _weaponsReady.append(weapon);
        }
    } else {
        _weaponsOpenFire.removeAll(weapon);
        _weaponsReady.removeAll(weapon);
    }

    emit signal_FiringChange(weapon, _weaponsReady.contains(weapon));
}

void GunFiringService::onAssignModeChange(const QString &weapon, const WeaponAssign::WeaponAssignMode &mode)
{
    auto gun_mode = _gunService->getBarrelMode();

    if (mode == WeaponAssign::NONE && gun_mode == GunBarrelModeEntity::AUTO) {
        _weaponsOpenFire.removeAll(weapon);
        _weaponsReady.removeAll(weapon);

        emit signal_FiringChange(weapon, _weaponsReady.contains(weapon));
    }
}

void GunFiringService::onGunModeChange(BaseResponse<GunModeBarrelResponse> resp, bool needConfirm)
{
    Q_UNUSED(resp);
    Q_UNUSED(needConfirm);

    auto gun_mode = _gunService->getBarrelMode();
    bool state_change = false;

    switch (gun_mode) {
    case GunBarrelModeEntity::NONE:
    case GunBarrelModeEntity::AUTO:
        state_change = true;
        _weaponsOpenFire.removeAll("40mm");
        _weaponsReady.removeAll("40mm");
        break;
    case GunBarrelModeEntity::MANUAL:
        if (!_weaponsReady.contains("40mm")) {
            state_change = true;
            _weaponsOpenFire.removeAll("40mm");
            _weaponsReady.append("40mm");
        }
        break;
    default:
        break;
    }

    if (state_change) {
        emit signal_FiringChange("40mm", _weaponsReady.contains("40mm"));
    }
}

void GunFiringService::checkGunOfflineMode()
{
    auto gun_tech_mode = _gunService->getCurrentTechStat();

    if (gun_tech_mode == GunManagerService::OFFLINE) {
        auto gun_mode = _gunService->getBarrelMode();
        bool state_change = false;

        switch (gun_mode) {
        case GunBarrelModeEntity::NONE:
        case GunBarrelModeEntity::AUTO:
            state_change = true;
            _weaponsOpenFire.removeAll("40mm");
            _weaponsReady.removeAll("40mm");
            break;
        case GunBarrelModeEntity::MANUAL:
            if (!_weaponsReady.contains("40mm")) {
                state_change = true;
                _weaponsOpenFire.removeAll("40mm");
                _weaponsReady.append("40mm");
            }
            break;
        default:
            break;
        }

        if (state_change) {
            emit signal_FiringChange("40mm", _weaponsReady.contains("40mm"));
        }
    }
}

void GunFiringService::OnTimeout()
{
    checkGunOfflineMode();

    QString single = _gunService->getStatusSingleShot() ? "1" : "0";
    foreach (auto w, _weapons) {
        auto port = firingPorts.value(w);

        port->checkConnection();

        if (_weaponsOpenFire.contains(w)) {
            port->sendMessage(FIRE_CMD_TEMPLATE.arg("1").arg(single).toUtf8());
            //            port->sendActivateButtonMessage(FIRE_CMD_TEMPLATE.arg("1").toUtf8());
        } else {
            //            port->sendActivateButtonMessage(FIRE_CMD_TEMPLATE.arg("0").toUtf8());
            port->sendMessage(FIRE_CMD_TEMPLATE.arg("0").arg(single).toUtf8());
        }
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
                                                MessagingTcpConfig *msgConfig,
                                                GunManagerService *gunService,
                                                WeaponAssignService *waService,
                                                WeaponTrackAssignService *wtaService
                                                )
{
    if(instance == nullptr)
    {
        if(msgConfig == nullptr)
        {
            throw ErrObjectCreation();
        }

        if(gunService == nullptr) {
            throw ErrObjectCreation();
        }

        if(waService == nullptr) {
            throw ErrObjectCreation();
        }

        if(wtaService == nullptr) {
            throw ErrObjectCreation();
        }

        TcpMessagingOpts *gunFiringStreamVal = msgConfig->getInstance("")->getContent().value("fire_button");
        //        SerialMessagingOpts *gunFiringStreamVal = msgConfig->getInstance("")->getContent().value("fire_button");

        instance = new GunFiringService(parent, gunFiringStreamVal, gunService, waService, wtaService);

        connect(gunService, &GunManagerService::OnBarrelModeResponse,
                instance, &GunFiringService::onGunModeChange);
        connect(waService, &WeaponAssignService::OnAssignModeChange,
                instance, &GunFiringService::onAssignModeChange);
        connect(wtaService, &WeaponTrackAssignService::signal_assignmentResponseData, instance, &GunFiringService::OnWeaponAssign);
    }

    return instance;
}
