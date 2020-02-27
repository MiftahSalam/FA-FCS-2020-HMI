#include "frameosd.h"
#include "ui_frameosd.h"
#include "global.h"
#include "unistd.h"
#include "math.h"

#include <QMessageBox>
#include <QRegExpValidator>

#define NUMBER_RX "[0-9.-]+$"


FrameOSD::FrameOSD(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameOSD)
{
    ui->setupUi(this);

    QRegExp rxNumber(NUMBER_RX);
    QRegExpValidator *valiNumber = new QRegExpValidator(rxNumber, this);

    // ==== validator Gyro ==== //
    ui->osdGyroHeadingValue->setValidator(valiNumber);
    ui->osdGyroPitchValue->setValidator(valiNumber);
    ui->lineEditGyroRoll->setValidator(valiNumber);

    // ==== validator Wind ==== //
    ui->lineEditWindDir->setValidator(valiNumber);
    ui->lineEditWindSpeed->setValidator(valiNumber);

    // ==== validator Weather ==== //
    ui->lineEditWeatherTemp->setValidator(valiNumber);
    ui->lineEditWeatherPress->setValidator(valiNumber);
    ui->lineEditWeatherHumidity->setValidator(valiNumber);

    // ==== validator Speed ==== //
    ui->lineEditSpeedSOG->setValidator(valiNumber);
    ui->lineEditSpeedCOG->setValidator(valiNumber);

    // ==== validator Water Speed ==== //
    ui->lineEditWaterSOG->setValidator(valiNumber);
    ui->lineEditWaterCOG->setValidator(valiNumber);


    // ==== redisClient validator Gyro ==== //
    ui->osdGyroHeadingValue->setValidator(valiNumber);
    ui->osdGyroPitchValue->setValidator(valiNumber);
    ui->lineEditGyroRoll->setValidator(valiNumber);

    // ==== redisClient validator GPS ==== //
    // ui->lineEditGpsLat->setValidator(valiNumber);
    // ui->lineEditGpsLong->setValidator(valiNumber);

    // ==== redisClient validator Wind ==== //
    ui->lineEditWindDir->setValidator(valiNumber);
    ui->lineEditWindSpeed->setValidator(valiNumber);

    // ==== redisClient validator Weather ==== //
    ui->lineEditWeatherTemp->setValidator(valiNumber);
    ui->lineEditWeatherPress->setValidator(valiNumber);
    ui->lineEditWeatherHumidity->setValidator(valiNumber);

    // ==== redisClient validator Speed ==== //
    ui->lineEditSpeedSOG->setValidator(valiNumber);
    ui->lineEditSpeedCOG->setValidator(valiNumber);

    // ==== redisClient validator Water Speed ==== //
    ui->lineEditWaterSOG->setValidator(valiNumber);
    ui->lineEditWaterCOG->setValidator(valiNumber);

}

FrameOSD::~FrameOSD()
{
    delete ui;
}

QString FrameOSD::getHeading() const
{
    return inersiadata.heading;
}

void FrameOSD::setConfig(QString Config)
{
    this->Config = Config;
    qDebug() <<Q_FUNC_INFO<<"Redis config"<<this->Config;

    try
    {
        redisClient = new Redis(this->Config.toStdString());
    }
    catch (Error e)
    {
        splash->showMessage("OSD Setup error\n\nDatabase error: " + QString(e.what()) + "\n\nApplication now will clossing ",Qt::AlignCenter);
        sleep(3);
        splash->finish(this);
        qApp->exit();
    }

    bool status;
    status = false;

    // ==== Gyro ==== //
    try
    {
        status = redisClient->set("inersia_mode", "auto");
        curStatusString = "";
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << "Gyro " << curStatusString;
    }

    // ==== GPS ==== //
    try
    {
        status = redisClient->set("position_mode", "auto");
        curStatusString = "";
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << "GPS" << curStatusString;
    }

    // ==== Wind ==== //
    try
    {
        status = redisClient->set("wind_mode", "auto");
        curStatusString = "";
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << "Wind" << curStatusString;
    }

    // ==== Weather ==== //
    try
    {
        status = redisClient->set("weather_mode", "auto");
        curStatusString = "";
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << "Weather" << curStatusString;
    }

    // ==== Speed ==== //
    try
    {
        status = redisClient->set("speed_mode", "auto");
        curStatusString = "";
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << "Speed" << curStatusString;
    }

    // ==== Water Speed ==== //
    try
    {
        status = redisClient->set("waterspeed_mode", "auto");
        curStatusString = "";
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << "Speed" << curStatusString;
    }

    qDebug() << "curStatusString" << curStatusString;
}

void FrameOSD::on_osdTimerTimeOut()
{
    if(!curStatusString.isEmpty())
    {
        if(curStatusString.contains("refuse"))
        {
            if(isEnabled())
                setEnabled(false);
        }
    }
    else
    {
        if(!isEnabled())
            setEnabled(true);
    }

    GyroTimerTimeOut();
    GpsTimerTimeOut();
    WindTimerTimeOut();
    WeatherTimerTimeOut();
    SpeedTimerTimeOut();
    WaterSpeedTimerTimeOut();
}


// ==== Gyro ==== //
void FrameOSD::GyroAutoModeUi()
{
    ui->pushButtonGyroApply->setEnabled(false);
    ui->pushButtonGyroApply->setStyleSheet("color: rgb(20, 20, 20);");
    ui->osdGyroHeadingValue->setEnabled(false);
    ui->osdGyroPitchValue->setEnabled(false);
    ui->lineEditGyroRoll->setEnabled(false);
}

