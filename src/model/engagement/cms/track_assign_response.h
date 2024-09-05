#ifndef TRACKASSIGNRESPONSE_H
#define TRACKASSIGNRESPONSE_H

#include "qjsonobject.h"

class TrackAssignResponse
{
public:
    TrackAssignResponse();
    TrackAssignResponse(int trackId, const std::string &weapon);

    static TrackAssignResponse FromJsonObject(QJsonObject obj);

    int getTrackId() const;

    std::string getWeapon() const;

private:
    int trackId;
    std::string weapon;
};

#endif // TRACKASSIGNRESPONSE_H
