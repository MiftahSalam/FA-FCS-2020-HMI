#include "gun_status_feedback_entity.h"

GunStatusFeedbackEntity::GunStatusFeedbackEntity()
{

}

bool GunStatusFeedbackEntity::opMode() const
{
    return _opMode;
}

void GunStatusFeedbackEntity::setOpMode(bool newOpMode)
{
    _opMode = newOpMode;
}

bool GunStatusFeedbackEntity::remote() const
{
    return _remote;
}

void GunStatusFeedbackEntity::setRemote(bool newRemote)
{
    _remote = newRemote;
}

bool GunStatusFeedbackEntity::mount() const
{
    return _mount;
}

void GunStatusFeedbackEntity::setMount(bool newMount)
{
    _mount = newMount;
}

bool GunStatusFeedbackEntity::barrelTemperature() const
{
    return _barrelTemperature;
}

void GunStatusFeedbackEntity::setBarrelTemperature(bool newBarrelTemperature)
{
    _barrelTemperature = newBarrelTemperature;
}

bool GunStatusFeedbackEntity::gunReadyToStart() const
{
    return _gunReadyToStart;
}

void GunStatusFeedbackEntity::setGunReadyToStart(bool newGunReadyToStart)
{
    _gunReadyToStart = newGunReadyToStart;
}

bool GunStatusFeedbackEntity::gunReadyToFire() const
{
    return _gunReadyToFire;
}

void GunStatusFeedbackEntity::setGunReadyToFire(bool newGunReadyToFire)
{
    _gunReadyToFire = newGunReadyToFire;
}

bool GunStatusFeedbackEntity::fireMode() const
{
    return _fireMode;
}

void GunStatusFeedbackEntity::setFireMode(bool newFireMode)
{
    _fireMode = newFireMode;
}

bool GunStatusFeedbackEntity::blindArc() const
{
    return _blindArc;
}

void GunStatusFeedbackEntity::setBlindArc(bool newBlindArc)
{
    _blindArc = newBlindArc;
}

bool GunStatusFeedbackEntity::missAlignment() const
{
    return _missAlignment;
}

void GunStatusFeedbackEntity::setMissAlignment(bool newMissAlignment)
{
    _missAlignment = newMissAlignment;
}

bool GunStatusFeedbackEntity::magazine() const
{
    return _magazine;
}

void GunStatusFeedbackEntity::setMagazine(bool newMagazine)
{
    _magazine = newMagazine;
}
