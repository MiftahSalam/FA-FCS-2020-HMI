#ifndef GUN_COVERAGE_ENTITY_H
#define GUN_COVERAGE_ENTITY_H

class GunCoverageEntity
{
public:
    GunCoverageEntity(float max_range, float blind_arc, float gun_orientation);

    float max_range() const;
    void setMaxRange(float newMaxRange);
    float blind_arc() const;
    void setBlindArc(float newBlindArc);
    float gun_orientation() const;
    void setGunOrientation(float newGunOrientation);

private:
    float _maxRange;
    float _blindArc;
    float _gunOrientation;

};

#endif // GUN_COVERAGE_ENTITY_H
