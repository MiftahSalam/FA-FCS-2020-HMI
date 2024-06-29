#ifndef OSD_BASE_REPOSITORY_H
#define OSD_BASE_REPOSITORY_H

#include "src/domain/osd/entity/osd_base_entity.h"
class OSDBaseRepository
{
public:
    void SetEntity(const OSDBaseEntity &entity);

protected:
    OSDBaseRepository(OSDBaseEntity* entity);
    OSDBaseEntity *_entity;
};
#endif // OSD_BASE_REPOSITORY_H
