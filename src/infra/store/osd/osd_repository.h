#ifndef OSDREPOSITORY_H
#define OSDREPOSITORY_H

#include "src/domain/osd/repository/osd_inertia_repository.h"
#include "src/domain/osd/repository/osd_position_repository.h"
#include <QObject>

class OSDRepository : public QObject
{
    Q_OBJECT
public:
    explicit OSDRepository(QObject *parent = nullptr);

    OSDPositionRepository *getRepoOSDPosition() const;

    OSDInertiaRepository *getRepoOSDInertia() const;

private:
    OSDPositionRepository *repoOSDPosition;
    OSDInertiaRepository *repoOSDInertia;
};

#endif // OSDREPOSITORY_H
