#ifndef TDATRACKSYMBOL_H
#define TDATRACKSYMBOL_H

#include "src/view/tda/components/track/track_param.h"
#include <QLabel>

class TdaTrackIcon: public QLabel
{
public:
    TdaTrackIcon(QWidget *parent, TrackParam* param);

    void updateProps(TrackParam param);

private:
    TrackParam* currentParam;
    QString currentIconImagePath;

    void onSelectedChange(){};
    void onIdentityChange(){};
    QString fileImageLocation(TrackUtils::Identity identity, TrackUtils::Environment env);
};

#endif // TDATRACKSYMBOL_H