void FrameOSD::GyroManualModeUi()
{
    ui->pushButtonGyroApply->setEnabled(true);
    ui->pushButtonGyroApply->setStyleSheet("");
    ui->osdGyroHeadingValue->setStyleSheet("color:white;");
    ui->lineEditGyroRoll->setStyleSheet("color:white;");
    ui->osdGyroPitchValue->setStyleSheet("color:white;");
    ui->osdGyroHeadingValue->setEnabled(true);
    ui->osdGyroPitchValue->setEnabled(true);
    ui->lineEditGyroRoll->setEnabled(true);
}

void FrameOSD::GyroTimerTimeOut()
{
    QString inersiamode ;
    try
    {
         auto inersia_mode = redisClient->get("inersia_mode");
         inersiamode = QString::fromStdString(*inersia_mode);
         curStatusString = "";
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << Q_FUNC_INFO <<  curStatusString;
    }

    //qDebug() << Q_FUNC_INFO << inersiamode;

    if(inersiamode == "auto")
    {
        bool status = false;
        try
        {
            status = redisClient->exists("inersia");
            curStatusString = "";
        }
        catch (Error e)
        {
            curStatusString = e.what();
            qDebug() << Q_FUNC_INFO <<  curStatusString;
        }

        if(status)
        {
            ui->osdGyroHeadingValue->setStyleSheet("color:rgb(0, 255, 0);");
            ui->lineEditGyroRoll->setStyleSheet("color:rgb(0, 255, 0);");
            ui->osdGyroPitchValue->setStyleSheet("color:rgb(0, 255, 0);");

            std::vector<std::string> inersia;
            inersia.reserve(3);

            try
            {
                redisClient->hmget("inersia", {"heading", "roll", "pitch"}, std::back_inserter(inersia));

                curStatusString = "";

                inersiadata.heading = QString::fromStdString(inersia.at(0));
                inersiadata.roll = QString::fromStdString(inersia.at(1));
                inersiadata.picth = QString::fromStdString(inersia.at(2));

                ui->osdGyroHeadingValue->setText(inersiadata.heading);
                ui->lineEditGyroRoll->setText(inersiadata.roll);
                ui->osdGyroPitchValue->setText(inersiadata.picth);
            }
            catch (Error e)
            {
                curStatusString = e.what();
                qDebug() << Q_FUNC_INFO <<  curStatusString;
            }

        }
        else
        {
            ui->osdGyroHeadingValue->setStyleSheet("color: rgb(255, 0, 0);");
            ui->lineEditGyroRoll->setStyleSheet("color: rgb(255, 0, 0);");
            ui->osdGyroPitchValue->setStyleSheet("color: rgb(255, 0, 0);");
        }
    }
    //qDebug() << Q_FUNC_INFO;
}

void FrameOSD::on_osdGryoComboBox_activated(int index)
{
    if (index)
    {
        try
        {
            redisClient->set("inersia_mode", "auto");
            curStatusString = "";

            ui->osdGyroHeadingValue->setEnabled(false);
            ui->lineEditGyroRoll->setEnabled(false);
            ui->osdGyroPitchValue->setEnabled(false);

            GyroAutoModeUi();
        }
        catch (Error e)
        {
            curStatusString = e.what();
            qDebug() << Q_FUNC_INFO <<  curStatusString;
        }
    }
    else
    {
        try
        {
            redisClient->set("inersia_mode", "manual");
            curStatusString = "";

            ui->osdGyroHeadingValue->setEnabled(true);
            ui->lineEditGyroRoll->setEnabled(true);
            ui->osdGyroPitchValue->setEnabled(true);

            GyroManualModeUi();

            try
            {
                bool key_exist = redisClient->exists("inersia");

                if(!key_exist)
                {
                    std::unordered_map<std::string, std::string> data_map =
                    {
                        {"heading", "0.0"},
                        {"roll", "0.0"},
                        {"pitch", "0.0"},
                    };

                    redisClient->hmset("inersia",data_map.begin(), data_map.end());
                }
                else
                    redisClient->persist("inersia");

            }
            catch (Error e)
            {
                curStatusString = e.what();
                qDebug() << Q_FUNC_INFO <<  curStatusString;
            }
        }
        catch (Error e)
        {
            curStatusString = e.what();
            qDebug() << Q_FUNC_INFO <<  curStatusString;
        }
    }
}

void FrameOSD::on_pushButtonGyroApply_clicked()
{
    QString heading = ui->osdGyroHeadingValue->text();
    QString roll = ui->lineEditGyroRoll->text();
    QString pitch = ui->osdGyroPitchValue->text();

    bool ok;

    float heading_float = heading.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error Heading", "Invalid input value\nValid input range : 0 - 360" );
        return;
    }
    if ((heading_float < 0) || (heading_float > 360) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid heading input\nValid input range : 0 - 360" );
        return;
    }

    float roll_float = roll.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error Roll", "Invalid input value\nValid input range : -30 to 30" );
        return;
    }
    if ((roll_float < -30) || (roll_float > 30) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid roll input\nValid input range : -30 to 30" );
        return;
    }

    float pitch_float = pitch.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error Pitch", "Invalid input value\nValid input range : -30 to 30" );
        return;
    }
    if ((pitch_float < -30) || (pitch_float > 30) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid pitch input\nValid input range : -30 to 30" );
        return;
    }

    inersiadata.heading = heading;
    inersiadata.roll = roll;
    inersiadata.picth = pitch;

    std::unordered_map<std::string, std::string> data_map =
    {
        {"heading", heading.toStdString()},
        {"roll", roll.toStdString()},
        {"pitch", pitch.toStdString()},
    };

    try
    {
        redisClient->hmset("inersia",data_map.begin(), data_map.end());
        curStatusString = "";
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << Q_FUNC_INFO <<  curStatusString;
    }
}


