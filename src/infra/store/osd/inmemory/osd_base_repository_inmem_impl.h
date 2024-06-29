#ifndef OSD_BASE_REPOSITORY_INMEM_IMPL_H
#define OSD_BASE_REPOSITORY_INMEM_IMPL_H

#include "src/domain/osd/entity/osd_base_entity.h"
#include "src/domain/osd/repository/osd_base_repository.h"

class OSDBaseRepositoryInMemImpl: public OSDBaseRepository
{
public:
    void SetEntity(const OSDBaseEntity &entity);

protected:
    OSDBaseRepositoryInMemImpl(OSDBaseEntity* entity);
    OSDBaseEntity *_entity;
};

#endif // OSD_BASE_REPOSITORY_INMEM_IMPL_H
