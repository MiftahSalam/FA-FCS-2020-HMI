#include "gun_coverage_entity.h"

GunCoverageEntity::GunCoverageEntity(float max_range, float blind_arc, float gun_orientation)
{
    _maxRange = max_range;
    _blindArc = blind_arc;
    _gunOrientation = gun_orientation;
}

float GunCoverageEntity::getMax_range() const
{
    return _maxRange;
}

void GunCoverageEntity::setMaxRange(float newMaxRange)
{
    _maxRange = newMaxRange;
}

float GunCoverageEntity::getBlindArc() const
{
    return _blindArc;
}

void GunCoverageEntity::setBlindArc(float newBlindArc)
{
    _blindArc = newBlindArc;
}

float GunCoverageEntity::getGunOrientation() const
{
    return _gunOrientation;
}

void GunCoverageEntity::setGunOrientation(float newGunOrientation)
{
    _gunOrientation = newGunOrientation;
}
