#ifndef TDATRACKID_H
#define TDATRACKID_H

#include <QLabel>

#include "src/shared/utils/track/track_utils.h"

class TdaTrackId : public QLabel
{
public:
    explicit TdaTrackId(QWidget *parent = nullptr, int tn = 0, TrackUtils::trackSource source = TrackUtils::T_NAVRAD);

    void changeSource(TrackUtils::trackSource source);

private:
    int currentTn;
    TrackUtils::trackSource currentSource;
};

#endif // TDATRACKID_H
