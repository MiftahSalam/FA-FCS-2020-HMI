#ifndef OSD_BASE_REPOSITORY_H
#define OSD_BASE_REPOSITORY_H

#include "src/domain/osd/entity/osd_base_entity.h"

class OSDBaseRepository
{
public:
    virtual void SetEntity(const OSDBaseEntity &entity) = 0;
};
#endif // OSD_BASE_REPOSITORY_H
