#ifndef ENGAGEMENT_REPOSITORY_H
#define ENGAGEMENT_REPOSITORY_H

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

#endif // ENGAGEMENT_REPOSITORY_H
