#ifndef TRACK_PARAM_H
#define TRACK_PARAM_H

#include "src/shared/utils/track/track_utils.h"
#include <QString>

struct TrackParam
{
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
