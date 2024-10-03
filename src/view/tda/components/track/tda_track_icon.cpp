#include "tda_track_icon.h"

#include <QDebug>

const QString TRACK_INFO_TEMPLATE = "Track Information\n\n"
                                    "TN : %{TN}\n"
                                    "Range : %{RNG} NM\n"
                                    "Bearing : %{BRN} deg\n"
                                    "Speed : %{SPD} kts\n"
                                    "Course : %{CRS} deg\n"
                                    "Height : %{HGT} feet\n"
                                    "Identity : %{IDNT}\n";

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

    QString tp = TRACK_INFO_TEMPLATE;
    tp.replace("%{TN}", QString::number(param->getTn()));
    tp.replace("%{RNG}", QString::number(param->getRange(),'f',2));
    tp.replace("%{BRN}", QString::number(param->getBearing(),'f',2));
    tp.replace("%{SPD}", QString::number(param->getSpeed(),'f',2));
    tp.replace("%{CRS}", QString::number(param->getCourse(),'f',2));
    tp.replace("%{HGT}", QString::number(0,'f',2));
    tp.replace("%{IDNT}", TrackUtils::identity2String(param->getCur_identity()));

    setToolTip(tp);
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
    QString tp = TRACK_INFO_TEMPLATE;
    tp.replace("%{TN}", QString::number(currentParam->getTn()));
    tp.replace("%{RNG}", QString::number(currentParam->getRange(),'f',2));
    tp.replace("%{BRN}", QString::number(currentParam->getBearing(),'f',2));
    tp.replace("%{SPD}", QString::number(currentParam->getSpeed(),'f',2));
    tp.replace("%{CRS}", QString::number(currentParam->getCourse(),'f',2));
    tp.replace("%{HGT}", QString::number(0,'f',2));
    tp.replace("%{IDNT}", TrackUtils::identity2String(currentParam->getCur_identity()));

    setToolTip(tp);
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
