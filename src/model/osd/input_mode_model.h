#ifndef INPUTMODEMODEL_H
#define INPUTMODEMODEL_H


#include "qjsonobject.h"

class InputModeModel
{
public:
    InputModeModel(bool position, bool speed, bool inersia);
    InputModeModel(){};

    static InputModeModel fromJsonObject(QJsonObject obj);

    bool getPosition() const;
    bool getSpeed() const;
    bool getInersia() const;

private:
    bool position;
    bool speed;
    bool inersia;

};

#endif // INPUTMODEMODEL_H
