#ifndef OSD_DATETIME_REPOSITORY_INMEM_IMPL_H
#define OSD_DATETIME_REPOSITORY_INMEM_IMPL_H

#include "src/domain/osd/entity/osd_datetime_entity.h"
#include "src/domain/osd/repository/osd_datetime_repository.h"

class OSDDateTimeRepositoryInMemImpl: public OSDDateTimeRepository
{
public:
    static OSDDateTimeRepository *GetInstance();

public:
    void SetSource(const std::string &source) override;
    void SetStatus(const std::string &status) override;
    void SetMode(const OSD_MODE &mode) override;

public:
    void SetDateTime(const OSDDateTimeEntity &date_time) override;
    const OSDDateTimeEntity *GetDateTime() const override;
    void UpdateTimeLocalDisplay(const long long &epoch) override;
    void UpdateTimeDisplay(const long long &epoch) override;

protected:
    OSDDateTimeRepositoryInMemImpl(OSDDateTimeEntity *entity);

private:
    static OSDDateTimeRepositoryInMemImpl *instance;

    OSDDateTimeEntity *_entity;
};

#endif // OSD_DATETIME_REPOSITORY_INMEM_IMPL_H
