#include "gun_firing_service.h"
#include "src/shared/common/errors/err_object_creation.h"

const QString FIRE_CMD_TEMPLATE = "$FIRE,%1*HH\r\n";

GunFiringService *GunFiringService::instance = nullptr;

GunFiringService::GunFiringService(
        QObject *parent,
//        SerialMessagingOpts *serialConfig,
        TcpMessagingOpts *tcpConfig,
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
        qDebug() << Q_FUNC_INFO << "error resp code:" << resp.getHttpCode() << "resp msg:" << resp.getMessage();
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

void GunFiringService::OnTimeout()
{
    foreach (auto w, _weapons) {
        auto port = firingPorts.value(w);

        port->checkConnection();

        if (_weaponsOpenFire.contains(w)) {
            port->sendMessage(FIRE_CMD_TEMPLATE.arg("1").toUtf8());
//            port->sendActivateButtonMessage(FIRE_CMD_TEMPLATE.arg("1").toUtf8());
        } else {
//            port->sendActivateButtonMessage(FIRE_CMD_TEMPLATE.arg("0").toUtf8());
            port->sendMessage(FIRE_CMD_TEMPLATE.arg("0").toUtf8());
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
                                                GunFeedbackRepository *gunStatusREpo,
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

        if(gunStatusREpo == nullptr) {
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

        instance = new GunFiringService(parent, gunFiringStreamVal, gunStatusREpo, waService, wtaService);

        connect(wtaService, &WeaponTrackAssignService::signal_assignmentResponseData, instance, &GunFiringService::OnWeaponAssign);
    }

    return instance;
}
