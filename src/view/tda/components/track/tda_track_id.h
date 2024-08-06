#ifndef TDATRACKID_H
#define TDATRACKID_H

#include "src/view/tda/tda_global.h"
#include <QLabel>

class TdaTrackId : public QLabel
{
public:
    explicit TdaTrackId(QWidget *parent = nullptr, trackParam currentParam = {});

signals:

};

#endif // TDATRACKID_H
