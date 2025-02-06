#ifndef OSD_INPUT_MODE_REPOSITORY_INMEM_IMPL_H
#define OSD_INPUT_MODE_REPOSITORY_INMEM_IMPL_H

#include "src/domain/osd/entity/osd_mode_entity.h"
#include "src/domain/osd/repository/osd_mode_repository.h"

class OSDInputModeRepositoryInMemImpl: public OSDInputModeRepository
{
public:
    static OSDInputModeRepository *GetInstance();

    // OSDInputModeRepository interface
    virtual void SetMode(const OSDInputModeEntity &mode);
    virtual const OSDInputModeEntity *GetMode() const;

protected:
    OSDInputModeRepositoryInMemImpl(OSDInputModeEntity *entity);

private:
    static OSDInputModeRepositoryInMemImpl *instance;

    OSDInputModeEntity *_entity;
};

#endif // OSD_INPUT_MODE_REPOSITORY_INMEM_IMPL_H
