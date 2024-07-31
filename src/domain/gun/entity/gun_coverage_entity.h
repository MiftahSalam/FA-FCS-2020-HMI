#ifndef GUN_COVERAGE_ENTITY_H
#define GUN_COVERAGE_ENTITY_H

class GunCoverageEntity
{
public:
    GunCoverageEntity(float max_range, float blind_arc, float gun_orientation);

    float getMax_range() const;
    void setMaxRange(float newMaxRange);
    float getBlindArc() const;
    void setBlindArc(float newBlindArc);
    float getGunOrientation() const;
    void setGunOrientation(float newGunOrientation);

private:
    float _maxRange;
    float _blindArc;
    float _gunOrientation;

};

#endif // GUN_COVERAGE_ENTITY_H
