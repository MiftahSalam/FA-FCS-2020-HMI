#include "tda_track_id.h"

TdaTrackId::TdaTrackId(QWidget *parent, TrackParam currentParam)
    : QLabel{parent}
{
    setObjectName(QString::fromUtf8("no_track"));
    setFrameShape(QFrame::NoFrame);
    setStyleSheet(QString::fromUtf8("color: rgba(255, 255, 255);background-color: rgb(0,0,0,0);"));
    setScaledContents(true);

    setText("source" + QString::number(currentParam.tn));
}
