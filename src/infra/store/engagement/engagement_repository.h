#ifndef ENGAGEMENTREPOSITORY_H
#define ENGAGEMENTREPOSITORY_H

#include <QObject>

class EngagementRepository : public QObject
{
    Q_OBJECT
public:
    explicit EngagementRepository(QObject *parent = nullptr);

};

#endif // ENGAGEMENTREPOSITORY_H
