#ifndef OSDPOSITIONREPOSITORYIMPL_H
#define OSDPOSITIONREPOSITORYIMPL_H


#include "src/domain/osd/entity/osd_position_entity.h"
#include "src/domain/osd/repository/osd_base_repository.h"

class OSDPositionRepositoryInMemImpl: public OSDBaseRepository
{
private:
    OSDPositionRepositoryInMemImpl(OSDPositionEntity* entity);

    // OSDBaseRepository interface
public:
    void SetEntity(const OSDPositionEntity &entity);

    // OSDBaseRepository interface
public:
   static const OSDBaseRepository *GetInstance();

private:
    static OSDPositionRepositoryInMemImpl *instance;
};

#endif // OSDPOSITIONREPOSITORYIMPL_H
