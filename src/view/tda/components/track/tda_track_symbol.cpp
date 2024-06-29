#include "tda_track_symbol.h"

TdaTrackSymbol::TdaTrackSymbol(QWidget *parent, trackParam param)
    : QLabel(parent), currentParam(param)
{
    QImage image(currentImagePath);
    setObjectName(QString::fromUtf8("symbol"));
    setFrameShape(QFrame::NoFrame);
    setGeometry(QRect(0,0,width()/3,height()));
    setPixmap(QPixmap::fromImage(image));
    setStyleSheet(QString::fromUtf8("color: rgba(255, 255, 255);background-color: rgb(0,0,0,0);"));
    setScaledContents(true);
}
