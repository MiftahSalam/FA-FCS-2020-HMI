#ifndef TDATRACKID_H
#define TDATRACKID_H

#include "src/shared/utils/track/track_utils.h"
#include <QLabel>

struct trackParam
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

class TdaTrackId : public QLabel
{
public:
    explicit TdaTrackId(QWidget *parent = nullptr, trackParam currentParam = {});

signals:

};

#endif // TDATRACKID_H
