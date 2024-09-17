#include "tda_track_icon.h"

#include <QDebug>

const QString TRACK_INFO_TEMPLATE = "Track Information\n\n"
                                    "TN : %1\n"
                                    "Range : %2 NM\n"
                                    "Bearing : %3 deg\n"
                                    "Speed : %4 kts\n"
                                    "Course : %5 deg\n"
                                    "Height : %6 feet\n"
                                    "Identity : %7\n";

TdaTrackIcon::TdaTrackIcon(QWidget *parent, TrackParam* param)
    : QLabel(parent), currentParam(param)
{
    currentIconImagePath = fileImageLocation(currentParam->getCur_identity(), currentParam->getCur_env()); //get image from file
    qDebug()<<Q_FUNC_INFO<<currentIconImagePath;
    QImage image(currentIconImagePath);

    setObjectName(QString::fromUtf8("trackIcon"));
    setFrameShape(QFrame::NoFrame);
    setGeometry(QRect(0,0,TRACK_ICON_SIZE.width(),TRACK_ICON_SIZE.height()));
    setPixmap(QPixmap::fromImage(image));
    setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);background-color: rgba(0,0,0,0);"));
    setScaledContents(true);
    setToolTip(QString(TRACK_INFO_TEMPLATE)
                       .arg(QString::number(param->getTn()))
                       .arg(QString::number(param->getRange(),'f',2))
                       .arg(QString::number(param->getBearing(),'f',2))
                       .arg(QString::number(param->getSpeed(),'f',2))
                       .arg(QString::number(param->getCourse(),'f',2))
                       .arg(QString::number(0,'f',2))
                       .arg(TrackUtils::identity2String(param->getCur_identity()))
                       );
}

void TdaTrackIcon::updateProps(TrackParam param)
{
    /*identity or environment change*/
    if((param.getCur_identity() != currentParam->getCur_identity()) || (param.getCur_env() != currentParam->getCur_env()))
    {
        currentIconImagePath = fileImageLocation(param.getCur_identity(),param.getCur_env());
        QImage image(currentIconImagePath);
        setPixmap(QPixmap::fromImage(image));
    }

    *currentParam = param;

    /*update track information*/
    setToolTip(TRACK_INFO_TEMPLATE
               .arg(QString::number(currentParam->getTn()))
               .arg(QString::number(currentParam->getRange(),'f',2))
               .arg(QString::number(currentParam->getBearing(),'f',2))
               .arg(QString::number(currentParam->getSpeed(),'f',2))
               .arg(QString::number(currentParam->getCourse(),'f',2))
               .arg(QString::number(0,'f',2))
               .arg(TrackUtils::identity2String(currentParam->getCur_identity()))
               .arg(currentParam->getWeapon_assign())
               );
}

QString TdaTrackIcon::fileImageLocation(TrackUtils::Identity identity,TrackUtils::Environment env)
{
    if(env==TrackUtils::AIR)
    {
        if(identity==TrackUtils::UNKNOWN)
            return ":/images/tda_track_symbol/surface-unknown.png";
        else if(identity==TrackUtils::FRIENDLY)
            return ":/images/tda_track_symbol/surface-friend.png";
        else if(identity==TrackUtils::NEUTRAL)
            return ":/images/tda_track_symbol/surface-netral.png";
        else if(identity==TrackUtils::HOSTILE)
            return ":/images/tda_track_symbol/surface-hostile.png";
        else
            return ":/images/tda_track_symbol/surface-unknown.png";
    }
    else if (env==TrackUtils::SURFACE)
    {
        if(identity==TrackUtils::UNKNOWN)
            return ":/images/tda_track_symbol/surface-unknown.png";
        else if(identity==TrackUtils::FRIENDLY)
            return ":/images/tda_track_symbol/surface-friend.png";
        else if(identity==TrackUtils::NEUTRAL)
            return ":/images/tda_track_symbol/surface-netral.png";
        else if(identity==TrackUtils::HOSTILE)
            return ":/images/tda_track_symbol/surface-hostile.png";
        else
            return ":/images/tda_track_symbol/surface-unknown.png";
    }
}
