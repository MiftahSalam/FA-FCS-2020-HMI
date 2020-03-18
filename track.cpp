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
    qDebug()<<current_symbol_image;
    QImage image(current_symbol_image);

    /*contex menu desig*/
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

    /*contex menu desig direct*/
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
    rc_radarevent=new RCEventHandler(this);
    connect(rc_radarevent,SIGNAL(send_rightButtonClicked(QPoint)),this,SLOT(RC_track(QPoint))); //tombol2 klik kanan track
    symbol->installEventFilter(rc_radarevent);

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

    /*weapon assign change
//    qDebug()<<Q_FUNC_INFO<<param.weapon_assign<<trackDat.weapon_assign;

    if(param.weapon_assign!=trackDat.weapon_assign)
    {
        for(int i=0;i<DESIG_DIRECT_COUNT;i++)
        {
            if(desigDirectAction[i]->text()==param.weapon_assign)
                desigDirectAction[i]->setEnabled(true);
            else
                desigDirectAction[i]->setEnabled(false);
            qDebug()<<Q_FUNC_INFO<<desigDirectAction[i]->isEnabled();

        }
    }
*/

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
    menu->addMenu(desigSubMenu);
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
        if(envAction[i]->isChecked() && i!=cur_checked_env)
        {
            envAction[cur_checked_env]->setChecked(false);
            cur_checked_env = i;
        }
    }
    env_change_signal(trackDat.tn,int2Environment(cur_checked_env));
}

void track::desig_change()
{
    QAction *signalSender = dynamic_cast<QAction*>(sender());
    qDebug()<< signalSender->text();

    for(int i=0;i<DESIG_COUNT;i++)
    {
        if(desigAction[i]->isChecked() && i!=cur_checked_desig)
        {
            if(cur_checked_desig < DESIG_COUNT)
                desigAction[cur_checked_desig]->setChecked(false);

            cur_checked_desig = i;
        }
    }

    desig_request(signalSender->text());

}
void track::desig_direct_change()
{
    QAction *signalSender = dynamic_cast<QAction*>(sender());
    QString desig_str;
    qDebug()<< signalSender->text()<<signalSender->isChecked();

    if(signalSender->isChecked())
        desig_str = QString("Direct,add,%2").arg(signalSender->text());
    else
        desig_str = QString("Direct,rm,%2").arg(signalSender->text());

    desig_request(desig_str);

}
void track::desig_request(QString desig_mode)
{
    qDebug()<<desig_mode;

    bool checked_status;

    if(desig_mode == "EOT")
    {
        QAction *liodAction;

        for(int i=0;i<DESIG_COUNT;i++)
        {
            if(desigAction[i]->text()=="EOT")
            {
                liodAction = desigAction[i];
                break;
            }
        }

        checked_status = liodAction->isChecked();
    }

    emit desig_request_signal(trackDat.tn,checked_status,desig_mode);

}

void track::desig_feedback(int tn,bool approve,QString desig_mode)
{
    if(tn==trackDat.tn)
    {
        if(!approve)
        {
            QAction *action;
            bool action_checked_found = false;

            if(desig_mode == "EOT")
            {
                for(int i=0;i<DESIG_COUNT;i++)
                {
                    if(desigAction[i]->text()== desig_mode)
                    {
                        action = desigAction[i];
                        action_checked_found = true;
                        break;
                    }
                }
            }
            else if(desig_mode.contains("Direct"))
            {
                QString direct_mode = desig_mode.split(",").at(2);

                for(int i=0;i<DESIG_DIRECT_COUNT;i++)
                {
                    if(desigDirectAction[i]->text() == direct_mode)
                    {
                        action = desigDirectAction[i];
                        action_checked_found = true;
                        break;
                    }
                }

            }

            if(action_checked_found)
                action->setChecked(false);
        }
    }

    bool no_checked_eot = true;
    bool no_checked_direct = true;

    for(int i=0;i<DESIG_COUNT;i++)
    {
        if(desigAction[i]->isChecked())
        {
            no_checked_eot = false;
            break;
        }
    }

    for(int i=0;i<DESIG_DIRECT_COUNT;i++)
    {
        if(desigDirectAction[i]->isChecked())
        {
            no_checked_direct = false;
            break;
        }
    }

    if(no_checked_eot)
        cur_checked_desig = 100;

}

/*
void track::setDesigLIODEnable(bool enabled)
{
//    qDebug()<<Q_FUNC_INFO<<"tn "<<trackDat.tn<<" liod action enable"<<enabled;
    QAction *liodAction;

    for(int i=0;i<DESIG_COUNT;i++)
    {
        if(desigAction[i]->text()=="EOT")
        {
            liodAction = desigAction[i];
            break;
        }
    }
    liodAction->setEnabled(enabled);
}

void track::setDesigWeaponEnable(QString weapon, bool enabled)
{
    weapon.remove(",");
    for(int i=0;i<DESIG_DIRECT_COUNT;i++)
    {
        if(desigDirectAction[i]->text()==weapon)
            desigDirectAction[i]->setEnabled(enabled);
//        qDebug()<<Q_FUNC_INFO<<trackDat.tn<<weapon<<desigDirectAction[i]->isEnabled();

    }
}
*/

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
QString track::desig2String(Desig desig)
{
    if(desig==EOT)
        return "EOT";
}
QString track::desigDirect2String(DesigDirect desig_direct)
{
    if(desig_direct==M_40)
        return "40 mm";
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
        else if(identity==UNIDENTIFY)
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
        else if(identity==UNIDENTIFY)
            return ":/tda_track_symbol/surface-unknown.png";
    }

}


