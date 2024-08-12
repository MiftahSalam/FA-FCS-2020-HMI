#ifndef TRACK_PARAM_H
#define TRACK_PARAM_H

#include "src/shared/utils/track/track_utils.h"
#include <QString>

class TrackParam
{public:
    TrackParam(
            int tn,
            double range,
            double bearing,
            double height,
            double speed,
            double course,
            TrackUtils::Identity cur_identity,
            TrackUtils::trackSource cur_source,
            TrackUtils::Environment cur_env,
            const QString &weapon_assign
            );
    TrackParam(const TrackParam &track);

    void operator=(const TrackParam &track);

    int getTn() const;
    double getRange() const;
    double getBearing() const;
    double getHeight() const;
    double getSpeed() const;
    double getCourse() const;
    TrackUtils::Identity getCur_identity() const;
    TrackUtils::trackSource getCur_source() const;
    TrackUtils::Environment getCur_env() const;
    QString getWeapon_assign() const;

private:
    int tn;
    double range;
    double bearing;
    double height;
    double speed;
    double course;

    TrackUtils::Identity cur_identity;
    TrackUtils::trackSource cur_source;
    TrackUtils::Environment cur_env;
    QString weapon_assign;
};

#endif // TRACK_PARAM_H
