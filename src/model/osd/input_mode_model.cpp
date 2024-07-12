#include "input_mode_model.h"

InputModeModel::InputModeModel(bool position, bool speed, bool inersia, bool waterspeed,
                               bool wind)
{
    this->position = position;
    this->speed = speed;
    this->inersia = inersia;
    this->waterspeed = waterspeed;
    this->wind = wind;
}

InputModeModel InputModeModel::fromJsonObject(QJsonObject obj)
{
    InputModeModel model(
                obj.value("position").toBool(false),
                obj.value("speed").toBool(false),
                obj.value("inertia").toBool(false),
                obj.value("water_speed").toBool(false),
                obj.value("wind").toBool(false)
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

bool InputModeModel::getWind() const
{
    return wind;
}

