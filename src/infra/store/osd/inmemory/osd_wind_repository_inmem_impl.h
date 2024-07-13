#ifndef OSDWINDREPOSITORYINMEMIMPL_H
#define OSDWINDREPOSITORYINMEMIMPL_H

#include "src/domain/osd/entity/osd_wind_entity.h"
#include "src/domain/osd/repository/osd_wind_repository.h"

class OSDWindRepositoryInMemImpl: public OSDWindRepository
{
public:
    static OSDWindRepository *GetInstance();

    // OSDBaseRepository interface
public:
    void SetSource(const std::string &source) override;
    void SetStatus(const std::string &status) override;
    void SetMode(const OSD_MODE &mode) override;

    // OSDInertiaRepository interface
public:
    void SetWind(const OSDWindEntity &wind) override;
    const OSDWindEntity *GetWind() const override;

protected:
    OSDWindRepositoryInMemImpl(OSDWindEntity *entity);

private:
    static OSDWindRepositoryInMemImpl *instance;

    OSDWindEntity *_entity;
};

#endif // OSDWINDREPOSITORYINMEMIMPL_H
