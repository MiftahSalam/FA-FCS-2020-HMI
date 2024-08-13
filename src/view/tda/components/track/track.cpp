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

    /*contex menu identity*/
    identitySubMenu = new QMenu("Identity",this);
    identitySubMenu->setStyleSheet("background-color: black;");
    for(int i=0;i<TrackUtils::IDENTITY_COUNT;i++)
    {
        identityAction[i] = new QAction(TrackUtils::identity2String(TrackUtils::int2Identity(i)),this);
        connect(identityAction[i], &QAction::triggered, this, &Track::identityChange);
        identityAction[i]->setCheckable(true);

        identitySubMenu->addAction(identityAction[i]);
    }
    curCheckedIdentity = TrackUtils::identity2Int(param->getCur_identity());
    identityAction[curCheckedIdentity]->setChecked(true);

    /*contex menu environment
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
    */

    /*create QLabel for holding track image and information display*/
    trackIconLabel = new TdaTrackIcon(this, param);

    /*
      track event handler
    rc_radarevent=new TdaEventFilter();
    connect(rc_radarevent,SIGNAL(send_rightButtonClicked(QPoint)),this,SLOT(RC_track(QPoint))); //tombol2 klik kanan track
    trackIconLabel->installEventFilter(rc_radarevent);
    */
\
    /*create QLabel for holding track number and source display*/
    trackIdLabel = new TdaTrackId(this, param->getTn(), param->getCur_source());
    trackIdLabel->setGeometry(QRect(trackIconLabel->width()+5,0,width()*2/3,height()));
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
    trackIdLabel->changeSource(param.getCur_source());
    trackIconLabel->updateProps(param);

    *trackData = param;
}

/*right click menu*/
void Track::RC_track(QPoint pos)
{
    qDebug()<<Q_FUNC_INFO<<" point "<<pos;

    QMenu *menu = new QMenu(this);
    menu->setStyleSheet("background-color: black;");
    menu->addMenu(identitySubMenu);
//    menu->addMenu(envSubMenu);
   // menu->addMenu(desigSubMenu);
    menu->exec(pos);
}

void Track::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<Q_FUNC_INFO<<" point "<<event->globalPos();

    QMenu *menu = new QMenu(this);
    menu->setStyleSheet("background-color: black;");
    menu->addMenu(identitySubMenu);
//    menu->addMenu(envSubMenu);
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
    emit identityChange_Signal(trackData->getTn(),TrackUtils::int2Identity(curCheckedIdentity));
}
/*
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
*/
const TrackParam *Track::getTrackData() const
{
    return trackData;
}
