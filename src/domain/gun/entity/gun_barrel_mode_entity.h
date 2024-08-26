#ifndef GUNBARRELMODEENTITY_H
#define GUNBARRELMODEENTITY_H


class GunBarrelModeEntity
{
public:
    enum MODE{
        NONE = 0,
        MANUAL,
        AUTO
    };

    GunBarrelModeEntity(MODE mode);

    MODE getMode() const;
    void setMode(MODE newManualMode);

private:
    MODE _mode;
};

#endif // GUNBARRELMODEENTITY_H
