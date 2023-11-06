#include "input_mode_model.h"

InputModeModel::InputModeModel(bool position, bool speed, bool inersia)
{
    this->position = position;
    this->speed = speed;
    this->inersia = inersia;
}

InputModeModel InputModeModel::fromJsonObject(QJsonObject obj)
{
    InputModeModel model(
                obj.value("position").toBool(false),
                obj.value("speed").toBool(false),
                obj.value("inersia").toBool(false)
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
