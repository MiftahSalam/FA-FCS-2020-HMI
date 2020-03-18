
#include "frametda.h"
#include "ui_frametda.h"

#include <QDebug>
#include <math.h>
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>


FrameTDA::FrameTDA(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameTDA)
{
    ui->setupUi(this);

    timer = new QTimer(this);

    currentHeading = 0.0;

    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    connect(timer,SIGNAL(timeout()),this,SLOT(updateDataTracks()));
    timer->start(1000);

    // ==== Status bar ==== //
    statusBarMouse = new QStatusBar(this);
    statusBarMouse->setObjectName(QString::fromUtf8("statusBarMouse"));
    statusBarSelectedTrack = new QStatusBar(this);
    statusBarSelectedTrack->setObjectName(QString::fromUtf8("statusBarSelectedTrack"));

    // ==== Mouse event ==== //
    setMouseTracking(true);

    rc_radarevent=new RCEventHandler(this); // menu klik kanan pada TDA
    connect(rc_radarevent,SIGNAL(send_rightButtonClicked(QPoint)),this,SLOT(RC_radar(QPoint))); //tombol2 klik kanan radar
    connect(rc_radarevent,SIGNAL(move_mouse(QPointF)),this,SLOT(mousepos(QPointF)));

    installEventFilter(rc_radarevent);

    // ==== Contex menu ==== //
    zoomSubMenu = new QMenu("Zoom",this);
    zoomSubMenu->setStyleSheet("QMenu{color: rgb(255,255,255);background-color: rgb(0,0,0);selection-color: yellow;}");
    for(int i=0;i<Z_TOTAL;i++)
    {
        zoomAction[i] = new QAction(zoomScale2String(zoomInt2Scale(i)),this);
        connect(zoomAction[i],SIGNAL(triggered()),this,SLOT(zoom_change()));
        zoomAction[i]->setCheckable(true);
        zoomSubMenu->addAction(zoomAction[i]);
    }
    cur_checked_zoom_scale = zoomScale2Int(Z_080);
    zoomAction[cur_checked_zoom_scale]->setChecked(true);

    cur_selected_track = -1;

    tnList.clear();
    mapTracks = new QMap<int,tracks>;
    mapTracks->clear();

    tdaScale = 8.0;


}

void FrameTDA::zoom_change()
{
    for(int i=0;i<Z_TOTAL;i++)
    {
        if(zoomAction[i]->isChecked() && i!=cur_checked_zoom_scale)
        {
            zoomAction[cur_checked_zoom_scale]->setChecked(false);
            cur_checked_zoom_scale = i;
        }
    }

    tdaScale = zoomAction[cur_checked_zoom_scale]->text().remove(" NM").toDouble();
    //    qDebug()<<Q_FUNC_INFO<<tdaScale;
}

void FrameTDA::setConfig(QString Config)
{
    this->Config = Config;
    qDebug() <<Q_FUNC_INFO<<"Redis config"<<this->Config;

    try
    {
        redisClient = new Redis(this->Config.toStdString());
    }
    catch (Error e)
    {
        qDebug() <<Q_FUNC_INFO;
    }
}

