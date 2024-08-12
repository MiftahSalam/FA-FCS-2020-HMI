#include "track.h"
#include "qevent.h"
#include <QDir>
#include <QDebug>

Track::Track(QWidget *parent, QSize size) :
    QWidget(parent), trackData(nullptr)
{
    resize(size);
}

void Track::buildUI(TrackParam *param)
{
    trackData = param;

    currentIconImagePath = fileImageLocation(trackData->getCur_identity(), trackData->getCur_env()); //get image from file
    qDebug()<<Q_FUNC_INFO<<currentIconImagePath;
    QImage image(currentIconImagePath);

    /*contex menu identity*/
    identitySubMenu = new QMenu("Identity",this);
    identitySubMenu->setStyleSheet("QMenu{color: rgb(255,255,255);background-color: rgb(0,0,0);selection-color: yellow; item:disabled: black;}");
    for(int i=0;i<TrackUtils::IDENTITY_COUNT;i++)
    {
        identityAction[i] = new QAction(identity2String(TrackUtils::int2Identity(i)),this);
        connect(identityAction[i],SIGNAL(triggered()),this,SLOT(identity_change()));
        identityAction[i]->setCheckable(true);

        identitySubMenu->addAction(identityAction[i]);
    }
    curCheckedIdentity = TrackUtils::identity2Int(param->getCur_identity());
    identityAction[curCheckedIdentity]->setChecked(true);

    /*contex menu environment*/
    envSubMenu = new QMenu("Environment",this);
    envSubMenu->setStyleSheet("QMenu{color: rgb(255,255,255);background-color: rgb(0,0,0);selection-color: yellow; item:disabled: black;}");
    for(int i=0;i<TrackUtils::ENVIRONMENT_COUNT;i++)
    {
        envAction[i] = new QAction(env2String(TrackUtils::int2Environment(i)),this);
        connect(envAction[i],SIGNAL(triggered()),this,SLOT(environment_change()));
        envAction[i]->setCheckable(true);
        envAction[i]->setDisabled(true);

        envSubMenu->addAction(envAction[i]);
    }
    curCheckedEnv = TrackUtils::environment2Int(param->getCur_env());
    envAction[curCheckedEnv]->setChecked(true);

    /*create QLabel for holding track image and information display*/
    trackIconLabel = new QLabel(this);
    trackIconLabel->setObjectName(QString::fromUtf8("trackIconLabel"));
    trackIconLabel->setFrameShape(QFrame::NoFrame);
    trackIconLabel->setGeometry(QRect(0,0,width()/3,height()));
    trackIconLabel->setPixmap(QPixmap::fromImage(image));
    trackIconLabel->setStyleSheet(QString::fromUtf8("color: rgba(255, 255, 255);background-color: rgb(0,0,0,0);"));
    trackIconLabel->setScaledContents(true);
    trackIconLabel->setToolTip(QString("Track Information\n\n"
                               "TN : %1\n"
                               "Range : %2 NM\n"
                               "Bearing : %3 deg\n"
                               "Speed : %4 kts\n"
                               "Course : %5 deg\n"
                               "Height : %6 feet\n"
                               "Identity : %7\n"
                               )
                       .arg(QString::number(trackData->getTn()))
                       .arg(QString::number(trackData->getRange(),'f',2))
                       .arg(QString::number(trackData->getBearing(),'f',2))
                       .arg(QString::number(trackData->getSpeed(),'f',2))
                       .arg(QString::number(trackData->getCourse(),'f',2))
                       .arg(QString::number(0,'f',2))
                       .arg(identity2String(trackData->getCur_identity()))
                       );
    /*
      track event handler
    rc_radarevent=new TdaEventFilter();
    connect(rc_radarevent,SIGNAL(send_rightButtonClicked(QPoint)),this,SLOT(RC_track(QPoint))); //tombol2 klik kanan track
    trackIconLabel->installEventFilter(rc_radarevent);
    */
\
//    qDebug() <<Q_FUNC_INFO <<"filter" <<trackIconLabel;

    /*create QLabel for holding track number and source display*/
    trackIdLabel = new QLabel(this);
    trackIdLabel->setObjectName(QString::fromUtf8("trackIdLabel"));
    trackIdLabel->setFrameShape(QFrame::NoFrame);
    trackIdLabel->setStyleSheet(QString::fromUtf8("color: rgba(255, 255, 255);background-color: rgb(0,0,0,0);"));
    trackIdLabel->setGeometry(QRect(trackIconLabel->width()+5,0,width()*2/3,height()));
    trackIdLabel->setScaledContents(true);
//    trackIdLabel->installEventFilter(rc_radarevent);

    QString source;
    if(trackData->getCur_source()==TrackUtils::T_NAVRAD)
        source = "N ";
    else if(trackData->getCur_source()==TrackUtils::T_LIOD)
        source = "L ";
    trackIdLabel->setText(source+QString::number(trackData->getTn()));
}

