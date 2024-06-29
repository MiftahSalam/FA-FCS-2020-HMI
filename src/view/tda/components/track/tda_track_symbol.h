#ifndef TDATRACKSYMBOL_H
#define TDATRACKSYMBOL_H

#include "src/view/tda/tda_global.h"

#include <QLabel>

class TdaTrackSymbol: public QLabel
{
public:
    TdaTrackSymbol(QWidget *parent, trackParam param);

    //TODO: implementation
    void updateProps(){};

private:
    trackParam currentParam;
    QString currentImagePath;

    //TODO: implementation
    void onSelectedChange(){};

    //TODO: implementation
    void onIdentityChange(){};
};

#endif // TDATRACKSYMBOL_H
