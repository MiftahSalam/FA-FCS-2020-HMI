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
    ui->lineEditGyroHeading->setValidator(valiNumber);
    ui->lineEditGyroPitch->setValidator(valiNumber);
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
        qDebug() << "Water Speed" << curStatusString;
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
    ui->lineEditGyroHeading->setEnabled(false);
    ui->lineEditGyroPitch->setEnabled(false);
    ui->lineEditGyroRoll->setEnabled(false);
}

void FrameOSD::GyroManualModeUi()
{
    ui->pushButtonGyroApply->setEnabled(true);
    ui->pushButtonGyroApply->setStyleSheet("");
    ui->lineEditGyroHeading->setStyleSheet("color:white;");
    ui->lineEditGyroRoll->setStyleSheet("color:white;");
    ui->lineEditGyroPitch->setStyleSheet("color:white;");
    ui->lineEditGyroHeading->setEnabled(true);
    ui->lineEditGyroPitch->setEnabled(true);
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
        //qDebug() << Q_FUNC_INFO <<  curStatusString;
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
            std::vector<std::string> inersia;
            inersia.reserve(4);

            try
            {
                redisClient->hmget("inersia", {"heading", "roll", "pitch", "status"}, std::back_inserter(inersia));

                curStatusString = "";

                inersiadata.heading = QString::fromStdString(inersia.at(0));
                inersiadata.roll = QString::fromStdString(inersia.at(1));
                inersiadata.picth = QString::fromStdString(inersia.at(2));
                inersiadata.status = QString::fromStdString(inersia.at(3));

                ui->lineEditGyroHeading->setToolTip(inersiadata.status);
                ui->lineEditGyroRoll->setToolTip(inersiadata.status);
                ui->lineEditGyroPitch->setToolTip(inersiadata.status);

                if(inersiadata.status.isEmpty())
                {
                    ui->lineEditGyroHeading->setStyleSheet("color:rgb(0, 255, 0);");
                    ui->lineEditGyroRoll->setStyleSheet("color:rgb(0, 255, 0);");
                    ui->lineEditGyroPitch->setStyleSheet("color:rgb(0, 255, 0);");

                    ui->lineEditGyroHeading->setText(inersiadata.heading);
                    ui->lineEditGyroRoll->setText(inersiadata.roll);
                    ui->lineEditGyroPitch->setText(inersiadata.picth);
                }
                else
                {
                    ui->lineEditGyroHeading->setStyleSheet("color: rgb(255, 0, 0);");
                    ui->lineEditGyroRoll->setStyleSheet("color: rgb(255, 0, 0);");
                    ui->lineEditGyroPitch->setStyleSheet("color: rgb(255, 0, 0);");
                }

            }
            catch (Error e)
            {
                inersiadata.heading = "-";
                curStatusString = e.what();
                qDebug() << Q_FUNC_INFO <<  curStatusString;
            }
        }
        else
        {
//            qDebug() << Q_FUNC_INFO <<  "no inersia";
            inersiadata.heading = "-";
            ui->lineEditGyroHeading->setStyleSheet("color: rgb(255, 0, 0);");
            ui->lineEditGyroRoll->setStyleSheet("color: rgb(255, 0, 0);");
            ui->lineEditGyroPitch->setStyleSheet("color: rgb(255, 0, 0);");
        }
    }
}

