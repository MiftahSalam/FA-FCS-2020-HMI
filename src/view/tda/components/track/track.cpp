#include "track.h"
#include "qevent.h"
#include <QDir>
#include <QDebug>

Track::Track(QWidget *parent, QSize size) :
    QWidget(parent)
{
    resize(size);
}

void Track::buildUI(TrackParam param)
{
    trackDat = param;

    current_symbol_image_path = fileImageLocation(trackDat.cur_identity,trackDat.cur_env); //get image from file
    qDebug()<<current_symbol_image_path;
    QImage image(current_symbol_image_path);

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
    cur_checked_identity = TrackUtils::identity2Int(param.cur_identity);
    identityAction[cur_checked_identity]->setChecked(true);

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
    cur_checked_env = TrackUtils::environment2Int(param.cur_env);
    envAction[cur_checked_env]->setChecked(true);

    /*create QLabel for holding track image and information display*/
    symbol = new QLabel(this);
    symbol->setObjectName(QString::fromUtf8("symbol"));
    symbol->setFrameShape(QFrame::NoFrame);
    symbol->setGeometry(QRect(0,0,width()/3,height()));
    symbol->setPixmap(QPixmap::fromImage(image));
    symbol->setStyleSheet(QString::fromUtf8("color: rgba(255, 255, 255);background-color: rgb(0,0,0,0);"));
    symbol->setScaledContents(true);
    symbol->setToolTip(QString("Track Information\n\n"
                               "TN : %1\n"
                               "Range : %2 NM\n"
                               "Bearing : %3 deg\n"
                               "Speed : %4 kts\n"
                               "Course : %5 deg\n"
                               "Height : %6 feet\n"
                               "Identity : %7\n"
                               )
                       .arg(QString::number(trackDat.tn))
                       .arg(QString::number(trackDat.range,'f',2))
                       .arg(QString::number(trackDat.bearing,'f',2))
                       .arg(QString::number(trackDat.speed,'f',2))
                       .arg(QString::number(trackDat.course,'f',2))
                       .arg(QString::number(0,'f',2))
                       .arg(identity2String(trackDat.cur_identity))
                       );
    /*
      track event handler
    rc_radarevent=new TdaEventFilter();
    connect(rc_radarevent,SIGNAL(send_rightButtonClicked(QPoint)),this,SLOT(RC_track(QPoint))); //tombol2 klik kanan track
    symbol->installEventFilter(rc_radarevent);
    */
\
//    qDebug() <<Q_FUNC_INFO <<"filter" <<symbol;

    /*create QLabel for holding track number and source display*/
    no_track = new QLabel(this);
    no_track->setObjectName(QString::fromUtf8("no_track"));
    no_track->setFrameShape(QFrame::NoFrame);
    no_track->setStyleSheet(QString::fromUtf8("color: rgba(255, 255, 255);background-color: rgb(0,0,0,0);"));
    no_track->setGeometry(QRect(symbol->width()+5,0,width()*2/3,height()));
    no_track->setScaledContents(true);
//    no_track->installEventFilter(rc_radarevent);

    QString source;
    if(trackDat.cur_source==TrackUtils::T_NAVRAD)
        source = "N ";
    else if(trackDat.cur_source==TrackUtils::T_LIOD)
        source = "L ";
    no_track->setText(source+QString::number(trackDat.tn));
}

void Track::setSelected(bool select)
{
    if(select)
        symbol->setFrameShape(QFrame::Box);
    else
        symbol->setFrameShape(QFrame::NoFrame);
}

void Track::updateData(TrackParam param)
{
    /*identity or environment change*/
    if((param.cur_identity!=trackDat.cur_identity) || (param.cur_env!=trackDat.cur_env))
    {
        current_symbol_image_path = fileImageLocation(param.cur_identity,param.cur_env);
        QImage image(current_symbol_image_path);
        symbol->setPixmap(QPixmap::fromImage(image));
    }

    /*source change*/
    if(param.cur_source!=trackDat.cur_source)
    {
        QString source;
        if(param.cur_source==TrackUtils::T_NAVRAD)
            source = "N ";
        else if(param.cur_source==TrackUtils::T_LIOD)
            source = "L ";
        no_track->setText(source+QString::number(trackDat.tn));
    }


    trackDat = param;

    /*update track information*/
    symbol->setToolTip(QString("Track Information\n\n"
                               "TN : %1\n"
                               "Range : %2 NM\n"
                               "Bearing : %3 deg\n"
                               "Speed : %4 kts\n"
                               "Course : %5 deg\n"
                               "Height : %6 feet\n"
                               "Identity : %7\n"
                               "Weapon Assign : %8\n"
                               )
                       .arg(QString::number(trackDat.tn))
                       .arg(QString::number(trackDat.range,'f',2))
                       .arg(QString::number(trackDat.bearing,'f',2))
                       .arg(QString::number(trackDat.speed,'f',2))
                       .arg(QString::number(trackDat.course,'f',2))
                       .arg(QString::number(0,'f',2))
                       .arg(identity2String(trackDat.cur_identity))
                       .arg(trackDat.weapon_assign)
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

void Track::identity_change()
{
    for(int i=0;i<TrackUtils::IDENTITY_COUNT;i++)
    {
        if(identityAction[i]->isChecked() && i!=cur_checked_identity)
        {
            identityAction[cur_checked_identity]->setChecked(false);
            cur_checked_identity = i;
        }
    }
    identity_change_signal(trackDat.tn,TrackUtils::int2Identity(cur_checked_identity));
}

void Track::environment_change()
{
    for(int i=0;i<TrackUtils::ENVIRONMENT_COUNT;i++)
    {
        if(envAction[i]->isChecked() || i!=cur_checked_env)
        {
            envAction[cur_checked_env]->setChecked(true);
            cur_checked_env = i;
        }
    }
    env_change_signal(trackDat.tn,TrackUtils::int2Environment(cur_checked_env));
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


