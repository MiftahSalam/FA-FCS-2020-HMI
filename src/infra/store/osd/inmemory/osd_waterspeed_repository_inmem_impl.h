#ifndef OSDWATERSPEEDREPOSITORYINMEMIMPL_H
#define OSDWATERSPEEDREPOSITORYINMEMIMPL_H

#include "src/domain/osd/entity/osd_waterspeed_entity.h"
#include "src/domain/osd/repository/osd_waterspeed_repository.h"

class OSDWaterSpeedRepositoryInMemImpl: public OSDWaterSpeedRepository
{
public:
   static OSDWaterSpeedRepository *GetInstance();

   // OSDBaseRepository interface
public:
   void SetSource(const std::string &source) override;
   void SetStatus(const std::string &status) override;
   void SetMode(const OSD_MODE &mode) override;

public:
    void SetWaterSpeed(const OSDWaterSpeedEntity &w_speed) override;
    const OSDWaterSpeedEntity* GetWaterSpeed() const override;

protected:
    OSDWaterSpeedRepositoryInMemImpl(OSDWaterSpeedEntity* entity);

private:
    static OSDWaterSpeedRepositoryInMemImpl *instance;

    OSDWaterSpeedEntity *_entity;
};

#endif // OSDWATERSPEEDREPOSITORYINMEMIMPL_H
