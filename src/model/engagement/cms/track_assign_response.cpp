#include "track_assign_response.h"

TrackAssignResponse::TrackAssignResponse()
{

}

TrackAssignResponse::TrackAssignResponse(int trackId, const std::string &weapon) : trackId(trackId),
    weapon(weapon)
{}

int TrackAssignResponse::getTrackId() const
{
    return trackId;
}

std::string TrackAssignResponse::getWeapon() const
{
    return weapon;
}

TrackAssignResponse TrackAssignResponse::FromJsonObject(QJsonObject obj)
{
    TrackAssignResponse model(
                obj.value("id").toInt(),
                obj.value("weapon").toString().toStdString()
                );

    return model;
}
