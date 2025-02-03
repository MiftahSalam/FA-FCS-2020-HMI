#include "input_mode_cms_response_model.h"

InputModeResponseModel::InputModeResponseModel(bool position, bool speed, bool inersia, bool waterspeed,
                               bool wind, bool weather)
{
    this->position = position;
    this->speed = speed;
    this->inersia = inersia;
    this->waterspeed = waterspeed;
    this->wind = wind;
    this->weather = weather;
}

InputModeResponseModel InputModeResponseModel::fromJsonObject(QJsonObject obj)
{
    InputModeResponseModel model(
        obj.value("position").toBool(false),
        obj.value("speed").toBool(false),
        obj.value("inertia").toBool(false),
        obj.value("water_speed").toBool(false),
        obj.value("wind").toBool(false),
        obj.value("weather").toBool(false)
        );

    return model;
}
bool InputModeResponseModel::getPosition() const
{
    return position;
}

bool InputModeResponseModel::getSpeed() const
{
    return speed;
}

bool InputModeResponseModel::getInersia() const
{
    return inersia;
}

bool InputModeResponseModel::getWaterSpeed() const
{
    return waterspeed;
}

bool InputModeResponseModel::getWind() const
{
    return wind;
}

bool InputModeResponseModel::getWeather() const
{
    return weather;
}
