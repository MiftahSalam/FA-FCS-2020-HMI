#ifndef OSD_SPEED_REPOSITORY_H
#define OSD_SPEED_REPOSITORY_H

#include "osd_base_repository.h"
#include "src/domain/osd/entity/osd_speed_entity.h"

class OSDSpeedRepository: public OSDBaseRepository
{
public:
    virtual void SetSpeed(const OSDSpeedEntity &speed) = 0;
    virtual const OSDSpeedEntity* GetSpeed() const = 0;
};

#endif // OSD_SPEED_REPOSITORY_H
