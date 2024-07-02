#ifndef OSDPOSITIONREPOSITORYIMPL_H
#define OSDPOSITIONREPOSITORYIMPL_H


#include "src/domain/osd/entity/osd_position_entity.h"
#include "src/domain/osd/repository/osd_position_repository.h"

class OSDPositionRepositoryInMemImpl: public OSDPositionRepository
{
public:
   static OSDPositionRepository *GetInstance();

   // OSDBaseRepository interface
public:
   void SetSource(const std::string &source) override;
   void SetStatus(const std::string &status) override;
   void SetMode(const OSD_MODE &mode) override;

   // OSDPositionRepository interface
public:
    void SetPosition(const OSDPositionEntity &pos) override;
    const OSDPositionEntity* GetPosition() const override;

protected:
    OSDPositionRepositoryInMemImpl(OSDPositionEntity* entity);

private:
    static OSDPositionRepositoryInMemImpl *instance;

    OSDPositionEntity *_entity;
};

#endif // OSDPOSITIONREPOSITORYIMPL_H