void FrameTDA::updateDataTracks()
{
    /*
     * flow program
     * 1. query available track ke redis
     * 2. bandingkan jumlah track hasil query redis dengan record local
     *    -> jika lebih besar
     *       >> iterasi semua hasil query redis
     *       >> cari track id yang tidak terdapat di record lokal (track baru)
     *          ~ jika ada track baru
     *            <> gambar track
     *            <> insert track ke record local
     *  ke record local (tnList dan mapTrack)
     *    -> jika sama
     *       >> iterasi semua hasil query redis
     *       >> update track data dan gambar
     *    -> jika lebih kecil
     *       >> iterasi semua hasil query redis
     *       >> cari track-track id yang tidak terdapat di record lokal (track hilang)
     *          ~ hapus gambar track
     *          ~ hapus track dari record local (tnList dan mapTrack)

    */

    std::vector<std::string> trackList;
    try
    {
        redisClient->keys("track:Data:*",std::back_inserter(trackList));
        qDebug() << Q_FUNC_INFO <<"track:Data query size"<<trackList.size();
    }
    catch(Error e)
    {
        qDebug()<<Q_FUNC_INFO<<"error get track"<<e.what();
    }

    if((int)trackList.size()>tnList.size())
    {
        qDebug()<<Q_FUNC_INFO<<"lebih besar";

        QPoint os_pos((width())/2,(height()/2));

        for(int i=0;i<trackList.size();i++)
        {
            qDebug() << Q_FUNC_INFO <<"track:Data query result"<<QString::fromStdString(trackList.at(i));

            std::vector<std::string> trackQuery;
            try
            {
                redisClient->hmget(trackList.at(i).data(), {"id", "range", "bearing", "speed", "course", "identity" }, std::back_inserter(trackQuery));

                trackParam trackdata;

                trackdata.tn= QString::fromStdString(trackQuery.at(0)).toInt();
                trackdata.range= QString::fromStdString(trackQuery.at(1)).toDouble();
                trackdata.bearing= QString::fromStdString(trackQuery.at(2)).toDouble();
                trackdata.speed= QString::fromStdString(trackQuery.at(3)).toDouble();
                trackdata.course= QString::fromStdString(trackQuery.at(4)).toDouble();
                trackdata.identity= QString::fromStdString(trackQuery.at(5)).toDouble();


                qDebug() << "Menampilkan data track:Data:" << trackdata.tn << trackdata.range << trackdata.bearing << trackdata.speed << trackdata.course << trackdata.identity  ;

                if(!tnList.contains(trackdata.tn))
                {
                    tracks bufTracks;
                    loadTrackParam(bufTracks,trackdata);

                    //draw track
                    bufTracks.track_symbol = new track(this, QSize(60,20));
                    bufTracks.track_symbol->buildUI(bufTracks.trackData);

                    //track position in pixel
                    double range_pixel= range2Pixel(bufTracks.trackData.range);
                    qDebug()<< range_pixel;
                    double range_pixel_y = range_pixel*sin((bufTracks.trackData.bearing-90)*M_PI/180);
                    double range_pixel_x  = range_pixel*cos((bufTracks.trackData.bearing-90)*M_PI/180);
                    int final_pos_y = os_pos.y()+range_pixel_y;
                    int final_pos_x = os_pos.x()+range_pixel_x;

                    bufTracks.track_symbol->setGeometry(final_pos_x-10,final_pos_y-10,60,20); //add track simbol size correction to final pos
                    bufTracks.track_symbol->adjustSize();
                    bufTracks.track_symbol->show();

                    mapTracks->insert(bufTracks.trackData.tn,bufTracks);
                    tnList.append(trackdata.tn);

                    qDebug() << Q_FUNC_INFO << "insert"<< trackdata.tn;
                    qDebug() << Q_FUNC_INFO << "mapTracks"<< mapTracks->size();
                }
            }
            catch (Error e)
            {
                qDebug() << Q_FUNC_INFO << e.what();
            }
        }
    }
    else if(trackList.size()==tnList.size())
    {
        qDebug() << "jika sama" << tnList.size();

        QPoint os_pos((width())/2,(height()/2));

        qSort(tnList.begin(),tnList.end());

        for(int i=0; i<trackList.size();i++)
        {
            std::vector<std::string> trackQuery;

            try
            {
                redisClient->hmget(trackList.at(i).data(), {"id", "range", "bearing", "speed", "course", "identity" }, std::back_inserter(trackQuery));

                trackParam trackdata;

                trackdata.tn= QString::fromStdString(trackQuery.at(0)).toInt();
                trackdata.range= QString::fromStdString(trackQuery.at(1)).toDouble();
                trackdata.bearing= QString::fromStdString(trackQuery.at(2)).toDouble();
                trackdata.speed= QString::fromStdString(trackQuery.at(3)).toDouble();
                trackdata.course= QString::fromStdString(trackQuery.at(4)).toDouble();
                trackdata.identity= QString::fromStdString(trackQuery.at(5)).toDouble();


                qDebug() << "Menampilkan data track:Data:" << trackdata.tn << trackdata.range << trackdata.bearing << trackdata.speed << trackdata.course << trackdata.identity  ;

                    int tn = trackdata.tn;
                    tracks bufTracks = mapTracks->take(tn);
                    loadTrackParam(bufTracks,trackdata);


                    if(cur_selected_track==tn)
                    {
                        bufTracks.track_symbol->setSelected(true);
                        bufTracks.track_symbol->raise();
                    }
                    else
                    {
                        bufTracks.track_symbol->setSelected(false);
                    }

                    mapTracks->insert(bufTracks.trackData.tn,bufTracks);

                    //update track position in tda
                    double range_pixel = range2Pixel(mapTracks->value(tn).trackData.range);
                    double range_pixel_y = range_pixel*sin((mapTracks->value(tn).trackData.bearing-90)*M_PI/180);
                    double range_pixel_x = range_pixel*cos((mapTracks->value(tn).trackData.bearing-90)*M_PI/180);
                    int final_pos_y = os_pos.y()+range_pixel_y;
                    int final_pos_x = os_pos.x()+range_pixel_x;

                    mapTracks->value(tn).track_symbol->setGeometry(final_pos_x-10,final_pos_y-10,60,20);
                    mapTracks->value(tn).track_symbol->updateData(bufTracks.trackData);


                /*
                else //track hilang
                {
                    qDebug()<<Q_FUNC_INFO<<"delete one ";

                    tracks bufTracks = mapTracks->take(tnList.at(cur_track_index_update));
                    delete bufTracks.track_symbol;
                    tnList.removeAt(cur_track_index_update);
                }*/
            }
            catch (Error e)
            {
                qDebug() << Q_FUNC_INFO << e.what();
            }

        }
    }
    else if(trackList.size()<tnList.size())
    {
        qDebug() << "jika lebih kecil" << tnList.size();

        QList <int> bufTn;
        QList <int> removeTn;

        bufTn.clear();
        removeTn.clear();

        for(int i=0; i<trackList.size();i++)
        {
            std::vector<std::string> trackQuery;

            try
            {
                redisClient->hmget(trackList.at(i).data(), {"id"}, std::back_inserter(trackQuery));

                int trackID = QString::fromStdString(trackQuery.at(0)).toInt();
                bufTn.append(trackID);
            }
            catch (Error e)
            {
                qDebug() << Q_FUNC_INFO << e.what();
            }

        }

        qDebug() << "track ID redis" << bufTn;
        qDebug() << "track ID local" << tnList;

        for(int i=0; i<tnList.size();i++)
        {
            if(!bufTn.contains(tnList.at(i)))
            {
                tracks bufTracks = mapTracks->take(tnList.at(i));
                delete bufTracks.track_symbol;
                removeTn.append(tnList.at(i));
            }
        }

        qDebug() << "mencari yang beda" << removeTn;

        for(int i=0; i<removeTn.size();i++)
            tnList.removeAll(removeTn.at(i));

        qDebug() << "remove" << removeTn;
    }
}

