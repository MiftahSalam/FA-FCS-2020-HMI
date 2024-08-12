#ifndef TDATRACKSYMBOL_H
#define TDATRACKSYMBOL_H

#include "src/view/tda/components/track/track_param.h"
#include <QLabel>

class TdaTrackSymbol: public QLabel
{
public:
    TdaTrackSymbol(QWidget *parent, TrackParam param);

    //TODO: implementation
    void updateProps(){};

private:
    TrackParam currentParam;
    QString currentImagePath;

    //TODO: implementation
    void onSelectedChange(){};

    //TODO: implementation
    void onIdentityChange(){};
};

#endif // TDATRACKSYMBOL_H
