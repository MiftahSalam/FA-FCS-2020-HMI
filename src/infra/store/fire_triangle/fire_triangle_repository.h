#ifndef FIRETRIANGLEREPOSITORY_H
#define FIRETRIANGLEREPOSITORY_H

#include "src/domain/fire_triangle/repository/fire_triangle_repository.h"

#include <QObject>

class FireTriangleRepository : public QObject
{
    Q_OBJECT
public:
    explicit FireTriangleRepository(QObject *parent = nullptr);

    FireTriangleBaseRepository* getRepoFT40mm() const;

private:
    FireTriangleBaseRepository *repoFT40mm;
};

#endif // FIRETRIANGLEREPOSITORY_H
