#ifndef GUNREPOSITORYINMEMIMPL_H
#define GUNREPOSITORYINMEMIMPL_H

#include "src/domain/gun/entity/gun_status_command_entity.h"
#include "src/domain/gun/repository/gun_command_repository.h"

class GunCommandRepositoryInMemImpl: public GunCommandRepository
{
public:
    static GunCommandRepository *GetInstance();

    // GunCommandRepository interface
    void SetBarrel(const GunBarrelEntity &barrel) override;
    void SetStatus(const GunStatusCommandEntity &status) override;
    void SetMount(const bool &mount) override;
    void SetSingleShot(const bool &single_shot) override;
    void SetFire(const bool &fire) override;
    void SetProximity(const bool &proximity) override;
    void SetSiren(const bool &siren) override;
    const GunBarrelEntity *GetBarrel() const override;
    const GunStatusCommandEntity *GetStatus() const override;

protected:
    GunCommandRepositoryInMemImpl(GunBarrelEntity *bEntity, GunStatusCommandEntity *sEntity);

private:
    static GunCommandRepositoryInMemImpl *instance;

    GunBarrelEntity *_barrelEntity;
    GunStatusCommandEntity *_statusEntity;
};

#endif // GUNREPOSITORYINMEMIMPL_H
