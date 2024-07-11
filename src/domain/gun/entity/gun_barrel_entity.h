#ifndef GUN_BARREL_ENTITY_H
#define GUN_BARREL_ENTITY_H

class GunBarrelEntity
{
public:
    GunBarrelEntity(float azimuth, float elevation);

    float azimuth() const;
    void setAzimuth(float newAzimuth);
    float elevation() const;
    void setElevation(float newElevation);

private:
    float _azimuth;
    float _elevation;
};

#endif // GUN_BARREL_ENTITY_H
