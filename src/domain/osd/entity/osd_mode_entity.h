#ifndef OSD_MODE_ENTITY_H
#define OSD_MODE_ENTITY_H

class OSDInputModeEntity
{
public:
    OSDInputModeEntity(bool position, bool speed, bool inersia, bool waterspeed, bool wind, bool weather);

    bool position() const;
    void setPositionMode(bool newPosition);
    bool speed() const;
    void setSpeedMode(bool newSpeed);
    bool inersia() const;
    void setInersiaMode(bool newInersia);
    bool waterSpeed() const;
    void setWaterSpeedMode(bool newWaterSpeed);
    bool wind() const;
    void setWindMode(bool newWind);
    bool weather() const;
    void setWeatherMode(bool newWeather);

private:
    bool _position;
    bool _speed;
    bool _inersia;
    bool _waterspeed;
    bool _wind;
    bool _weather;
};

#endif // OSD_MODE_ENTITY_H
