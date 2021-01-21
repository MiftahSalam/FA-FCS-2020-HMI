#include "frametda.h"
#include "ui_frametda.h"
#include "global.h"
#include "unistd.h"

#include <math.h>
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>

FrameTDA::FrameTDA(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameTDA)
{
    ui->setupUi(this);

    timer = new QTimer(this);

    currentHeading = -1;

    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    connect(timer,SIGNAL(timeout()),this,SLOT(updateDataTracks()));
    timer->start(1000);

    // ==== Status bar ==== //
    statusBarMouse = new QStatusBar(this);
    statusBarMouse->setObjectName(QString::fromUtf8("statusBarMouse"));
    statusBarSelectedTrack = new QStatusBar(this);
    statusBarSelectedTrack->setObjectName(QString::fromUtf8("statusBarSelectedTrack"));
    statusBarSelectedTrack->hide();

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

//    tnList.clear();
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
        splash->showMessage("Track Setup error\n\nDatabase error: " + QString(e.what()) + "\n\nApplication now will clossing ",Qt::AlignCenter);
        sleep(3);
        splash->finish(this);
        qApp->exit();
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

        if(verbose)
            qDebug() << Q_FUNC_INFO <<"track:Data query size"<<trackList.size();
    }
    catch(Error e)
    {
        qDebug()<<Q_FUNC_INFO<<"error get track"<<e.what();
    }

    if((int)trackList.size()>tnList.size())
    {
//      qDebug()<<Q_FUNC_INFO<<"lebih besar" <<trackList.size() <<"tracklist" << tnList.size() <<"tnlist";

        QPoint os_pos((width())/2,(height()/2));
        std::vector<std::string> trackQuery;

        for(int i=0;i<trackList.size();i++)
        {
            if(verbose)
                qDebug() << Q_FUNC_INFO <<"track:Data query result"<<QString::fromStdString(trackList.at(i));

            try
            {
                redisClient->hmget(trackList.at(i).data(), {"id", "range", "bearing", "speed", "height", "course" }, std::back_inserter(trackQuery));

                trackParam trackdata;

                trackdata.tn= QString::fromStdString(trackQuery.at(0)).toInt();
                trackdata.range= QString::fromStdString(trackQuery.at(1)).toDouble();
                trackdata.bearing= QString::fromStdString(trackQuery.at(2)).toDouble();
                trackdata.speed= QString::fromStdString(trackQuery.at(3)).toDouble();
                trackdata.height= QString::fromStdString(trackQuery.at(4)).toDouble();
                trackdata.course= QString::fromStdString(trackQuery.at(5)).toDouble();

               // qDebug() << "TN" << trackdata.tn << "range" << trackdata.range <<"bearing" << trackdata.bearing << "speed" <<trackdata.height <<"course" <<trackdata.course;
                trackQuery.clear();

                try
                {
                    redisClient->hmget(trackList.at(i).data(), {"identity"}, std::back_inserter(trackQuery));
                    trackdata.cur_identity= int2Identity(QString::fromStdString(trackQuery.at(0)).toInt());
                    trackQuery.clear();
                }
                catch (Error e)
                {
                    redisClient->hset(trackList.at(i).data(), "identity", "0");
                    trackdata.cur_identity= int2Identity(0);

                    qDebug() << Q_FUNC_INFO << e.what() << "cek identity lebih besar ";
                }

                try
                {
                    redisClient->hmget(trackList.at(i).data(), {"weapon_assigned" }, std::back_inserter(trackQuery));
                    trackdata.weapon_assign= QString::fromStdString(trackQuery.at(0));
                    trackQuery.clear();
                }
                catch (Error e)
                {
                    redisClient->hset(trackList.at(i).data(), "weapon_assigned", "");
                    trackdata.weapon_assign= "";
                    qDebug() << Q_FUNC_INFO << e.what() << "cek weapon lebih besar";
                }

                if(verbose)
                    qDebug() << "Menampilkan data track:Data:" << trackdata.tn << trackdata.range << trackdata.bearing << trackdata.speed << trackdata.course << trackdata.cur_identity  ;

                if(!tnList.contains(trackdata.tn))
                {
                    tracks bufTracks;
                    loadTrackParam(bufTracks,trackdata);

                    //draw track
                    bufTracks.track_symbol = new track(this, QSize(60,20));
                    bufTracks.track_symbol->buildUI(bufTracks.trackData);

                    //connect
                    connect(bufTracks.track_symbol,SIGNAL(identity_change_signal(int,Identity)),this,SLOT(track_identity_changed(int,Identity)));

                    //track position in pixel
                    double range_pixel= range2Pixel(bufTracks.trackData.range);
                    double range_pixel_y = range_pixel*sin((bufTracks.trackData.bearing-90)*M_PI/180);
                    double range_pixel_x  = range_pixel*cos((bufTracks.trackData.bearing-90)*M_PI/180);
                    int final_pos_y = os_pos.y()+range_pixel_y;
                    int final_pos_x = os_pos.x()+range_pixel_x;

                    bufTracks.track_symbol->setGeometry(final_pos_x-10,final_pos_y-10,60,20); //add track simbol size correction to final pos
                    bufTracks.track_symbol->adjustSize();
                    bufTracks.track_symbol->show();

                    mapTracks->insert(bufTracks.trackData.tn,bufTracks);
                    tnList.append(trackdata.tn);

//                    qDebug() <<Q_FUNC_INFO << "tnlist" << tnList;

                    if(verbose)
                    {
                        qDebug() << Q_FUNC_INFO << "insert"<< trackdata.tn;
                        qDebug() << Q_FUNC_INFO << "mapTracks"<< mapTracks->size();
                    }
                }
            }
            catch (Error e)
            {
                qDebug() << Q_FUNC_INFO << e.what() << "cek";
            }
        }
    }
    else if(trackList.size()==tnList.size())
    {
//        qDebug() << "jika sama" << tnList.size();

        QPoint os_pos((width())/2,(height()/2));
        std::vector<std::string> trackQuery;

        qSort(tnList.begin(),tnList.end());

        for(int i=0; i<trackList.size();i++)
        {
            try
            {
                redisClient->hmget(trackList.at(i).data(), {"id", "range", "bearing", "speed", "height", "course" }, std::back_inserter(trackQuery));

                trackParam trackdata;

                trackdata.tn= QString::fromStdString(trackQuery.at(0)).toInt();
                trackdata.range= QString::fromStdString(trackQuery.at(1)).toDouble();
                trackdata.bearing= QString::fromStdString(trackQuery.at(2)).toDouble();
                trackdata.speed= QString::fromStdString(trackQuery.at(3)).toDouble();
                trackdata.height= QString::fromStdString(trackQuery.at(4)).toDouble();
                trackdata.course= QString::fromStdString(trackQuery.at(5)).toDouble();

                trackQuery.clear();

                try
                {
                    redisClient->hmget(trackList.at(i).data(), {"identity"}, std::back_inserter(trackQuery));
                    trackdata.cur_identity= int2Identity(QString::fromStdString(trackQuery.at(0)).toInt());
                    trackQuery.clear();
                }
                catch (Error e)
                {
                    redisClient->hset(trackList.at(i).data(), "identity", "0");
                    qDebug() << Q_FUNC_INFO << e.what() << "cek identity jika sama ";
                }

                try
                {
                    redisClient->hmget(trackList.at(i).data(), {"weapon_assigned" }, std::back_inserter(trackQuery));
                    trackdata.weapon_assign= QString::fromStdString(trackQuery.at(0));
                    trackQuery.clear();
                }
                catch (Error e)
                {
                    redisClient->hset(trackList.at(i).data(), "weapon_assigned", "");
                    qDebug() << Q_FUNC_INFO << e.what() << "cek weapon jika sama";
                }

                if(verbose)
                    qDebug() << "Menampilkan data track:Data:" << trackdata.tn << trackdata.range << trackdata.bearing << trackdata.speed << trackdata.course << trackdata.cur_identity  <<trackdata.weapon_assign;

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
            }
            catch (Error e)
            {
                qDebug() << Q_FUNC_INFO << e.what();
            }
        }
    }
    else if(trackList.size()<tnList.size())
    {
//        qDebug() << "jika lebih kecil" << tnList.size();

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

        if(verbose)
        {
            qDebug() << "track ID redis" << bufTn;
            qDebug() << "track ID local" << tnList;
        }

        for(int i=0; i<tnList.size();i++)
        {
            if(!bufTn.contains(tnList.at(i)))
            {
                tracks bufTracks = mapTracks->take(tnList.at(i));
                delete bufTracks.track_symbol;
                removeTn.append(tnList.at(i));
            }
        }

        if(verbose)
            qDebug() << "mencari yang beda" << removeTn;

        for(int i=0; i<removeTn.size();i++)
            tnList.removeAll(removeTn.at(i));

        if(verbose)
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

    bufParam.trackData.cur_identity = int2Identity(track_data.cur_identity);
    bufParam.trackData.cur_source = int2TrackSource(0);
    bufParam.trackData.cur_env = int2Environment(0);
    bufParam.trackData.weapon_assign = track_data.weapon_assign;
}