// ==== GPS ==== //
void FrameOSD::GpsAutoModeUi()
{
    ui->pushButtonGPSApply->setEnabled(false);
    ui->pushButtonGPSApply->setStyleSheet("color: rgb(20, 20, 20);");
    ui->lineEditGpsLat->setEnabled(false);
    ui->lineEditGpsLong->setEnabled(false);

}

void FrameOSD::GpsManualModeUi()
{
    ui->pushButtonGPSApply->setEnabled(true);
    ui->pushButtonGPSApply->setStyleSheet("");
    ui->lineEditGpsLat->setStyleSheet("color:white;");
    ui->lineEditGpsLong->setStyleSheet("color:white;");
    ui->lineEditGpsLat->setEnabled(true);
    ui->lineEditGpsLong->setEnabled(true);
}

void FrameOSD::GpsTimerTimeOut()
{
    QString positionmode;
    try
    {
        auto position_mode = redisClient->get("position_mode");
        positionmode = QString::fromStdString(*position_mode);
        curStatusString = "";
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << Q_FUNC_INFO <<  curStatusString;
    }

    //qDebug() << Q_FUNC_INFO << positionmode;

    if(positionmode == "auto")
    {
        bool status = false;
        try
        {
            status = redisClient->exists("position");
            curStatusString = "";
        }
        catch (Error e)
        {
            curStatusString = e.what();
            qDebug() << Q_FUNC_INFO <<  curStatusString;
        }

        if(status)
        {
            ui->lineEditGpsLat->setStyleSheet("color:rgb(0, 255, 0);");
            ui->lineEditGpsLong->setStyleSheet("color:rgb(0, 255, 0);");

            std::vector<std::string> position;
            position.reserve(2);

            try
            {
                redisClient->hmget("position", {"latitude", "longitude"}, std::back_inserter(position));
                curStatusString = "";

                gpsdata.latitude = QString::fromStdString(position.at(0));
                gpsdata.longitude = QString::fromStdString(position.at(1));

               // ==== Float latitude ====
               float lat_float = gpsdata.latitude.toFloat();
               QString tanda_latitude;
               if (lat_float < 0)
                   tanda_latitude = "S";
               else
                  tanda_latitude = "N";
               lat_float = fabs(lat_float);
               float deg = floor(lat_float);
               float min = lat_float - deg ;
                     min = min * 60;
                     min = floor(min);
               float sec = (lat_float - deg - (min / 60.0)) * 3600.0;

               qDebug() << deg << lat_float << gpsdata.latitude << min << sec << tanda_latitude;

               QString deg_string = QString::number(deg,'f', 0);
               if (deg_string.size() < 2)
                   deg_string.prepend("0");

               QString min_string = QString::number(min,'f', 0);
               if (min_string.size() < 2)
                   min_string.prepend("0");

               QString sec_string = QString::number(sec, 'f', 0);
               if (sec_string.size() <2)
                   sec_string.prepend("0");

               QString latitude_string = deg_string + "-" + min_string + "'" + sec_string + "''" + tanda_latitude;


               // ==== Float Longitude ====
               float long_float = gpsdata.longitude.toFloat();
               QString tanda_longitude;
               if (long_float < 0)
                   tanda_longitude = "W";
               else
                   tanda_longitude = "E";
               long_float = fabs(long_float);
               float degg = floor(long_float);
               float minn = long_float - degg ;
                     minn = minn * 60;
                     minn = floor(minn);
               float secc = (long_float - degg - (minn / 60.0)) * 3600.0;

               qDebug() << degg << long_float << gpsdata.longitude << minn << secc << tanda_longitude;

               QString degg_string = QString::number(degg,'f', 0);
               if (degg_string.size() < 3)
                   degg_string.prepend("0");
               if (degg_string.size() < 3)
                   degg_string.prepend("0");

               QString minn_string = QString::number(minn,'f', 0);
               if (minn_string.size() < 2)
                   minn_string.prepend("0");

               QString secc_string = QString::number(secc, 'f', 0);
               if (secc_string.size() <2)
                   secc_string.prepend("0");

               QString longitude_string = degg_string + "-" + minn_string + "'" + secc_string + "''" + tanda_longitude;

                ui->lineEditGpsLat->setText(latitude_string);
                ui->lineEditGpsLong->setText(longitude_string);
            }
            catch (Error e)
            {
                curStatusString = e.what();
                qDebug() << Q_FUNC_INFO <<  curStatusString;
            }
        }
        else
        {
            ui->lineEditGpsLat->setStyleSheet("color: rgb(255, 0, 0);");
            ui->lineEditGpsLong->setStyleSheet("color: rgb(255, 0, 0);");
        }
    }
//    qDebug() << Q_FUNC_INFO;
}

