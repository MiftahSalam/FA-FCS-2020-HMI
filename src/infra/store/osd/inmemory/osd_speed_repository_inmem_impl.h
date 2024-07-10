#ifndef OSDSPEEDREPOSITORYINMEMIMPL_H
#define OSDSPEEDREPOSITORYINMEMIMPL_H

#include "src/domain/osd/entity/osd_speed_entity.h"
#include "src/domain/osd/repository/osd_speed_repository.h"

class OSDSpeedRepositoryInMemImpl: public OSDSpeedRepository
{
public:
    static OSDSpeedRepository *GetInstance();

    void SetSource(const std::string &source) override;
    void SetStatus(const std::string &status) override;
    void SetMode(const OSD_MODE &mode) override;

    void SetSpeed(const OSDSpeedEntity &speed) override;
    const OSDSpeedEntity *GetSpeed() const override;

protected:
    OSDSpeedRepositoryInMemImpl(OSDSpeedEntity *entity);

private:
    static OSDSpeedRepositoryInMemImpl *instance;

    OSDSpeedEntity *_entity;

};

#endif // OSDSPEEDREPOSITORYINMEMIMPL_H
