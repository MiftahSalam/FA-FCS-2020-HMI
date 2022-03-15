#include "track.h"
#include <QDir>
#include <QDebug>

track::track(QWidget *parent, QSize size) :
    QWidget(parent)
{
    resize(size);
}

void track::buildUI(trackParam param)
{
    trackDat = param;

    current_symbol_image = fileImageLocation(trackDat.cur_identity,trackDat.cur_env); //get image from file
//    qDebug()<<current_symbol_image;
    QImage image(current_symbol_image);

    /*contex menu desig
    desigSubMenu = new QMenu("Desig",this);
    desigSubMenu->setStyleSheet("QMenu{color: rgb(255,255,255);background-color: rgb(0,0,0);selection-color: yellow; item:disabled: black;}");
    for(int i=0;i<DESIG_COUNT;i++)
    {
        desigAction[i] = new QAction(desig2String(int2Desig(i)),this);
        connect(desigAction[i],SIGNAL(triggered()),this,SLOT(desig_change()));
        desigAction[i]->setCheckable(true);

        desigSubMenu->addAction(desigAction[i]);
    }
    cur_checked_desig = 100; //no checked
    */

    /*contex menu desig direct
    desigDirectSubMenu = new QMenu("Direct",this);
    desigDirectSubMenu->setStyleSheet("QMenu{color: rgb(255,255,255);background-color: rgb(0,0,0);selection-color: yellow; item:disabled: black;}");
    for(int i=0;i<DESIG_DIRECT_COUNT;i++)
    {
        desigDirectAction[i] = new QAction(desigDirect2String(int2DesigDirect(i)),this);
        connect(desigDirectAction[i],SIGNAL(triggered()),this,SLOT(desig_direct_change()));
        desigDirectAction[i]->setCheckable(true);

        desigDirectSubMenu->addAction(desigDirectAction[i]);
    }
    desigSubMenu->addMenu(desigDirectSubMenu);
    */

    /*contex menu identity*/
    identitySubMenu = new QMenu("Identity",this);
    identitySubMenu->setStyleSheet("QMenu{color: rgb(255,255,255);background-color: rgb(0,0,0);selection-color: yellow; item:disabled: black;}");
    for(int i=0;i<IDENTITY_COUNT;i++)
    {
        identityAction[i] = new QAction(identity2String(int2Identity(i)),this);
        connect(identityAction[i],SIGNAL(triggered()),this,SLOT(identity_change()));
        identityAction[i]->setCheckable(true);

        identitySubMenu->addAction(identityAction[i]);
    }
    cur_checked_identity = identity2Int(param.cur_identity);
    identityAction[cur_checked_identity]->setChecked(true);

    /*contex menu environment*/
    envSubMenu = new QMenu("Environment",this);
    envSubMenu->setStyleSheet("QMenu{color: rgb(255,255,255);background-color: rgb(0,0,0);selection-color: yellow; item:disabled: black;}");
    for(int i=0;i<ENVIRONMENT_COUNT;i++)
    {
        envAction[i] = new QAction(env2String(int2Environment(i)),this);
        connect(envAction[i],SIGNAL(triggered()),this,SLOT(environment_change()));
        envAction[i]->setCheckable(true);
        envAction[i]->setDisabled(true);

        envSubMenu->addAction(envAction[i]);
    }
    cur_checked_env = environment2Int(param.cur_env);
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
    */
//    rc_radarevent=new RCEventHandler(this);
//    connect(rc_radarevent,SIGNAL(send_rightButtonClicked(QPoint)),this,SLOT(RC_track(QPoint))); //tombol2 klik kanan track
//    symbol->installEventFilter(rc_radarevent);

    /*create QLabel for holding track number and source display*/
    no_track = new QLabel(this);
    no_track->setObjectName(QString::fromUtf8("no_track"));
    no_track->setFrameShape(QFrame::NoFrame);
    no_track->setStyleSheet(QString::fromUtf8("color: rgba(255, 255, 255);background-color: rgb(0,0,0,0);"));
    no_track->setGeometry(QRect(symbol->width()+5,0,width()*2/3,height()));
    no_track->setScaledContents(true);

    QString source;
    if(trackDat.cur_source==T_NAVRAD)
        source = "N ";
    else if(trackDat.cur_source==T_LIOD)
        source = "L ";
    no_track->setText(source+QString::number(trackDat.tn));

}