void FrameOSD::on_comboBoxGPSMode_activated(int index)
{
    if (index)
    {
        try
        {
            redisClient->set("position_mode", "auto");
            curStatusString = "";

            ui->lineEditGpsLat->setEnabled(false);
            ui->lineEditGpsLong->setEnabled(false);

            GpsAutoModeUi();
        }
        catch (Error e)
        {
            curStatusString = e.what();
            qDebug() << Q_FUNC_INFO <<  curStatusString;
        }
    }
    else
    {
        try
        {
            redisClient->set("position_mode", "manual");
            curStatusString = "";

            ui->lineEditGpsLat->setEnabled(true);
            ui->lineEditGpsLong->setEnabled(true);

            GpsManualModeUi();
            try
            {
                bool key_exist = redisClient->exists("position");

                if(!key_exist)
                {
                    std::unordered_map<std::string, std::string> data_map =
                    {
                        {"latitude", "0.0"},
                        {"longitude", "0.0"},
                    };

                    redisClient->hmset("position",data_map.begin(), data_map.end());
                }
                else
                    redisClient->persist("position");

            }
            catch (Error e)
            {
                curStatusString = e.what();
                qDebug() << Q_FUNC_INFO <<  curStatusString;
            }
        }
        catch (Error e)
        {
            curStatusString = e.what();
            qDebug() << Q_FUNC_INFO <<  curStatusString;
        }
    }
}

void FrameOSD::on_pushButtonGPSApply_clicked()
{
    // ==== Float latitude ====
    QString lat_str_trimmed = ui->lineEditGpsLat->text();
    lat_str_trimmed.remove(" ");
    qDebug()<<Q_FUNC_INFO<<"before trimm"<<ui->lineEditGpsLat->text()<<"after trimm"<<lat_str_trimmed;

    QString str = lat_str_trimmed;
    QStringList list1 = str.split("-");

    if(list1.size()<2)
    {
        QMessageBox::critical(this, "Fatal Error Latitude", "Invalid input value" );
        return;
    }

    QString deg = list1.at(0);
    QStringList list2 = list1.at(1).split("'");

    if(list2.size()<4)
    {
        QMessageBox::critical(this, "Fatal Error Latitude", "Invalid input value" );
        return;
    }

    qDebug() << list1 << list2;

    QString min = list2.at(0);
    QString sec = list2.at(1);
    QString sign = list2.at(3);

    qDebug() << deg  <<min <<sec <<sign;

    bool ok;
    float valuedeg = deg.toFloat(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Fatal Error Latitude", "Invalid degree input value.\nValid input : 00-90" );
        return;
    }

    float valuemin = min.toFloat(&ok)/60.0;
    if ((!ok) || (valuemin > 1))
    {
        QMessageBox::critical(this, "Fatal Error Latitude", "Invalid minute input value.\nValid input : 00-60" );
        return;
    }

    float valuesec = sec.toFloat(&ok)/3600.0;
    if ((!ok) || (valuesec > (1.0/60.0)))
    {
        QMessageBox::critical(this, "Fatal Error Latitude", "Invalid second input value.\nValid input : 00-60" );
        return;
    }

    float valueLat = valuedeg+valuemin+valuesec;

    if(sign == "S")
        valueLat *= -1.0;
    else if((sign != "S") && (sign != "N"))
    {
        QMessageBox::critical(this, "Fatal Error Latitude", "Invalid orientation input value.\nValid input : S/N" );
        return;
    }

    if ((valueLat < -90) || (valueLat > 90) )
    {
        QMessageBox::critical(this, "Fatal Error Latitude", "Invalid input : out of range" );
        return;
    }

    qDebug() << Q_FUNC_INFO<<valuedeg << valuemin <<valuesec <<valueLat;

    // ==== Float longitude ====

    QString long_str_trimmed = ui->lineEditGpsLong->text();
    long_str_trimmed.remove(" ");
    qDebug()<<Q_FUNC_INFO<<"before trimm"<<ui->lineEditGpsLong->text()<<"after trimm"<<long_str_trimmed;

    QString str1 = long_str_trimmed;
    QStringList long_list1 = str1.split("-");

    if(long_list1.size()<2)
    {
        QMessageBox::critical(this, "Fatal Error Longitude", "Invalid input value" );
        return;
    }

    QString degg = long_list1.at(0);
    QStringList long_list2 = long_list1.at(1).split("'");

    if(long_list2.size()<4)
    {
        QMessageBox::critical(this, "Fatal Error Longitude", "Invalid input value" );
        return;
    }

    qDebug() << long_list1 << long_list2;

    QString minn = long_list2.at(0);
    QString secc = long_list2.at(1);
    QString signn = long_list2.at(3);

    qDebug() <<degg  <<minn <<secc <<signn;

    bool ok1;
    float valuedegg = degg.toFloat(&ok1);
    if (!ok1)
    {
        QMessageBox::critical(this, "Fatal Error Longitude", "Invalid degree input value.\nValid input : 00-180" );
        return;
    }

    float valueminn = minn.toFloat()/60.0;
    if ((!ok) || (valueminn > 1))
    {
        QMessageBox::critical(this, "Fatal Error Longitude", "Invalid minute input value.\nValid input : 00-60" );
        return;
    }

    float valuesecc = secc.toFloat()/3600.0;
    if ((!ok) || (valuesecc > (1.0/60.0)))
    {
        QMessageBox::critical(this, "Fatal Error Longitude", "Invalid second input value.\nValid input : 00-60" );
        return;
    }

    float valueLong = valuedegg+valueminn+valuesecc;

    if(signn == "W")
        valueLong *= -1.0;
    else if ((signn != "W") && (signn != "E"))
    {
        QMessageBox::critical(this, "Fatal Error Longitude", "Invalid orientation input value.\nValid input : W/E" );
        return;
    }

    if ((valueLong < -180) || (valueLong > 180) )
    {
        QMessageBox::critical(this, "Fatal Error Longitude", "Invalid input : out of range" );
        return;
    }

    qDebug() << Q_FUNC_INFO<<valuedegg << valueminn <<valuesecc <<valueLong;

    QString valueLatStr = QString::number(valueLat);
    QString valueLongStr = QString::number(valueLong);

    qDebug() << valueLatStr << valueLongStr;

    std::unordered_map<std::string, std::string> data_map =
    {
        {"latitude", valueLatStr.toStdString()},
        {"longitude", valueLongStr.toStdString()},
    };

    try
    {
        redisClient->hmset("position",data_map.begin(), data_map.end());
        curStatusString = "";
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << Q_FUNC_INFO <<  curStatusString;
    }
}


