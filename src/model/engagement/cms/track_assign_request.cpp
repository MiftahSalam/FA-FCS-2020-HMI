#include "track_assign_request.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

TrackAssignRequest::TrackAssignRequest()
{

}

TrackAssignRequest::TrackAssignRequest(int trackId, const std::string &weapon) : trackId(trackId),
    weapon(weapon)
{}

QByteArray TrackAssignRequest::toJSON()
{
    QJsonObject obj;
    obj["id"] = trackId;
    obj["weapon"] = QString::fromStdString(weapon);

    return QJsonDocument(obj).toJson();
}

int TrackAssignRequest::getTrackId() const
{
    return trackId;
}

void TrackAssignRequest::setTrackId(int newTrackId)
{
    trackId = newTrackId;
}

std::string TrackAssignRequest::getWeapon() const
{
    return weapon;
}

void TrackAssignRequest::setWeapon(const std::string &newWeapon)
{
    weapon = newWeapon;
}
