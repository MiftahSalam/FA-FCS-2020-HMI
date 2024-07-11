#include "input_mode_model.h"

InputModeModel::InputModeModel(bool position, bool speed, bool inersia, bool waterspeed)
{
    this->position = position;
    this->speed = speed;
    this->inersia = inersia;
    this->waterspeed = waterspeed;
}

InputModeModel InputModeModel::fromJsonObject(QJsonObject obj)
{
    InputModeModel model(
                obj.value("position").toBool(false),
                obj.value("speed").toBool(false),
                obj.value("inertia").toBool(false),
                obj.value("waterspeed").toBool(false)
                );

    return model;
}
bool InputModeModel::getPosition() const
{
    return position;
}

bool InputModeModel::getSpeed() const
{
    return speed;
}

bool InputModeModel::getInersia() const
{
    return inersia;
}

bool InputModeModel::getWaterSpeed() const
{
    return waterspeed;
}