// ==== Wind ==== //
void FrameOSD::WindAutoModeUi()
{
    ui->pushButtonWindApply->setEnabled(false);
    ui->pushButtonWindApply->setStyleSheet("color: rgb(20, 20, 20);");
    ui->lineEditWindSpeed->setEnabled(false);
    ui->lineEditWindDir->setEnabled(false);
}

void FrameOSD::WindManualModeUi()
{
    ui->pushButtonWindApply->setEnabled(true);
    ui->pushButtonWindApply->setStyleSheet("");
    ui->lineEditWindDir->setStyleSheet("color:white;");
    ui->lineEditWindSpeed->setStyleSheet("color:white;");
    ui->lineEditWindSpeed->setEnabled(true);
    ui->lineEditWindDir->setEnabled(true);
}

void FrameOSD::WindTimerTimeOut()
{
    QString windmode;
    try
    {
        auto wind_mode = redisClient->get("wind_mode");
        windmode = QString::fromStdString(*wind_mode);
        curStatusString = "";
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << Q_FUNC_INFO <<  curStatusString;
    }

//    qDebug() << Q_FUNC_INFO << windmode;

    if(windmode == "auto")
    {
        bool status = false;
        try
        {
            status = redisClient->exists("wind");
            curStatusString = "";
        }
        catch (Error e)
        {
            qDebug() << "Wind " <<  e.what();
            curStatusString = e.what();
            qDebug() << Q_FUNC_INFO <<  curStatusString;
        }

        if(status)
        {
            ui->lineEditWindDir->setStyleSheet("color:rgb(0, 255, 0);");
            ui->lineEditWindSpeed->setStyleSheet("color:rgb(0, 255, 0);");

            std::vector<std::string> wind;
            wind.reserve(2);

            try
            {
                redisClient->hmget("wind", {"dir", "speed"}, std::back_inserter(wind));

                curStatusString = "";

                winddata.dir = QString::fromStdString(wind.at(0));
                winddata.speed = QString::fromStdString(wind.at(1));

                ui->lineEditWindDir->setText(winddata.dir);
                ui->lineEditWindSpeed->setText(winddata.speed);
            }
            catch (Error e)
            {
                curStatusString = e.what();
                qDebug() << Q_FUNC_INFO <<  curStatusString;
            }
        }
        else
        {
            ui->lineEditWindDir->setStyleSheet("color: rgb(255, 0, 0);");
            ui->lineEditWindSpeed->setStyleSheet("color: rgb(255, 0, 0);");
        }
    }
//    qDebug() << Q_FUNC_INFO;
}

void FrameOSD::on_comboBoxWindMode_activated(int index)
{
    if (index)
    {
        try
        {
            redisClient->set("wind_mode", "auto");
            curStatusString = "";

            ui->lineEditWindDir->setEnabled(false);
            ui->lineEditWindSpeed->setEnabled(false);

            WindAutoModeUi();

        }
        catch (Error e)
        {
            curStatusString = e.what();
            qDebug() << Q_FUNC_INFO <<  curStatusString;
        }
    }
    else
    {
        try
        {
            redisClient->set("wind_mode", "manual");
            curStatusString = "";

            ui->lineEditWindDir->setEnabled(true);
            ui->lineEditWindSpeed->setEnabled(true);

            WindManualModeUi();

            try
            {
                bool key_exist = redisClient->exists("wind");

                if(!key_exist)
                {
                    std::unordered_map<std::string, std::string> data_map =
                    {
                        {"dir", "0.0"},
                        {"speed", "0.0"},
                    };

                    redisClient->hmset("wind",data_map.begin(), data_map.end());
                }
                else
                    redisClient->persist("wind");

            }
            catch (Error e)
            {
                curStatusString = e.what();
                qDebug() << Q_FUNC_INFO <<  curStatusString;
            }
        }
        catch (Error e)
        {
            curStatusString = e.what();
            qDebug() << Q_FUNC_INFO <<  curStatusString;
        }
    }
}

void FrameOSD::on_pushButtonWindApply_clicked()
{
    QString dir = ui->lineEditWindDir->text();
    QString speed = ui->lineEditWindSpeed->text();

    bool ok;

    float dir_float = dir.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error Dir", "Invalid input value\nValid input range : 0 - 360" );
        return;
    }
    if ((dir_float < 0) || (dir_float > 360) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid dir value\nValid input range : 0 - 360" );
        return;
    }

    float speed_float =speed.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error Speed", "Invalid input value\nValid input range : -150 to 150" );
        return;
    }
    if ((speed_float < -150) || (speed_float > 150) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid speed input\nValid input range : -150 to 150" );
        return;
    }

    std::unordered_map<std::string, std::string> data_map =
    {
        {"dir", dir.toStdString()},
        {"speed", speed.toStdString()},
    };

    try
    {
        redisClient->hmset("wind",data_map.begin(), data_map.end());
        curStatusString = "";
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << Q_FUNC_INFO <<  curStatusString;
    }
}


