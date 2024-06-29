#ifndef OSDPOSITIONREPOSITORYIMPL_H
#define OSDPOSITIONREPOSITORYIMPL_H


#include "src/domain/osd/entity/osd_position_entity.h"
#include "src/infra/store/osd/inmemory/osd_base_repository_inmem_impl.h"

class OSDPositionRepositoryInMemImpl: public OSDBaseRepositoryInMemImpl
{
private:
    OSDPositionRepositoryInMemImpl(OSDPositionEntity* entity);

    // OSDBaseRepository interface
public:
    void SetEntity(const OSDPositionEntity &entity);

    // OSDBaseRepository interface
public:
   static OSDBaseRepository *GetInstance();

private:
    static OSDPositionRepositoryInMemImpl *instance;
};

#endif // OSDPOSITIONREPOSITORYIMPL_H
