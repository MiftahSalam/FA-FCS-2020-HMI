#ifndef OSD_DATETIME_REPOSITORY_INMEM_IMPL_H
#define OSD_DATETIME_REPOSITORY_INMEM_IMPL_H

#include "src/domain/osd/entity/osd_datetime_entity.h"
#include "src/domain/osd/repository/osd_datetime_repository.h"

class DateTimeRepositoryInMemImpl: public DateTimeRepository
{
public:
    static DateTimeRepository *GetInstance();

public:
    void SetSource(const std::string &source) override;
    void SetStatus(const std::string &status) override;
    void SetMode(const OSD_MODE &mode) override;

public:
    void SetDateTime(const DateTimeEntity &date_time) override;
    const DateTimeEntity *GetDateTime() const override;

protected:
    DateTimeRepositoryInMemImpl(DateTimeEntity *entity);

private:
    static DateTimeRepositoryInMemImpl *instance;

    DateTimeEntity *_entity;
};

#endif // OSD_DATETIME_REPOSITORY_INMEM_IMPL_H
