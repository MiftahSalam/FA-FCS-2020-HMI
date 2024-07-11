#ifndef OSDWATERSPEEDREPOSITORY_H
#define OSDWATERSPEEDREPOSITORY_H

#include "osd_base_repository.h"
#include "src/domain/osd/entity/osd_waterspeed_entity.h"

class OSDWaterSpeedRepository: public OSDBaseRepository
{
public:
    virtual void SetWaterSpeed(const OSDWaterSpeedEntity &w_speed) = 0;
    virtual const OSDWaterSpeedEntity* GetWaterSpeed() const = 0;

};

#endif // OSDWATERSPEEDREPOSITORY_H