void FrameTDA::track_identity_changed(int tn,Identity identity)
{
    QString s = QString::number(identity2Int(identity));

    std::unordered_map<std::string, std::string> data_map =
    {
            {"identity", s.toStdString()},
    };

    try
    {
        redisClient->hmset("track:Data:"+ QString::number(tn).toStdString(), data_map.begin(), data_map.end());
    }
    catch (Error e)
    {
        qDebug() << Q_FUNC_INFO << e.what();
    }
}

// ==== Right Click TDA for contex Menu ==== //
void FrameTDA::RC_radar(QPoint pos)
{
    //qDebug()<<Q_FUNC_INFO;

    QMenu *menu = new QMenu(this);

    menu->setStyleSheet("QMenu{color: rgb(255,255,255);background-color: rgb(0,0,0);selection-color: yellow;}");
    menu->addMenu(zoomSubMenu);
    menu->exec(pos);
}

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

    // ==== Clock ==== //
    QDateTime dateTime = dateTime.currentDateTime();
    QString dateTimeString = dateTime.toString("dd-MM-yyyy hh:mm:ss");

    QRect rect1(-75,(-height()/2)+10,150,25);
    QTextOption opt1;
    opt1.setAlignment(Qt::AlignBaseline);

    painter.drawText(rect1,dateTimeString,opt1);

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

    if(currentHeading >= 0)
    {
        // ==== Heading Marker ==== //
        int sideMax = qMax(width(),height());
        painter.rotate(currentHeading);
        painter.setPen(QColor(255,255,0,255));
        painter.drawLine(0,0,0,-sideMax);
        painter.rotate(-currentHeading);

        // ==== Gun Coverage ==== //
        QString str = currentGunData.join(",");
        QStringList list = str.split(',');
        QString orientation = list.at(0);
        QString blind_arc1 = list.at(1);
        QString max_range1 = list.at(2);
        QString azimuth = list.at(3);
        const qreal gun_orientation = QString(orientation).toDouble();
        const qreal blind_arc = QString(blind_arc1).toDouble();
        const qreal max_range = QString(max_range1).toDouble(); //NM

    //    qDebug() <<Q_FUNC_INFO << gun_orientation << blind_arc << max_range;

        int span = 360-blind_arc;
        int gun_coverage_pixel = 2*range2Pixel(max_range);

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

        // ==== Gun barrel ==== //
        if(currentAccessStatus > "0")
        {
            const double bearing = azimuth.toDouble();
            int sideMax1 = qMax(width(),height());
            painter.rotate(currentHeading+bearing);
            painter.setPen(QPen(Qt::green, 5));
            painter.drawLine(0,0,0,(-sideMax1/20));
            painter.rotate(-currentHeading-bearing);
//            qDebug() <<Q_FUNC_INFO << "ok" << currentAccessStatus;
        }
    }

    // ==== Fire Triangle (QMap) ==== //
    foreach(int i, mapTracks->keys())
    {

        if(mapTracks->value(i).trackData.weapon_assign == "40 mm")
        {
            qDebug() << Q_FUNC_INFO
                     << mapTracks->value(i).trackData.tn << ","
                     << mapTracks->value(i).trackData.range << ","
                     << mapTracks->value(i).trackData.bearing << ","
                     << mapTracks->value(i).trackData.speed << ","
                     << mapTracks->value(i).trackData.course << ","
                     << mapTracks->value(i).trackData.cur_identity << ","
                     << mapTracks->value(i).trackData.weapon_assign;

                double rng = range2Pixel(mapTracks->value(i).trackData.range);
                double brn = 90-mapTracks->value(i).trackData.bearing;

                int rangetrack =rng*cos(brn*(M_PI/180))-M_PI+(-9);
                int bearingtrack = -rng*sin(brn*(M_PI/180))-M_PI-8;
                painter.setPen(QPen(Qt::yellow, 3));

                qDebug()<< rangetrack << "range";
                qDebug()<< bearingtrack << "bearing";

                // ==== Gambar Kotak ==== //
              //  painter.drawRect(rangetrack,bearingtrack,23,23);

                // ==== Contoh gambar Track ==== //
                // ==== horizontal ==== //
//                painter.drawLine(-36, 64, -30, 64);
//                painter.drawLine(-36, 90, -30, 90);

//                painter.drawLine(-17, 64, -11, 64);
//                painter.drawLine(-17, 90, -11, 90);

//                // ==== vertical ==== //
//                painter.drawLine(-36, 64, -36, 70);
//                painter.drawLine(-36, 84, -36, 90);

//                painter.drawLine(-11, 64, -11, 70);
//                painter.drawLine(-11, 84, -11, 90);

                // ==== horizontal ==== //
                painter.drawLine(rangetrack, bearingtrack, rangetrack+5, bearingtrack);
                painter.drawLine(rangetrack, bearingtrack+21, rangetrack+5, bearingtrack+21);
                painter.drawLine(rangetrack+19, bearingtrack, rangetrack+24, bearingtrack);
                painter.drawLine(rangetrack+24, bearingtrack+21, rangetrack+19, bearingtrack+21);

                // ==== vertical ==== //
                painter.drawLine(rangetrack, bearingtrack, rangetrack, bearingtrack+5);
                painter.drawLine(rangetrack, bearingtrack+15,  rangetrack, bearingtrack+21);
                painter.drawLine(rangetrack+24, bearingtrack, rangetrack+24, bearingtrack+5);
                painter.drawLine(rangetrack+24, bearingtrack+15, rangetrack+24, bearingtrack+21);

            try
            {
                std::vector<std::string> fire;
                redisClient->hmget("fire_triangle", {"ttlf", "ttlf_x", "ttlf_y"}, std::back_inserter(fire));

                double TTLF_x = QString::fromStdString(fire.at(1)).toDouble();
                double TTLF_y = QString::fromStdString(fire.at(2)).toDouble();
                double rng = range2Pixel(mapTracks->value(i).trackData.range);
                double brn = 90-mapTracks->value(i).trackData.bearing;
                int tn = mapTracks->value(i).trackData.tn;
                painter.setPen(QColor(0,255,0,255));
                painter.drawLine(0,0,range2Pixel(TTLF_x),-range2Pixel(TTLF_y));
                painter.drawLine(rng*cos(brn*(M_PI/180)),-rng*sin(brn*(M_PI/180)),range2Pixel(TTLF_x),-range2Pixel(TTLF_y));
                painter.drawLine(rng*cos(brn*(M_PI/180)),-rng*sin(brn*(M_PI/180)),0,0);

                qDebug() << Q_FUNC_INFO << TTLF_x << TTLF_y;

                statusBarSelectedTrack->showMessage(QString("Tn : %1     "
                                                            "Range : %2 NM     "
                                                            "Bearing : %3     "
                                                            "Speed : %4 kts     "
                                                            "Course : %5     "
                                                            //                                                    "Height : %6 ft     "
                                                            )
                                                    .arg(tn)
                                                    .arg(QString::number(mapTracks->value(tn).trackData.range,'f',2))
                                                    .arg(QString::number(mapTracks->value(tn).trackData.bearing,'f',2))
                                                    .arg(QString::number(mapTracks->value(tn).trackData.speed,'f',2))
                                                    .arg(QString::number(mapTracks->value(tn).trackData.course,'f',2))
                                                    );
                break;
            }
            catch (Error e)
            {
                curStatusString = e.what();
                qDebug() << Q_FUNC_INFO << "cannot find fire_triangle" <<curStatusString;
            }
        }
        // ==== Track Selected ==== //
    }
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
    bool ok;
    currentHeading = heading.toFloat(&ok);

    if(!ok)
        currentHeading = -1;
//    qDebug()<<Q_FUNC_INFO<<currentHeading<<ok;
}

void FrameTDA::setAccessStatus(QString access_status)
{
    currentAccessStatus = access_status;
//    qDebug() <<Q_FUNC_INFO << access_status;
}

void FrameTDA::setGundata(QStringList datagun)
{
    currentGunData = datagun;
//    qDebug() <<"hasil set"<<datagun;
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
