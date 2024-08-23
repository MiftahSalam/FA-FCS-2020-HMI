#ifndef GUN_FEEDBACK_REPOSITORY_INMEM_IMP_H
#define GUN_FEEDBACK_REPOSITORY_INMEM_IMP_H

#include "src/domain/gun/entity/gun_status_feedback_entity.h"
#include "src/domain/gun/repository/gun_feedback_repository.h"

class GunFeedbackRepositoryInMemImp: public GunFeedbackRepository
{
public:
    static GunFeedbackRepository *GetInstance();

    // GunBaseRepository interface
public:
    void SetBarrel(const float &azimuth, const float &elevation) override;
    const GunBarrelEntity *GetBarrel() const override;
    // GunFeedbackRepository interface
public:
    void SetStatus(const GunStatusFeedbackEntity &status) override;
    void SetOpMode(const bool &mode) override;
    void SetRemote(const bool &remote) override;
    void SetMount(const bool &mount) override;
    void SetBarrelTemperature(const bool &bTemp) override;
    void SetGunReadyToStart(const bool &ready) override;
    void SetGunReadyToFire(const bool &ready) override;
    void SetFireMode(const bool &mode) override;
    void SetBlindArc(const bool &blind) override;
    void SetMissAlign(const bool &miss) override;
    void SetGunMegazine(const bool &magazine) override;
    const GunStatusFeedbackEntity *GetStatus() const override;

protected:
    GunFeedbackRepositoryInMemImp(GunBarrelEntity *bEntity, GunStatusFeedbackEntity *sEntity);

private:
    static GunFeedbackRepositoryInMemImp *instance;

    GunBarrelEntity *_barrelEntity;
    GunStatusFeedbackEntity *_statusEntity;

};

#endif // GUN_FEEDBACK_REPOSITORY_INMEM_IMP_H
