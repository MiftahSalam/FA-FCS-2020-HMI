#ifndef ENGAGEMENTREPOSITORY_H
#define ENGAGEMENTREPOSITORY_H

#include "src/domain/engagement/repository/engagement_data_repository.h"
#include <QObject>

class EngagementRepository : public QObject
{
    Q_OBJECT
public:
    explicit EngagementRepository(QObject *parent = nullptr);
    EngagementDataRepository *getRepoEngageData( ) const;

private:
    EngagementDataRepository *repoEngageData;
};

#endif // ENGAGEMENTREPOSITORY_H
