#include "osd_set_gyro_request.h"
#include "qjsondocument.h"
#include "qjsonobject.h"


OSDSetGyroRequest::OSDSetGyroRequest(double _heading, double _pitch, double _roll):
    heading(_heading), pitch(_pitch), roll(_roll)
{

}

double OSDSetGyroRequest::getHeading() const
{
    return heading;
}

double OSDSetGyroRequest::getPitch() const
{
    return pitch;
}

double OSDSetGyroRequest::getRoll() const
{
    return roll;
}

void OSDSetGyroRequest::setHeading(const double _hdg)
{
    heading = _hdg;
}

void OSDSetGyroRequest::setPitch(const double _pitch)
{
    pitch = _pitch;
}

void OSDSetGyroRequest::setRoll(const double _roll)
{
    roll = _roll;
}

QByteArray OSDSetGyroRequest::toJSON()
{
    QJsonObject obj;
    obj["heading"] = heading;
    obj["pitch"] = pitch;
    obj["roll"] = roll;

    return QJsonDocument(obj).toJson();

}