void FrameOSD::on_osdGryoComboBox_activated(int index)
{
    if (index)
    {
        try
        {
            redisClient->set("inersia_mode", "auto");
            curStatusString = "";

            ui->lineEditGyroHeading->setEnabled(false);
            ui->lineEditGyroRoll->setEnabled(false);
            ui->lineEditGyroPitch->setEnabled(false);

            GyroAutoModeUi();

            try
            {
                redisClient->del("inersia");
            }
            catch (Error e)
            {
                qDebug() << Q_FUNC_INFO <<"cannot del position key";
            }
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

            ui->lineEditGyroHeading->setEnabled(true);
            ui->lineEditGyroRoll->setEnabled(true);
            ui->lineEditGyroPitch->setEnabled(true);

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

                inersiadata.heading = ui->lineEditGyroHeading->text();

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
    QString heading = ui->lineEditGyroHeading->text();
    QString roll = ui->lineEditGyroRoll->text();
    QString pitch = ui->lineEditGyroPitch->text();

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
            std::vector<std::string> position;
            position.reserve(2);

            try
            {
                redisClient->hmget("position", {"latitude", "longitude", "status"}, std::back_inserter(position));

                curStatusString = "";

                gpsdata.latitude = QString::fromStdString(position.at(0));
                gpsdata.longitude = QString::fromStdString(position.at(1));
                gpsdata.status = QString::fromStdString(position.at(2));


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

//               qDebug() << deg << lat_float << gpsdata.latitude << min << sec << tanda_latitude;

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

//               qDebug() << degg << long_float << gpsdata.longitude << minn << secc << tanda_longitude;

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

               ui->lineEditGpsLat->setToolTip(gpsdata.status);
               ui->lineEditGpsLong->setToolTip(gpsdata.status);

               if(gpsdata.status.isEmpty())
               {
                   ui->lineEditGpsLat->setStyleSheet("color:rgb(0, 255, 0);");
                   ui->lineEditGpsLong->setStyleSheet("color:rgb(0, 255, 0);");

                   ui->lineEditGpsLat->setText(latitude_string);
                   ui->lineEditGpsLong->setText(longitude_string);
               }
               else
               {
                   ui->lineEditGpsLat->setStyleSheet("color: rgb(255, 0, 0);");
                   ui->lineEditGpsLong->setStyleSheet("color: rgb(255, 0, 0);");
               }

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

            try
            {
                redisClient->del("position");
            }
            catch (Error e)
            {
                qDebug() << Q_FUNC_INFO <<"cannot del position key";
            }

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
                        {"status", ""},
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

    QString str = lat_str_trimmed;
    QStringList list1 = str.split("-");

    if(list1.size()<2)
    {
        QMessageBox::critical(this, "Fatal Error Latitude", "Invalid input value" );
        return;
    }

    QString deg = list1.at(0);
    QStringList list2 = list1.at(1).split("'");

    if(list2.size()!=4)
    {
        QMessageBox::critical(this, "Fatal Error Latitude", "Invalid input value" );
        return;
    }

    QString min = list2.at(0);
    QString sec = list2.at(1);
    QString sign = list2.at(3);

//    qDebug() << deg  <<min <<sec <<sign;

    bool ok;
    float valuedeg = deg.toFloat(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Fatal Error Latitude", "Invalid degree input value.\nValid input : 00-90" );
        return;
    }

    float valuemin = min.toFloat(&ok)/60.0;
    if ((!ok) || (valuemin >= 1))
    {
        QMessageBox::critical(this, "Fatal Error Latitude", "Invalid minute input value.\nValid input : 00-59" );
        return;
    }

    float valuesec = sec.toFloat(&ok)/3600.0;
    if ((!ok) || (valuesec > (1.0/60.0)))
    {
        QMessageBox::critical(this, "Fatal Error Latitude", "Invalid second input value.\nValid input : 00-59" );
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
        QMessageBox::critical(this, "Fatal Error Latitude", "Invalid input : out of range.\nValid input : 00-00'00'' - 90-00'00''");
        return;
    }

//    qDebug() << Q_FUNC_INFO<<valuedeg << valuemin <<valuesec <<valueLat;

    // ==== Float longitude ====

    QString long_str_trimmed = ui->lineEditGpsLong->text();
    long_str_trimmed.remove(" ");

    QString str1 = long_str_trimmed;
    QStringList long_list1 = str1.split("-");

    if(long_list1.size()<2)
    {
        QMessageBox::critical(this, "Fatal Error Longitude", "Invalid input value" );
        return;
    }

    QString degg = long_list1.at(0);
    QStringList long_list2 = long_list1.at(1).split("'");

    if(long_list2.size()!=4)
    {
        QMessageBox::critical(this, "Fatal Error Longitude", "Invalid input value" );
        return;
    }

    QString minn = long_list2.at(0);
    QString secc = long_list2.at(1);
    QString signn = long_list2.at(3);

//    qDebug() <<degg  <<minn <<secc <<signn;

    bool ok1;
    float valuedegg = degg.toFloat(&ok1);
    if (!ok1)
    {
        QMessageBox::critical(this, "Fatal Error Longitude", "Invalid degree input value.\nValid input : 00-180" );
        return;
    }

    float valueminn = minn.toFloat()/60.0;
    if ((!ok) || (valueminn >= 1))
    {
        QMessageBox::critical(this, "Fatal Error Longitude", "Invalid minute input value.\nValid input : 00-59" );
        return;
    }

    float valuesecc = secc.toFloat()/3600.0;
    if ((!ok) || (valuesecc > (1.0/60.0)))
    {
        QMessageBox::critical(this, "Fatal Error Longitude", "Invalid second input value.\nValid input : 00-59" );
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
        QMessageBox::critical(this, "Fatal Error Longitude", "Invalid input : out of range.\nValid input : 00-00'00'' - 180-00'00''" );
        return;
    }

//    qDebug() << Q_FUNC_INFO<<valuedegg << valueminn <<valuesecc <<valueLong;

    QString valueLatStr = QString::number(valueLat);
    QString valueLongStr = QString::number(valueLong);

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
            std::vector<std::string> speed;
            speed.reserve(2);

            try
            {
                redisClient->hmget("speed", {"SOG", "COG","status"}, std::back_inserter(speed));

                curStatusString = "";

                speeddata.sog = QString::fromStdString(speed.at(0));
                speeddata.cog = QString::fromStdString(speed.at(1));
                speeddata.status = QString::fromStdString(speed.at(2));

                ui->lineEditSpeedSOG->setToolTip(speeddata.status);
                ui->lineEditSpeedCOG->setToolTip(speeddata.status);

                if(speeddata.status.isEmpty())
                {
                    ui->lineEditSpeedSOG->setStyleSheet("color:rgb(0, 255, 0);");
                    ui->lineEditSpeedCOG->setStyleSheet("color:rgb(0, 255, 0);");

                    ui->lineEditSpeedSOG->setText(speeddata.sog);
                    ui->lineEditSpeedCOG->setText(speeddata.cog);
                }
                else
                {
                    ui->lineEditSpeedSOG->setStyleSheet("color: rgb(255, 0, 0);");
                    ui->lineEditSpeedCOG->setStyleSheet("color: rgb(255, 0, 0);");
                }
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

            try
            {
                redisClient->del("speed");
            }
            catch (Error e)
            {
                qDebug() << Q_FUNC_INFO <<"cannot del position key";
            }
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
                        {"SOG", "0.0"},
                        {"COG", "0.0"},
                        {"status", ""},

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
        QMessageBox::critical(this, "Fatal Error Speed SOG", "Invalid input value\nValid input range : 0 to 150" );
        return;
    }
    if ((sog_float < 0) || (sog_float > 150) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid speed sog input\nValid input range : 0 to 150" );
        return;
    }

    float cog_float =cog. toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error Speed COG", "Invalid input value\nValid input range : 0 - 360" );
        return;
    }
    if ((cog_float < 0) || (cog_float > 360) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid speed cog input\nValid input range : 0 - 360" );
        return;
    }

    std::unordered_map<std::string, std::string> data_map =
    {
        {"SOG", sog.toStdString()},
        {"COG", cog.toStdString()},
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
            std::vector<std::string> waterspeed;
            waterspeed.reserve(2);

            try
            {
                redisClient->hmget("waterspeed", {"speed", "course", "status"}, std::back_inserter(waterspeed));

                curStatusString = "";

                waterspeeddata.speed = QString::fromStdString(waterspeed.at(0));
                waterspeeddata.course = QString::fromStdString(waterspeed.at(1));
                waterspeeddata.status = QString::fromStdString(waterspeed.at(2));

                ui->lineEditWaterSOG->setToolTip(waterspeeddata.status);
                ui->lineEditWaterCOG->setToolTip(waterspeeddata.status);

                if(waterspeeddata.status.isEmpty())
                {
                    ui->lineEditWaterSOG->setStyleSheet("color:rgb(0, 255, 0);");
                    ui->lineEditWaterCOG->setStyleSheet("color:rgb(0, 255, 0);");

                    ui->lineEditWaterSOG->setText(waterspeeddata.speed);
                    ui->lineEditWaterCOG->setText(waterspeeddata.course);
                }
                else
                {
                    ui->lineEditWaterSOG->setStyleSheet("color: rgb(255, 0, 0);");
                    ui->lineEditWaterCOG->setStyleSheet("color: rgb(255, 0, 0);");
                }
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

            try
            {
                redisClient->del("waterspeed");
            }
            catch (Error e)
            {
                qDebug() << Q_FUNC_INFO <<"cannot del position key";
            }
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
                        {"status", ""},
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
        QMessageBox::critical(this, "Fatal Error Water Speed", "Invalid input value\nValid input range : -150 - 150" );
        return;
    }
    if ((speed_float < -150) || (speed_float > 150) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid water speed input\nValid input range : -150 - 150" );
        return;
    }

    float course_float =course. toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error Water Course", "Invalid input value\nValid input range : 0 - 360" );
        return;
    }
    if ((course_float < 0) || (course_float > 360) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid water course input\nValid input range : 0 - 360" );
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
            std::vector<std::string> wind;
            wind.reserve(3);

            try
            {
                redisClient->hmget("wind", {"dir", "speed", "status"}, std::back_inserter(wind));

                curStatusString = "";

                winddata.dir = QString::fromStdString(wind.at(0));
                winddata.speed = QString::fromStdString(wind.at(1));
                winddata.status = QString::fromStdString(wind.at(2));

                ui->lineEditWindDir->setToolTip(winddata.status);
                ui->lineEditWindSpeed->setToolTip(winddata.status);

                if(winddata.status.isEmpty())
                {
                    ui->lineEditWindDir->setStyleSheet("color:rgb(0, 255, 0);");
                    ui->lineEditWindSpeed->setStyleSheet("color:rgb(0, 255, 0);");

                    ui->lineEditWindDir->setText(winddata.dir);
                    ui->lineEditWindSpeed->setText(winddata.speed);
                }
                else
                {
                    ui->lineEditWindDir->setStyleSheet("color: rgb(255, 0, 0);");
                    ui->lineEditWindSpeed->setStyleSheet("color: rgb(255, 0, 0);");
                }
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

            try
            {
                redisClient->del("wind");
            }
            catch (Error e)
            {
                qDebug() << Q_FUNC_INFO <<"cannot del position key";
            }
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

            QString SpeedVal;
            QString DirVal;
            SpeedVal = QString::fromStdString(redisClient->hget("wind","speed").value());
            DirVal = QString::fromStdString(redisClient->hget("wind","dir").value());

            ui->lineEditWindSpeed->setText(SpeedVal);
            ui->lineEditWindDir->setText(DirVal);
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
                        {"status", ""},
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
        QMessageBox::critical(this, "Fatal Error Wind Direction", "Invalid input value\nValid input range : 0 - 360" );
        return;
    }
    if ((dir_float < 0) || (dir_float > 360) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid wind direction value\nValid input range : 0 - 360" );
        return;
    }

    float speed_float =speed.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error Wind Speed", "Invalid input value\nValid input range : -150 to 150" );
        return;
    }
    if ((speed_float < -150) || (speed_float > 150) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid wind speed input\nValid input range : -150 to 150" );
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
            std::vector<std::string> weather;
            weather.reserve(4);

            try
            {
                redisClient->hmget("weather", {"temperature", "pressure", "humidity", "status"}, std::back_inserter(weather));

                curStatusString = "";

                weatherdata.temperature = QString::fromStdString(weather.at(0));
                weatherdata.pressure = QString::fromStdString(weather.at(1));
                weatherdata.humidity = QString::fromStdString(weather.at(2));
                weatherdata.status = QString::fromStdString(weather.at(3));
//                qDebug() <<"temperature" <<weatherdata.temperature
//                         <<"pressure" <<weatherdata.pressure
//                         <<"humidity" <<weatherdata.humidity
//                         <<"status" <<weatherdata.status;

                ui->lineEditWeatherTemp->setToolTip(weatherdata.status);
                ui->lineEditWeatherPress->setToolTip(weatherdata.status);
                ui->lineEditWeatherHumidity->setToolTip(weatherdata.status);

                if(weatherdata.status.isEmpty())
                {
                    ui->lineEditWeatherTemp->setStyleSheet("color:rgb(0, 255, 0);");
                    ui->lineEditWeatherPress->setStyleSheet("color:rgb(0, 255, 0);");
                    ui->lineEditWeatherHumidity->setStyleSheet("color:rgb(0, 255, 0);");

                    ui->lineEditWeatherTemp->setText(weatherdata.temperature);
                    ui->lineEditWeatherPress->setText(weatherdata.pressure);
                    ui->lineEditWeatherHumidity->setText(weatherdata.humidity);
                }
                else
                {
                    ui->lineEditWeatherTemp->setStyleSheet("color: rgb(255, 0, 0);");
                    ui->lineEditWeatherPress->setStyleSheet("color: rgb(255, 0, 0);");
                    ui->lineEditWeatherHumidity->setStyleSheet("color: rgb(255, 0, 0);");
                }
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

            try
            {
                redisClient->del("weather");
            }
            catch (Error e)
            {
                qDebug() << Q_FUNC_INFO <<"cannot del position key";
            }
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
                        {"status", ""},
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
        QMessageBox::critical(this, "Fatal Error Weather Temperature", "Invalid input value\nValid input range : -273 to 273" );
        return;
    }
    if ((temperature_float < -273) || (temperature_float > 273) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid weather temperature input\nValid input range : -273 to 273" );
        return;
    }

    float pressure_float =pressure. toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error Weather Pressure", "Invalid input value\nValid input range : 100 - 1000" );
        return;
    }
    if ((pressure_float < 100) || (pressure_float > 10000) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid weather pressure input\nValid input range : 100 - 10000" );
        return;
    }

    float humidity_float =humidity. toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error Weather Humidity", "Invalid input value\nValid input range : 0 - 100" );
        return;
    }

    if ((humidity_float < 0) || (humidity_float > 100) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid Weather humidity input\nValid input range : 0 - 100" );
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
