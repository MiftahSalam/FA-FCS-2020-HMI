#ifndef WEAPON_ENGAGE_DATA_ENTITY_H
#define WEAPON_ENGAGE_DATA_ENTITY_H

class WeaponEngageDataEntity
{
public:
    WeaponEngageDataEntity(float azimuth, float elevation);

    float azimuth() const;
    void setAzimuth(float newAzimuth);
    float elevation() const;
    void setElevation(float newElevation);

private:
    float _azimuth;
    float _elevation;
};

#endif // WEAPON_ENGAGE_DATA_ENTITY_H