void FrameTDA::loadTrackParam(tracks &bufParam, trackParam track_data)
{
    bufParam.trackData.tn = track_data.tn;
    bufParam.trackData.range = track_data.range;
    bufParam.trackData.bearing = track_data.bearing;
    bufParam.trackData.speed = track_data.speed;
    bufParam.trackData.course = track_data.course;
    bufParam.trackData.identity = track_data.identity;

    bufParam.trackData.cur_identity = int2Identity(0);
    bufParam.trackData.cur_source = int2TrackSource(0);
    bufParam.trackData.cur_env = int2Environment(0);
    bufParam.trackData.weapon_assign = track_data.weapon_assign;

}

void FrameTDA::track_identity_changed(int tn,Identity identity)
{
   // trackdata(QString.arg(identity2Int(identity)).arg(tn));
}

// ==== Right Click TDA for contex Menu ==== //
void FrameTDA::RC_radar(QPoint pos)
{
    //qDebug()<<Q_FUNC_INFO;

    QMenu *menu = new QMenu(this);
    /*
    QMenu *selectTrackSubMenu = new QMenu("Select Track", this);

    // ==== select track handling ==== //
    for(int i=0;i<tnList.size();i++)
    {
        QAction *stAction = new QAction(QString::number(tnList.at(i)),this);
        connect(stAction,SIGNAL(triggered()),this,SLOT(selectedTrack()));
        stAction->setCheckable(true);

        if(cur_selected_track==tnList.at(i) && cur_selected_track>-1)
            stAction->setChecked(true);

        selectTrackSubMenu->addAction(stAction);
        stAction->deleteLater();
    }
   */
    menu->setStyleSheet("QMenu{color: rgb(255,255,255);background-color: rgb(0,0,0);selection-color: yellow;}");
    menu->addMenu(zoomSubMenu);
    //menu->addMenu(selectTrackSubMenu);
    menu->exec(pos);
}

