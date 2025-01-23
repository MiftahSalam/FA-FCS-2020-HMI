#include "osd_input_mode_repository_inmem_impl.h"

OSDInputModeRepositoryInMemImpl* OSDInputModeRepositoryInMemImpl::instance{nullptr};

OSDInputModeRepositoryInMemImpl::OSDInputModeRepositoryInMemImpl(OSDInputModeEntity *entity)
    : _entity(entity)
{

}

void OSDInputModeRepositoryInMemImpl::SetMode(const OSDInputModeEntity &mode)
{
    _entity->setInersiaMode(mode.inersia());
    _entity->setPositionMode(mode.position());
    _entity->setSpeedMode(mode.speed());
    _entity->setWaterSpeedMode(mode.waterSpeed());
    _entity->setWeatherMode(mode.weather());
    _entity->setWindMode(mode.wind());
}

const OSDInputModeEntity *OSDInputModeRepositoryInMemImpl::GetMode() const
{
    return _entity;
}

OSDInputModeRepository *OSDInputModeRepositoryInMemImpl::GetInstance()
{
    if (instance == nullptr) {
        OSDInputModeEntity* entity = new OSDInputModeEntity(false, false, false, false, false, false);
        instance = new OSDInputModeRepositoryInMemImpl(entity);
    }

    return instance;
}