// ==== Weather ==== //
void FrameOSD::WeatherAutoModeUi()
{
    ui->pushButtonWeather->setEnabled(false);
    ui->pushButtonWeather->setStyleSheet("color: rgb(20, 20, 20);");
    ui->lineEditWeatherHumidity->setEnabled(false);
    ui->lineEditWeatherPress->setEnabled(false);
    ui->lineEditWeatherTemp->setEnabled(false);
}

void FrameOSD::WeatherManualModeUi()
{
    ui->pushButtonWeather->setEnabled(true);
    ui->pushButtonWeather->setStyleSheet("");
    ui->lineEditWeatherTemp->setStyleSheet("color:white;");
    ui->lineEditWeatherPress->setStyleSheet("color:white;");
    ui->lineEditWeatherHumidity->setStyleSheet("color:white;");
    ui->lineEditWeatherHumidity->setEnabled(true);
    ui->lineEditWeatherPress->setEnabled(true);
    ui->lineEditWeatherTemp->setEnabled(true);
}

void FrameOSD::WeatherTimerTimeOut()
{
    QString weathermode;
    try
    {
        auto weather_mode = redisClient->get("weather_mode");
        weathermode = QString::fromStdString(*weather_mode);
        curStatusString = "";
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << Q_FUNC_INFO <<  curStatusString;
    }

//    qDebug() << Q_FUNC_INFO << weathermode;

    if(weathermode == "auto")
    {
        bool status = false;
        try
        {
            status = redisClient->exists("weather");
            curStatusString = "";
        }
        catch (Error e)
        {
            curStatusString = e.what();
            qDebug() << Q_FUNC_INFO <<  curStatusString;
        }

        if(status)
        {
            ui->lineEditWeatherTemp->setStyleSheet("color:rgb(0, 255, 0);");
            ui->lineEditWeatherPress->setStyleSheet("color:rgb(0, 255, 0);");
            ui->lineEditWeatherHumidity->setStyleSheet("color:rgb(0, 255, 0);");

            std::vector<std::string> weather;
            weather.reserve(3);

            try
            {
                redisClient->hmget("weather", {"temperature", "pressure", "humidity"}, std::back_inserter(weather));

                weatherdata.temperature = QString::fromStdString(weather.at(0));
                weatherdata.pressure = QString::fromStdString(weather.at(1));
                weatherdata.humidity = QString::fromStdString(weather.at(2));

                ui->lineEditWeatherTemp->setText(weatherdata.temperature);
                ui->lineEditWeatherPress->setText(weatherdata.pressure);
                ui->lineEditWeatherHumidity->setText(weatherdata.humidity);

                curStatusString = "";
            }
            catch (Error e)
            {
                curStatusString = e.what();
                qDebug() << Q_FUNC_INFO <<  curStatusString;
            }
        }
        else
        {
            ui->lineEditWeatherTemp->setStyleSheet("color: rgb(255, 0, 0);");
            ui->lineEditWeatherPress->setStyleSheet("color: rgb(255, 0, 0);");
            ui->lineEditWeatherHumidity->setStyleSheet("color: rgb(255, 0, 0);");
        }
    }
//    qDebug() << Q_FUNC_INFO;
}

void FrameOSD::on_comboBoxWeatherMode_activated(int index)
{
    if (index)
    {
        try
        {
            redisClient->set("weather_mode", "auto");
            curStatusString = "";

            ui->lineEditWeatherTemp->setEnabled(false);
            ui->lineEditWeatherPress->setEnabled(false);
            ui->lineEditWeatherHumidity->setEnabled(false);

            WeatherAutoModeUi();
        }
        catch (Error e)
        {
            curStatusString = e.what();
            qDebug() << Q_FUNC_INFO <<  curStatusString;
        }
    }
    else
    {
        try
        {
            redisClient->set("weather_mode", "manual");
            curStatusString = "";

            ui->lineEditWeatherTemp->setEnabled(true);
            ui->lineEditWeatherPress->setEnabled(true);
            ui->lineEditWeatherHumidity->setEnabled(true);

            WeatherManualModeUi();

            try
            {
                bool key_exist = redisClient->exists("weather");

                if(!key_exist)
                {
                    std::unordered_map<std::string, std::string> data_map =
                    {
                        {"temperature", "0.0"},
                        {"pressure", "0.0"},
                        {"humidity", "0.0"},
                    };

                    redisClient->hmset("weather",data_map.begin(), data_map.end());
                }
                else
                    redisClient->persist("weather");

            }
            catch (Error e)
            {
                curStatusString = e.what();
                qDebug() << Q_FUNC_INFO <<  curStatusString;
            }
        }
        catch (Error e)
        {
            curStatusString = e.what();
            qDebug() << Q_FUNC_INFO <<  curStatusString;
        }
    }
}