void Track::setSelected(bool select)
{
    if(select)
        trackIconLabel->setFrameShape(QFrame::Box);
    else
        trackIconLabel->setFrameShape(QFrame::NoFrame);
}

void Track::updateTrackData(TrackParam param)
{
    /*identity or environment change*/
    if((param.getCur_identity()!=trackData->getCur_identity()) || (param.getCur_env()!=trackData->getCur_env()))
    {
        currentIconImagePath = fileImageLocation(param.getCur_identity(),param.getCur_env());
        QImage image(currentIconImagePath);
        trackIconLabel->setPixmap(QPixmap::fromImage(image));
    }

    /*source change*/
    if(param.getCur_source()!=trackData->getCur_source())
    {
        QString source;
        if(param.getCur_source()==TrackUtils::T_NAVRAD)
            source = "N ";
        else if(param.getCur_source()==TrackUtils::T_LIOD)
            source = "L ";
        trackIdLabel->setText(source+QString::number(trackData->getTn()));
    }


    *trackData = param;

    /*update track information*/
    trackIconLabel->setToolTip(QString("Track Information\n\n"
                               "TN : %1\n"
                               "Range : %2 NM\n"
                               "Bearing : %3 deg\n"
                               "Speed : %4 kts\n"
                               "Course : %5 deg\n"
                               "Height : %6 feet\n"
                               "Identity : %7\n"
                               "Weapon Assign : %8\n"
                               )
                       .arg(QString::number(trackData->getTn()))
                       .arg(QString::number(trackData->getRange(),'f',2))
                       .arg(QString::number(trackData->getBearing(),'f',2))
                       .arg(QString::number(trackData->getSpeed(),'f',2))
                       .arg(QString::number(trackData->getCourse(),'f',2))
                       .arg(QString::number(0,'f',2))
                       .arg(identity2String(trackData->getCur_identity()))
                       .arg(trackData->getWeapon_assign())
                       );

}
/*right click menu*/
void Track::RC_track(QPoint pos)
{
    qDebug()<<Q_FUNC_INFO<<" point "<<pos;

    QMenu *menu = new QMenu(this);
    menu->setStyleSheet("QMenu{color: rgb(255,255,255);background-color: rgb(0,0,0);selection-color: yellow; item:disabled: black;}");
    menu->addMenu(identitySubMenu);
    menu->addMenu(envSubMenu);
   // menu->addMenu(desigSubMenu);
    menu->exec(pos);
}

void Track::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<Q_FUNC_INFO<<" point "<<event->globalPos();

    QMenu *menu = new QMenu(this);
    menu->setStyleSheet("QMenu{color: rgb(255,255,255);background-color: rgb(0,0,0);selection-color: yellow; item:disabled: black;}");
    menu->addMenu(identitySubMenu);
    menu->addMenu(envSubMenu);
   // menu->addMenu(desigSubMenu);
    menu->exec(event->globalPos());
}

void Track::identityChange()
{
    for(int i=0;i<TrackUtils::IDENTITY_COUNT;i++)
    {
        if(identityAction[i]->isChecked() && i!=curCheckedIdentity)
        {
            identityAction[curCheckedIdentity]->setChecked(false);
            curCheckedIdentity = i;
        }
    }
    identityChange_Signal(trackData->getTn(),TrackUtils::int2Identity(curCheckedIdentity));
}

void Track::environmentChange()
{
    for(int i=0;i<TrackUtils::ENVIRONMENT_COUNT;i++)
    {
        if(envAction[i]->isChecked() || i!=curCheckedEnv)
        {
            envAction[curCheckedEnv]->setChecked(true);
            curCheckedEnv = i;
        }
    }
    envChangeSignal(trackData->getTn(),TrackUtils::int2Environment(curCheckedEnv));
}

const TrackParam *Track::getTrackData() const
{
    return trackData;
}

QString Track::identity2String(TrackUtils::Identity identity)
{
    if(identity==TrackUtils::UNKNOWN)
        return "Unknown";
    else if(identity==TrackUtils::FRIENDLY)
        return "Friend";
    else if(identity==TrackUtils::NEUTRAL)
        return "Neutral";
    else if(identity==TrackUtils::HOSTILE)
        return "Hostile";
    else
        return "Unidentify";
}

QString Track::env2String(TrackUtils::Environment env)
{
    if(env==TrackUtils::AIR)
        return "Air";
    else if(env==TrackUtils::SURFACE)
        return "Surface";
    else
        return "Unknown Environment";
}

QString Track::fileImageLocation(TrackUtils::Identity identity,TrackUtils::Environment env)
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


