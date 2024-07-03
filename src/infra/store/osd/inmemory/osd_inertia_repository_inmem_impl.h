#ifndef OSDINERTIAREPOSITORYINMEMIMPL_H
#define OSDINERTIAREPOSITORYINMEMIMPL_H


#include "src/domain/osd/entity/osd_inertia_entity.h"
#include "src/domain/osd/repository/osd_inertia_repository.h"

class OSDInertiaRepositoryInMemImpl: public OSDInertiaRepository
{
public:
    static OSDInertiaRepository *GetInstance();

    // OSDBaseRepository interface
public:
    void SetSource(const std::string &source) override;
    void SetStatus(const std::string &status) override;
    void SetMode(const OSD_MODE &mode) override;

    // OSDInertiaRepository interface
public:
    void SetInertia(const OSDInertiaEntity &inertia) override;
    const OSDInertiaEntity *GetInertia() const override;

protected:
    OSDInertiaRepositoryInMemImpl(OSDInertiaEntity *entity);

private:
    static OSDInertiaRepositoryInMemImpl *instance;

    OSDInertiaEntity *_entity;
};

#endif // OSDINERTIAREPOSITORYINMEMIMPL_H