void FrameOSD::on_pushButtonWeather_clicked()
{
    QString temperature = ui->lineEditWeatherTemp->text();
    QString pressure = ui->lineEditWeatherPress->text();
    QString humidity = ui->lineEditWeatherHumidity->text();

    bool ok;

    float temperature_float =temperature. toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error Temperature", "Invalid input value\nValid input range : -273 to 273" );
        return;
    }
    if ((temperature_float < -273) || (temperature_float > 273) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid temperature input\nValid input range : -273 to 273" );
        return;
    }

    float pressure_float =pressure. toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error Pressure", "Invalid input value\nValid input range : 100 - 1000" );
        return;
    }
    if ((pressure_float < 100) || (pressure_float > 10000) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid Pressure input\nValid input range : 100 - 1000" );
        return;
    }

    float humidity_float =humidity. toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error Humidity", "Invalid input value\nValid input range : 0 - 100" );
        return;
    }

    if ((humidity_float < 0) || (humidity_float > 100) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid humidity input\nValid input range : 0 - 100" );
        return;
    }

    std::unordered_map<std::string, std::string> data_map =
    {
        {"temperature", temperature.toStdString()},
        {"pressure", pressure.toStdString()},
        {"humidity", humidity.toStdString()},
    };

    try
    {
        redisClient->hmset("weather",data_map.begin(), data_map.end());
        curStatusString = "";
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << Q_FUNC_INFO <<  curStatusString;
    }
}


// ==== Speed ==== //
void FrameOSD::SpeedAutoModeUi()
{
    ui->pushButtonSpeedApply->setEnabled(false);
    ui->pushButtonSpeedApply->setStyleSheet("color: rgb(20, 20, 20);");
    ui->lineEditSpeedCOG->setEnabled(false);
    ui->lineEditSpeedSOG->setEnabled(false);
}

void FrameOSD::SpeedManualModeUi()
{
    ui->pushButtonSpeedApply->setEnabled(true);
    ui->pushButtonSpeedApply->setStyleSheet("");
    ui->lineEditSpeedSOG->setStyleSheet("color:white;");
    ui->lineEditSpeedCOG->setStyleSheet("color:white;");
    ui->lineEditSpeedCOG->setEnabled(true);
    ui->lineEditSpeedSOG->setEnabled(true);
}

void FrameOSD::SpeedTimerTimeOut()
{
    QString speedmode;
    try
    {
        auto speed_mode = redisClient->get("speed_mode");
        speedmode = QString::fromStdString(*speed_mode);
        curStatusString = "";
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << Q_FUNC_INFO <<  curStatusString;
    }

//    qDebug() << Q_FUNC_INFO << speedmode;

    if(speedmode == "auto")
    {
        bool status = false;
        try
        {
            status = redisClient->exists("speed");
            curStatusString = "";
        }
        catch (Error e)
        {
            curStatusString = e.what();
            qDebug() << Q_FUNC_INFO <<  curStatusString;
        }

        if(status)
        {
            ui->lineEditSpeedSOG->setStyleSheet("color:rgb(0, 255, 0);");
            ui->lineEditSpeedCOG->setStyleSheet("color:rgb(0, 255, 0);");

            std::vector<std::string> speed;
            speed.reserve(2);

            try
            {
                redisClient->hmget("speed", {"sog", "cog"}, std::back_inserter(speed));

                speeddata.sog = QString::fromStdString(speed.at(0));
                speeddata.cog = QString::fromStdString(speed.at(1));

                ui->lineEditSpeedSOG->setText(speeddata.sog);
                ui->lineEditSpeedCOG->setText(speeddata.cog);

                curStatusString = "";
            }
            catch (Error e)
            {
                curStatusString = e.what();
                qDebug() << Q_FUNC_INFO <<  curStatusString;
            }
        }
        else
        {
            ui->lineEditSpeedSOG->setStyleSheet("color: rgb(255, 0, 0);");
            ui->lineEditSpeedCOG->setStyleSheet("color: rgb(255, 0, 0);");
        }
    }
//    qDebug() << Q_FUNC_INFO;
}

void FrameOSD::on_comboBoxSpeedMode_activated(int index)
{
    if (index)
    {
        try
        {
            redisClient->set("speed_mode", "auto");
            curStatusString = "";

            ui->lineEditSpeedSOG->setEnabled(false);
            ui->lineEditSpeedCOG->setEnabled(false);

            SpeedAutoModeUi();
        }
        catch (Error e)
        {
            curStatusString = e.what();
            qDebug() << Q_FUNC_INFO <<  curStatusString;
        }
    }
    else
    {
        try
        {
            redisClient->set("speed_mode", "manual");
            curStatusString = "";

            ui->lineEditSpeedSOG->setEnabled(true);
            ui->lineEditSpeedCOG->setEnabled(true);

            SpeedManualModeUi();

            try
            {
                bool key_exist = redisClient->exists("speed");

                if(!key_exist)
                {
                    std::unordered_map<std::string, std::string> data_map =
                    {
                        {"sog", "0.0"},
                        {"cog", "0.0"},
                    };

                    redisClient->hmset("speed",data_map.begin(), data_map.end());
                }
                else
                    redisClient->persist("speed");

            }
            catch (Error e)
            {
                curStatusString = e.what();
                qDebug() << Q_FUNC_INFO <<  curStatusString;
            }
        }
        catch (Error e)
        {
            curStatusString = e.what();
            qDebug() << Q_FUNC_INFO <<  curStatusString;
        }
    }
}

void FrameOSD::on_pushButtonSpeedApply_clicked()
{
    QString sog = ui->lineEditSpeedSOG->text();
    QString cog = ui->lineEditSpeedCOG->text();

    bool ok;

    float sog_float =sog. toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error SOG", "Invalid input value\nValid input range : -150 to 150" );
        return;
    }
    if ((sog_float < -150) || (sog_float > 150) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid sog input\nValid input range : -150 to 150" );
        return;
    }

    float cog_float =cog. toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error COG", "Invalid input value\nValid input range : 0 - 360" );
        return;
    }
    if ((cog_float < 0) || (cog_float > 360) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid cog input\nValid input range : 0 - 360" );
        return;
    }

    std::unordered_map<std::string, std::string> data_map =
    {
        {"sog", sog.toStdString()},
        {"cog", cog.toStdString()},
    };

    try
    {
        redisClient->hmset("speed",data_map.begin(), data_map.end());
        curStatusString = "";
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << Q_FUNC_INFO <<  curStatusString;
    }
}