/*
void FrameTDA::selectedTrack()
{
    QAction *actionSender = (QAction *) QObject::sender();
    int selectTrack = actionSender->text().toInt();

    //qDebug()<<Q_FUNC_INFO<< selectTrack;

    if(selectTrack!=cur_selected_track)
        cur_selected_track = actionSender->text().toInt();
    else
        cur_selected_track = -1;
}
*/

void FrameTDA::mousepos(QPointF point)
{
    QPoint os_pos((width())/2,(height()/2));
    double range_pixel_x = os_pos.x()-point.x();
    double range_pixel_y = os_pos.y()-point.y();
    double bearing = atan2(range_pixel_y,range_pixel_x);
    bearing = (bearing*180/M_PI)-90;
    if(bearing<0)
        bearing+=360;

    double range = sqrt(pow(range_pixel_y,2)+pow(range_pixel_x,2)); //pixel
    range = pixel2Range(range); //NM
    statusBarMouse->showMessage(QString("Range : %1, Bearing : %2").arg(QString::number(range,'f',1)).arg(QString::number(bearing,'f',1)),2000);
    statusBarMouse->setGeometry(10,height()-40,200,20);
    //    qDebug() << Q_FUNC_INFO << range << bearing;
}

void FrameTDA::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    //    qDebug()<<Q_FUNC_INFO;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRect(0,0,width(),height());
    painter.setPen(Qt::SolidLine);
    painter.setPen(QColor(192,128,0,255));
    painter.translate(width()/2,height()/2);

    // ==== Compass Scale Text ==== //
    int range = qMin(width(),height())/4;
    QString text;
    for(int j=0;j<12;j++)
    {
        if(j<9)
            QTextStream(&text)<<(j*30)+90;
        else
            QTextStream(&text)<<(j*30)-270;

        QRect rect((range+20)*cos((j*30)*M_PI/180)-15,((range+20)*sin((j*30)*M_PI/180)-5),30,15);
        QTextOption opt;
        opt.setAlignment(Qt::AlignHCenter);
        QFont font;

        font.setPixelSize(12);
        painter.setFont(font);
        painter.drawText(rect,text,opt);
        text.clear();
    }

    // ==== Compass Ring ==== //
    for(int j=0;j<180;j++)
    {
        if(j%15==0)
            painter.drawLine(0,range-10,0,range+10);
        else
            painter.drawLine(0,range-5,0,range+5);

        painter.rotate(2);
    }

    // ==== Heading Marker ==== //
    int sideMax = qMax(width(),height());
    painter.rotate(currentHeading);
    painter.setPen(QColor(255,255,0,255));
    painter.drawLine(0,0,0,-sideMax);
    painter.rotate(-currentHeading);

    // ==== Gun Coverage ==== //
    const qreal gun_orientation = 0.0;
    const qreal blind_arc = 90.0;
    const qreal gun_fire_range = 7.0; //NM

    int span = 360-blind_arc;
    int gun_coverage_pixel = 2*range2Pixel(gun_fire_range);

    painter.setPen(QColor(255,0,0,255));
    painter.rotate(gun_orientation+currentHeading+qreal((span/2)-90));
    painter.drawPie(-gun_coverage_pixel/2,-gun_coverage_pixel/2,gun_coverage_pixel,gun_coverage_pixel,0*16,span*16);
    painter.rotate(-gun_orientation-currentHeading-qreal((span/2)-90));

    QRect rect(-((gun_coverage_pixel/2)+20)*cos((currentHeading+100)*M_PI/180)-15,-((gun_coverage_pixel/2)+20)*sin((currentHeading+100)*M_PI/180)-5,30,15);
    QTextOption opt;
    opt.setAlignment(Qt::AlignHCenter);
    QFont font;

    font.setPixelSize(11);
    painter.setFont(font);
    painter.drawText(rect,QString("40mm"),opt);

    /*
    bool create_fire_triangle = query.value(0).toBool();

    if(create_fire_triangle)
    {
    double TTLF_x = query.value(3).toDouble();
    double TTLF_y = query.value(4).toDouble();
    double rng = range2Pixel(query.value(0).toDouble());
    double brn = 90-query.value(1).toDouble();
    int tn = query.value(5).toInt();
    painter.setPen(QColor(0,255,0,255));
    painter.drawLine(0,0,range2Pixel(TTLF_x),-range2Pixel(TTLF_y));
    painter.drawLine(rng*cos(brn*(M_PI/180)),-rng*sin(brn*(M_PI/180)),range2Pixel(TTLF_x),-range2Pixel(TTLF_y));
    painter.drawLine(rng*cos(brn*(M_PI/180)),-rng*sin(brn*(M_PI/180)),0,0);

    statusBarSelectedTrack->showMessage(QString("Tn : %1     "
                                                "Range : %2 NM     "
                                                "Bearing : %3     "
                                                "Speed : %4 kts     "
                                                "Course : %5     "
                                                //"Height : %6 ft     "
                                                )
                                        .arg(tn)
                                        .arg(QString::number(mapTracks->value(tn).trackData.range,'f',2))
                                        .arg(QString::number(mapTracks->value(tn).trackData.bearing,'f',2))
                                        .arg(QString::number(mapTracks->value(tn).trackData.speed,'f',2))
                                        .arg(QString::number(mapTracks->value(tn).trackData.course,'f',2))
                                        );
    }
    else
        statusBarSelectedTrack->clearMessage();
    */
}

