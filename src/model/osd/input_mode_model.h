#ifndef INPUTMODEMODEL_H
#define INPUTMODEMODEL_H


#include "qjsonobject.h"

class InputModeModel
{
public:
    InputModeModel(bool position, bool speed, bool inersia, bool waterspeed);
    InputModeModel(){};

    static InputModeModel fromJsonObject(QJsonObject obj);

    bool getPosition() const;
    bool getSpeed() const;
    bool getInersia() const;
    bool getWaterSpeed() const;

private:
    bool position;
    bool speed;
    bool inersia;
    bool waterspeed;

};

#endif // INPUTMODEMODEL_H
