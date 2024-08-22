#include "gun_feedback_status_model.h"

GunFeedbackStatusModel::GunFeedbackStatusModel(bool opMode, bool remote, bool mount, bool barrelTemperature, bool gunReadyToStart,
                                   bool gunReadyToFire, bool fireMode, bool blindArc, bool missAlignment, bool magazine):
    _opMode(opMode), _remote(remote), _mount(mount), _barrelTemp(barrelTemperature), _gunReadyToStart(gunReadyToStart),
    _gunReadyToFire(gunReadyToFire), _fireMode(fireMode), _blindArc(blindArc), _missAlignment(missAlignment), _magazine(magazine)
{

}

GunFeedbackStatusModel GunFeedbackStatusModel::fromJsonObject(QJsonObject obj)
{
    GunFeedbackStatusModel model(
        obj.value("op_mode").toBool(),
        obj.value("remote").toBool(),
        obj.value("mount").toBool(),
        obj.value("btemp").toBool(),
        obj.value("grtst").toBool(),
        obj.value("grtfr").toBool(),
        obj.value("fire_mode").toBool(),
        obj.value("blarc").toBool(),
        obj.value("misalgn").toBool(),
        obj.value("magazine").toBool()
        );
    return model;
}

bool GunFeedbackStatusModel::getOpMode() const
{
    return _opMode;
}

bool GunFeedbackStatusModel::getRemote() const
{
    return _remote;
}

bool GunFeedbackStatusModel::getMount() const
{
    return _mount;
}

bool GunFeedbackStatusModel::getBarrelTemperature() const
{
    return _barrelTemp;
}

bool GunFeedbackStatusModel::getGunReadyToStart() const
{
    return _gunReadyToStart;
}

bool GunFeedbackStatusModel::getGunReadyToFire() const
{
    return _gunReadyToFire;
}

bool GunFeedbackStatusModel::getFireMode() const
{
    return _fireMode;
}

bool GunFeedbackStatusModel::getBlindArc() const
{
    return _blindArc;
}

bool GunFeedbackStatusModel::getMissAlignment() const
{
    return _missAlignment;
}

bool GunFeedbackStatusModel::getMagazine() const
{
    return _magazine;
}
