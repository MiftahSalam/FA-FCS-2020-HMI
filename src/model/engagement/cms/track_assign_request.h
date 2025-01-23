#ifndef TRACKASSIGNREQUEST_H
#define TRACKASSIGNREQUEST_H

#include "src/infra/core/base_request.h"

class TrackAssignRequest: public BaseModel
{
public:
    TrackAssignRequest();
    TrackAssignRequest(int trackId, const std::string &weapon);

    // BaseModel interface
    QByteArray toJSON() override;

    int getTrackId() const;
    void setTrackId(int newTrackId);

    std::string getWeapon() const;
    void setWeapon(const std::string &newWeapon);

private:
    int trackId;
    std::string weapon;
};

#endif // TRACKASSIGNREQUEST_H
