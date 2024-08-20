#include "gun_feedback_model.h"

GunFeedbackModel::GunFeedbackModel(bool opMode, bool remote, bool mount, bool barrelTemperature, bool gunReadyToStart,
                                   bool gunReadyToFire, bool fireMode, bool blindArc, bool missAlignment, bool magazine):
    _opMode(opMode), _remote(remote), _mount(mount), _barrelTemp(barrelTemperature), _gunReadyToStart(gunReadyToStart),
    _gunReadyToFire(gunReadyToFire), _fireMode(fireMode), _blindArc(blindArc), _missAlignment(missAlignment), _magazine(magazine)
{

}

GunFeedbackModel GunFeedbackModel::fromJsonObject(QJsonObject obj)
{
    GunFeedbackModel model(
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

bool GunFeedbackModel::getOpMode() const
{
    return _opMode;
}

bool GunFeedbackModel::getRemote() const
{
    return _remote;
}

bool GunFeedbackModel::getMount() const
{
    return _mount;
}

bool GunFeedbackModel::getBarrelTemperature() const
{
    return _barrelTemp;
}

bool GunFeedbackModel::getGunReadyToStart() const
{
    return _gunReadyToStart;
}

bool GunFeedbackModel::getGunReadyToFire() const
{
    return _gunReadyToFire;
}

bool GunFeedbackModel::getFireMode() const
{
    return _fireMode;
}

bool GunFeedbackModel::getBlindArc() const
{
    return _blindArc;
}

bool GunFeedbackModel::getMissAlignment() const
{
    return _missAlignment;
}

bool GunFeedbackModel::getMagazine() const
{
    return _magazine;
}
