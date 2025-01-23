#ifndef OSD_MODE_REPOSITORY_H
#define OSD_MODE_REPOSITORY_H

#include "src/domain/osd/entity/osd_mode_entity.h"
class OSDInputModeRepository
{
public:
    virtual void SetMode(const OSDInputModeEntity &mode) = 0;
    virtual const OSDInputModeEntity* GetMode() const = 0;
};

#endif // OSD_MODE_REPOSITORY_H