void track::setSelected(bool select)
{
    if(select)
        symbol->setFrameShape(QFrame::Box);
    else
        symbol->setFrameShape(QFrame::NoFrame);
}

void track::updateData(trackParam param)
{
    /*identity or environment change*/
    if((param.cur_identity!=trackDat.cur_identity) || (param.cur_env!=trackDat.cur_env))
    {
        current_symbol_image = fileImageLocation(param.cur_identity,param.cur_env);
        QImage image(current_symbol_image);
        symbol->setPixmap(QPixmap::fromImage(image));
    }

    /*source change*/
    if(param.cur_source!=trackDat.cur_source)
    {
        QString source;
        if(param.cur_source==T_NAVRAD)
            source = "N ";
        else if(param.cur_source==T_LIOD)
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
void track::RC_track(QPoint pos)
{
    qDebug()<<Q_FUNC_INFO<<" point "<<pos;

    QMenu *menu = new QMenu(this);
    menu->setStyleSheet("QMenu{color: rgb(255,255,255);background-color: rgb(0,0,0);selection-color: yellow; item:disabled: black;}");
    menu->addMenu(identitySubMenu);
    menu->addMenu(envSubMenu);
   // menu->addMenu(desigSubMenu);
    menu->exec(pos);
}

void track::identity_change()
{
    for(int i=0;i<IDENTITY_COUNT;i++)
    {
        if(identityAction[i]->isChecked() && i!=cur_checked_identity)
        {
            identityAction[cur_checked_identity]->setChecked(false);
            cur_checked_identity = i;
        }
    }
    identity_change_signal(trackDat.tn,int2Identity(cur_checked_identity));
}

void track::environment_change()
{
    for(int i=0;i<ENVIRONMENT_COUNT;i++)
    {
        if(envAction[i]->isChecked() || i!=cur_checked_env)
        {
            envAction[cur_checked_env]->setChecked(true);
            cur_checked_env = i;
        }
    }
    env_change_signal(trackDat.tn,int2Environment(cur_checked_env));
}

QString track::identity2String(Identity identity)
{
    if(identity==UNKNOWN)
        return "Unknown";
    else if(identity==FRIENDLY)
        return "Friend";
    else if(identity==NEUTRAL)
        return "Neutral";
    else if(identity==HOSTILE)
        return "Hostile";
    else
        return "Unidentify";
}

QString track::env2String(Environment env)
{
    if(env==AIR)
        return "Air";
    else if(env==SURFACE)
        return "Surface";
    else
        return "Unknown Environment";
}

QString track::fileImageLocation(Identity identity,Environment env)
{
    if(env==AIR)
    {
        if(identity==UNKNOWN)
            return ":/tda_track_symbol/surface-unknown.png";
        else if(identity==FRIENDLY)
            return ":/tda_track_symbol/surface-friend.png";
        else if(identity==NEUTRAL)
            return ":/tda_track_symbol/surface-netral.png";
        else if(identity==HOSTILE)
            return ":/tda_track_symbol/surface-hostile.png";
        else
            return ":/tda_track_symbol/surface-unknown.png";
    }
    else if (env==SURFACE)
    {
        if(identity==UNKNOWN)
            return ":/tda_track_symbol/surface-unknown.png";
        else if(identity==FRIENDLY)
            return ":/tda_track_symbol/surface-friend.png";
        else if(identity==NEUTRAL)
            return ":/tda_track_symbol/surface-netral.png";
        else if(identity==HOSTILE)
            return ":/tda_track_symbol/surface-hostile.png";
        else
            return ":/tda_track_symbol/surface-unknown.png";
    }

}


