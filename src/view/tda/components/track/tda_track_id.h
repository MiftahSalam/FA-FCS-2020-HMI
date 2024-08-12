#ifndef TDATRACKID_H
#define TDATRACKID_H

#include "src/view/tda/components/track/track_param.h"
#include <QLabel>

class TdaTrackId : public QLabel
{
public:
    explicit TdaTrackId(QWidget *parent = nullptr, TrackParam currentParam = {});

signals:

};

#endif // TDATRACKID_H