int FrameTDA::zoomScale2Int(zoomScale scale)
{
    if(scale==Z_025)
        return 0;
    else if(scale==Z_050)
        return 1;
    else if(scale==Z_010)
        return 2;
    else if(scale==Z_020)
        return 3;
    else if(scale==Z_040)
        return 4;
    else if(scale==Z_080)
        return 5;
    else if(scale==Z_160)
        return 6;
    else if(scale==Z_320)
        return 7;
    else if(scale==Z_640)
        return 8;
}

FrameTDA::zoomScale FrameTDA::zoomInt2Scale(int scale)
{
    if(scale==0)
        return Z_025;
    else if(scale==1)
        return Z_050;
    else if(scale==2)
        return Z_010;
    else if(scale==3)
        return Z_020;
    else if(scale==4)
        return Z_040;
    else if(scale==5)
        return Z_080;
    else if(scale==6)
        return Z_160;
    else if(scale==7)
        return Z_320;
    else if(scale==8)
        return Z_640;
    else
        return Z_TOTAL;
}

QString FrameTDA::zoomScale2String(zoomScale scale)
{
    if(scale==Z_025)
        return "0.25 NM";
    else if(scale==Z_050)
        return "0.5 NM";
    else if(scale==Z_010)
        return "1 NM";
    else if(scale==Z_020)
        return "2 NM";
    else if(scale==Z_040)
        return "4 NM";
    else if(scale==Z_080)
        return "8 NM";
    else if(scale==Z_160)
        return "16 NM";
    else if(scale==Z_320)
        return "32 NM";
    else if(scale==Z_640)
        return "64 NM";
    else
        return "0";
}

FrameTDA::zoomScale FrameTDA::zoomString2Scale(QString scale)
{
    if(scale=="0.25 NM")
        return Z_025;
    else if(scale=="0.5 NM")
        return Z_050;
    else if(scale=="1 NM")
        return Z_010;
    else if(scale=="2 NM")
        return Z_020;
    else if(scale=="4 NM")
        return Z_040;
    else if(scale=="8 NM")
        return Z_080;
    else if(scale=="16 NM")
        return Z_160;
    else if(scale=="32 NM")
        return Z_320;
    else if(scale=="64 NM")
        return Z_640;
    else
        return Z_TOTAL;
}

void FrameTDA::setHeading(QString heading)
{
    currentHeading = heading.toFloat();
    //    qDebug()<<Q_FUNC_INFO<<currentHeading;

}

int FrameTDA::range2Pixel(double range)
{
    return range*(width()/(2*tdaScale));
}

double FrameTDA::pixel2Range(int pixel)
{
    return 2*tdaScale*pixel/width();
}

FrameTDA::~FrameTDA()
{
    delete ui;
}
