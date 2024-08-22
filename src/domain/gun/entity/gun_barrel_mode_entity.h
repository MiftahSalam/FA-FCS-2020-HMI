#ifndef GUNBARRELMODEENTITY_H
#define GUNBARRELMODEENTITY_H


class GunBarrelModeEntity
{
public:
    GunBarrelModeEntity(bool manualMode);

    bool getManualMode() const;
    void setManualMode(bool newManualMode);

private:
    bool manualMode;
};

#endif // GUNBARRELMODEENTITY_H
