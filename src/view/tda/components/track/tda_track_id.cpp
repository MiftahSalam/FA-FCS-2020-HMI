#include "tda_track_id.h"

TdaTrackId::TdaTrackId(QWidget *parent, int tn, TrackUtils::trackSource source)
    : QLabel{parent}, currentTn(tn), currentSource(source)
{
    setObjectName(QString::fromUtf8("trackId"));
    setFrameShape(QFrame::NoFrame);
    setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);background-color: rgba(0,0,0,0);"));
    setScaledContents(true);

    QString sourceStr;
    if(currentSource == TrackUtils::T_NAVRAD)
        sourceStr = "N ";
    else if(currentSource == TrackUtils::T_LIOD)
        sourceStr = "L ";
    setText(sourceStr+QString::number(currentTn));
}

void TdaTrackId::changeSource(TrackUtils::trackSource source)
{
    if(source != currentSource)
    {
        QString sourceStr;
        if(source == TrackUtils::T_NAVRAD)
            sourceStr = "N ";
        else if(source == TrackUtils::T_LIOD)
            sourceStr = "L ";
        setText(sourceStr+QString::number(currentTn));

        currentSource = source;
    }
}