// ==== Water Speed ==== //
void FrameOSD::WaterSpeedAutoModeUi()
{
    ui->pushButtonWaterApply->setEnabled(false);
    ui->pushButtonWaterApply->setStyleSheet("color: rgb(20, 20, 20);");
    ui->lineEditWaterSOG->setEnabled(false);
    ui->lineEditWaterCOG->setEnabled(false);
}

void FrameOSD::WaterSpeedManualModeUi()
{
    ui->pushButtonWaterApply->setEnabled(true);
    ui->pushButtonWaterApply->setStyleSheet("");
    ui->lineEditWaterSOG->setStyleSheet("color:white;");
    ui->lineEditWaterCOG->setStyleSheet("color:white;");
    ui->lineEditWaterSOG->setEnabled(true);
    ui->lineEditWaterCOG->setEnabled(true);
}

void FrameOSD::WaterSpeedTimerTimeOut()
{
    QString waterspeedmode;
    try
    {
        auto waterspeed_mode = redisClient->get("waterspeed_mode");
        waterspeedmode = QString::fromStdString(*waterspeed_mode);
        curStatusString = "";
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << Q_FUNC_INFO <<  curStatusString;
    }

//    qDebug() << Q_FUNC_INFO << waterspeedmode;

    if(waterspeedmode == "auto")
    {
        bool status = false;
        try
        {
            status = redisClient->exists("waterspeed");
            curStatusString = "";
        }
        catch (Error e)
        {
            curStatusString = e.what();
            qDebug() << Q_FUNC_INFO <<  curStatusString;
        }

        if(status)
        {
            ui->lineEditWaterSOG->setStyleSheet("color:rgb(0, 255, 0);");
            ui->lineEditWaterCOG->setStyleSheet("color:rgb(0, 255, 0);");

            std::vector<std::string> waterspeed;
            waterspeed.reserve(2);

            try
            {
                redisClient->hmget("waterspeed", {"speed", "course"}, std::back_inserter(waterspeed));
                curStatusString = "";

                waterspeeddata.speed = QString::fromStdString(waterspeed.at(0));
                waterspeeddata.course = QString::fromStdString(waterspeed.at(1));

                ui->lineEditWaterSOG->setText(waterspeeddata.speed);
                ui->lineEditWaterCOG->setText(waterspeeddata.course);
            }
            catch (Error e)
            {
                curStatusString = e.what();
                qDebug() << Q_FUNC_INFO <<  curStatusString;
            }
        }
        else
        {
            ui->lineEditWaterSOG->setStyleSheet("color: rgb(255, 0, 0);");
            ui->lineEditWaterCOG->setStyleSheet("color: rgb(255, 0, 0);");
        }
    }
//    qDebug() << Q_FUNC_INFO;
}

void FrameOSD::on_comboBoxWaterMode_activated(int index)
{
    qDebug () <<Q_FUNC_INFO;

    if (index)
    {
        try
        {
            redisClient->set("waterspeed_mode", "auto");
            curStatusString = "";

            ui->lineEditWaterSOG->setEnabled(false);
            ui->lineEditWaterCOG->setEnabled(false);

            WaterSpeedAutoModeUi();
        }
        catch (Error e)
        {
            curStatusString = e.what();
            qDebug() << Q_FUNC_INFO <<  curStatusString;
        }
    }
    else
    {
        try
        {
            redisClient->set("waterspeed_mode", "manual");
            curStatusString = "";

            ui->lineEditWaterSOG->setEnabled(true);
            ui->lineEditWaterCOG->setEnabled(true);

            WaterSpeedManualModeUi();

            try
            {
                bool key_exist = redisClient->exists("waterspeed");

                if(!key_exist)
                {
                    std::unordered_map<std::string, std::string> data_map =
                    {
                        {"speed", "0.0"},
                        {"course", "0.0"},
                    };

                    redisClient->hmset("waterspeed",data_map.begin(), data_map.end());
                }
                else
                    redisClient->persist("waterspeed");

            }
            catch (Error e)
            {
                curStatusString = e.what();
                qDebug() << Q_FUNC_INFO <<  curStatusString;
            }
        }
        catch (Error e)
        {
            curStatusString = e.what();
            qDebug() << Q_FUNC_INFO <<  curStatusString;
        }
    }
}

void FrameOSD::on_pushButtonWaterApply_clicked()
{
    QString speed = ui->lineEditWaterSOG->text();
    QString course = ui->lineEditWaterCOG->text();

    bool ok;

    float speed_float =speed. toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error SOG", "Invalid input value\nValid input range : -150 - 150" );
        return;
    }
    if ((speed_float < -150) || (speed_float > 150) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid cog input\nValid input range : -150 - 150" );
        return;
    }

    float course_float =course. toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error COG", "Invalid input value\nValid input range : 0 - 360" );
        return;
    }
    if ((course_float < 0) || (course_float > 360) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid cog input\nValid input range : 0 - 360" );
        return;
    }

    std::unordered_map<std::string, std::string> data_map =
    {
        {"speed", speed.toStdString()},
        {"course", course.toStdString()},
    };

    try
    {
        redisClient->hmset("waterspeed",data_map.begin(), data_map.end());
        curStatusString = "";
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << Q_FUNC_INFO <<  curStatusString;
    }
}
