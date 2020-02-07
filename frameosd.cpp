#include "frameosd.h"
#include "ui_frameosd.h"

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

    // ==== validator GPS ==== //
   // ui->lineEditGpsLat->setValidator(valiNumber);
   // ui->lineEditGpsLong->setValidator(valiNumber);

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
    ui->lineEditGpsLat->setValidator(valiNumber);
    ui->lineEditGpsLong->setValidator(valiNumber);

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

void FrameOSD::setConfig(QString Config)
{
    this->Config=Config;
    qDebug() <<this->Config;

    redisClient = new Redis(this->Config.toStdString());

    bool status;
    status = false;

    // ==== Gyro ==== //
    try
    {
        status = redisClient->set("inersia_mode", "auto");
    }
    catch (Error e)
    {
        qDebug() << "Gyro " << e.what();
    }

    // ==== GPS ==== //
    try
    {
        status = redisClient->set("position_mode", "auto");
    }
    catch (Error e)
    {
        qDebug() << "GPS" << e.what();
    }

    // ==== Wind ==== //
    try
    {
        status = redisClient->set("wind_mode", "auto");
    }
    catch (Error e)
    {
        qDebug() << "Wind" << e.what();
    }

    // ==== Weather ==== //
    try
    {
        status = redisClient->set("weather_mode", "auto");
    }
    catch (Error e)
    {
        qDebug() << "Weather" << e.what();
    }

    // ==== Speed ==== //
    try
    {
        status = redisClient->set("speed_mode", "auto");
    }
    catch (Error e)
    {
        qDebug() << "Speed" << e.what();
    }

    // ==== Water Speed ==== //
    try
    {
        status = redisClient->set("waterspeed_mode", "auto");
    }
    catch (Error e)
    {
        qDebug() << "Speed" << e.what();
    }
}

void FrameOSD::on_osdTimerTimeOut()
{
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
    }
    catch (Error e)
    {
        qDebug() << "Gyro " <<  e.what();
    }

    qDebug() << Q_FUNC_INFO << inersiamode;

    if(inersiamode == "auto")
    {
        bool status = false;
        try
        {
            status = redisClient->exists("inersia");
        }
        catch (Error e)
        {
            qDebug() << "Gyro " <<  e.what();
        }

        if(status)
        {
            ui->osdGyroHeadingValue->setStyleSheet("color:rgb(0, 255, 0);");
            ui->lineEditGyroRoll->setStyleSheet("color:rgb(0, 255, 0);");
            ui->osdGyroPitchValue->setStyleSheet("color:rgb(0, 255, 0);");

            std::vector<std::string> inersia;
            inersia.reserve(3);
            redisClient->hmget("inersia", {"heading", "roll", "pitch"}, std::back_inserter(inersia));

            inersiadata.heading = QString::fromStdString(inersia.at(0));
            inersiadata.roll = QString::fromStdString(inersia.at(1));
            inersiadata.picth = QString::fromStdString(inersia.at(2));

            qDebug() << Q_FUNC_INFO;

            ui->osdGyroHeadingValue->setText(inersiadata.heading);
            ui->lineEditGyroRoll->setText(inersiadata.roll);
            ui->osdGyroPitchValue->setText(inersiadata.picth);
        }
        else
        {
            ui->osdGyroHeadingValue->setStyleSheet("color: rgb(255, 0, 0);");
            ui->lineEditGyroRoll->setStyleSheet("color: rgb(255, 0, 0);");
            ui->osdGyroPitchValue->setStyleSheet("color: rgb(255, 0, 0);");
        }
    }
    qDebug() << Q_FUNC_INFO;
}

void FrameOSD::on_osdGryoComboBox_activated(int index)
{
    if (index)
    {
        bool status;
        try
        {
            status = redisClient->set("inersia_mode", "auto");
        }
        catch (Error e)
        {
            qDebug() << "Gyro " <<  e.what();
        }

        ui->osdGyroHeadingValue->setEnabled(false);
        ui->lineEditGyroRoll->setEnabled(false);
        ui->osdGyroPitchValue->setEnabled(false);

        GyroAutoModeUi();
    }
    else
    {
        bool status;
        try
        {
            status = redisClient->set("inersia_mode", "manual");
        }
        catch (Error e)
        {
            qDebug() << "Gyro " <<  e.what();
        }

        ui->osdGyroHeadingValue->setEnabled(true);
        ui->lineEditGyroRoll->setEnabled(true);
        ui->osdGyroPitchValue->setEnabled(true);

        GyroManualModeUi();
    }
}

void FrameOSD::on_pushButtonGyroApply_clicked()
{
    QString heading = ui->osdGyroHeadingValue->text();
    QString roll = ui->lineEditGyroRoll->text();
    QString pitch = ui->osdGyroPitchValue->text();

    bool ok;
    float heading_float =heading. toFloat(&ok);
    float roll_float =roll. toFloat(&ok);
    float pitch_float =pitch. toFloat(&ok);

    if ((heading_float < 0) || (heading_float > 300) )
    {
        QMessageBox::critical(this, "Fatal Error Heading", "Invalid input text" );
        return;
    }

    if ((roll_float < -380) || (roll_float > 380) )
    {
        QMessageBox::critical(this, "Fatal Error roll", "Invalid input text" );
        return;
    }

    if ((pitch_float < -380) || (pitch_float > 380) )
    {
        QMessageBox::critical(this, "Fatal Error pitch", "Invalid input text" );
        return;
    }

    std::unordered_map<std::string, std::string> data_map =
    {
        {"heading", heading.toStdString()},
        {"roll", roll.toStdString()},
        {"pitch", pitch.toStdString()},
    };

    try
    {
        redisClient->hmset("inersia",data_map.begin(), data_map.end());
    }
    catch (Error e)
    {
        qDebug() << "Gyro " <<  e.what();
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
    }
    catch (Error e)
    {
        qDebug() << "GPS " <<  e.what();
    }

    qDebug() << Q_FUNC_INFO << positionmode;

    if(positionmode == "auto")
    {
        bool status = false;
        try
        {
            status = redisClient->exists("position");
        }
        catch (Error e)
        {
            qDebug() << "GPS" << e.what();
        }

        if(status)
        {
            ui->lineEditGpsLat->setStyleSheet("color:rgb(0, 255, 0);");
            ui->lineEditGpsLong->setStyleSheet("color:rgb(0, 255, 0);");

            std::vector<std::string> position;
            position.reserve(2);
            redisClient->hmget("position", {"latitude", "longitude"}, std::back_inserter(position));

            gpsdata.latitude = QString::fromStdString(position.at(0));
            gpsdata.longitude = QString::fromStdString(position.at(1));

            qDebug() << Q_FUNC_INFO;

            ui->lineEditGpsLat->setText(gpsdata.latitude);
            ui->lineEditGpsLong->setText(gpsdata.longitude);
        }
        else
        {
            ui->lineEditGpsLat->setStyleSheet("color: rgb(255, 0, 0);");
            ui->lineEditGpsLong->setStyleSheet("color: rgb(255, 0, 0);");
        }
    }
    qDebug() << Q_FUNC_INFO;
}

void FrameOSD::on_comboBoxGPSMode_activated(int index)
{
    if (index)
    {
        bool status;
        try
        {
            status = redisClient->set("position_mode", "auto");
        }
        catch (Error e)
        {
            qDebug() << "GPS " <<  e.what();
        }

        ui->lineEditGpsLat->setEnabled(false);
        ui->lineEditGpsLong->setEnabled(false);

        GpsAutoModeUi();
    }
    else
    {
        bool status;
        try
        {
            status = redisClient->set("position_mode", "manual");
        }
        catch (Error e)
        {
            qDebug() << "GPS" << e.what();
        }

        ui->lineEditGpsLat->setEnabled(true);
        ui->lineEditGpsLong->setEnabled(true);

        GpsManualModeUi();
    }
}

void FrameOSD::on_pushButtonGPSApply_clicked()
{
    QString latitude = ui->lineEditGpsLat->text();
    QString longitude = ui->lineEditGpsLong->text();

    bool ok;
    float latitude_float =latitude. toFloat(&ok);
    float longitude_float =longitude. toFloat(&ok);

    if ((latitude_float < 0) || (latitude_float > 360) )
    {
        QMessageBox::critical(this, "Fatal Error Latitude", "Invalid input text" );
        return;
    }

    if ((longitude_float < 0) || (longitude_float > 360) )
    {
        QMessageBox::critical(this, "Fatal Error Longitude", "Invalid input text" );
        return;
    }

    std::unordered_map<std::string, std::string> data_map =
    {
        {"latitude", latitude.toStdString()},
        {"longitude", longitude.toStdString()},
    };

    try
    {
        redisClient->hmset("position",data_map.begin(), data_map.end());
    }
    catch (Error e)
    {
        qDebug() << "GPS" << e.what();
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
    }
    catch (Error e)
    {
        qDebug() << "Wind " <<  e.what();
    }

    qDebug() << Q_FUNC_INFO << windmode;

    if(windmode == "auto")
    {
        bool status = false;
        try
        {
            status = redisClient->exists("wind");
        }
        catch (Error e)
        {
            qDebug() << "Wind " <<  e.what();
        }

        if(status)
        {
            ui->lineEditWindDir->setStyleSheet("color:rgb(0, 255, 0);");
            ui->lineEditWindSpeed->setStyleSheet("color:rgb(0, 255, 0);");

            std::vector<std::string> wind;
            wind.reserve(2);
            redisClient->hmget("wind", {"dir", "speed"}, std::back_inserter(wind));

            winddata.dir = QString::fromStdString(wind.at(0));
            winddata.speed = QString::fromStdString(wind.at(1));

            qDebug() << Q_FUNC_INFO;

            ui->lineEditWindDir->setText(winddata.dir);
            ui->lineEditWindSpeed->setText(winddata.speed);
        }
        else
        {
            ui->lineEditWindDir->setStyleSheet("color: rgb(255, 0, 0);");
            ui->lineEditWindSpeed->setStyleSheet("color: rgb(255, 0, 0);");
        }
    }
    qDebug() << Q_FUNC_INFO;
}

void FrameOSD::on_comboBoxWindMode_activated(int index)
{
    if (index)
    {
        bool status;
        try
        {
            status = redisClient->set("wind_mode", "auto");
        }
        catch (Error e)
        {
            qDebug() << "Wind " <<  e.what();
        }

        ui->lineEditWindDir->setEnabled(false);
        ui->lineEditWindSpeed->setEnabled(false);

        WindAutoModeUi();
    }
    else
    {
        bool status;
        try
        {
            status = redisClient->set("wind_mode", "manual");
        }
        catch (Error e)
        {
            qDebug() << "Wind " <<  e.what();
        }

        ui->lineEditWindDir->setEnabled(true);
        ui->lineEditWindSpeed->setEnabled(true);

        WindManualModeUi();
    }
}

void FrameOSD::on_pushButtonWindApply_clicked()
{
    QString dir = ui->lineEditWindDir->text();
    QString speed = ui->lineEditWindSpeed->text();

    bool ok;
    float dir_float =dir. toFloat(&ok);
    float speed_float =speed. toFloat(&ok);

    if ((dir_float < 0) || (dir_float > 350) )
    {
        QMessageBox::critical(this, "Fatal Error Direction", "Invalid input text" );
        return;
    }

    if ((speed_float < -150) || (speed_float > 150) )
    {
        QMessageBox::critical(this, "Fatal Error Speed", "Invalid input text" );
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
    }
    catch (Error e)
    {
        qDebug() << "Wind " <<  e.what();
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
    }
    catch (Error e)
    {
        qDebug() << "Weather " <<  e.what();
    }

    qDebug() << Q_FUNC_INFO << weathermode;

    if(weathermode == "auto")
    {
        bool status = false;
        try
        {
            status = redisClient->exists("weather");
        }
        catch (Error e)
        {
            qDebug() << "Weather" << e.what();
        }

        if(status)
        {
            ui->lineEditWeatherTemp->setStyleSheet("color:rgb(0, 255, 0);");
            ui->lineEditWeatherPress->setStyleSheet("color:rgb(0, 255, 0);");
            ui->lineEditWeatherHumidity->setStyleSheet("color:rgb(0, 255, 0);");

            std::vector<std::string> weather;
            weather.reserve(3);
            redisClient->hmget("weather", {"temperature", "pressure", "humidity"}, std::back_inserter(weather));

            weatherdata.temperature = QString::fromStdString(weather.at(0));
            weatherdata.pressure = QString::fromStdString(weather.at(1));
            weatherdata.humidity = QString::fromStdString(weather.at(2));

            qDebug() << Q_FUNC_INFO;

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
    qDebug() << Q_FUNC_INFO;
}

void FrameOSD::on_comboBoxWeatherMode_activated(int index)
{
    if (index)
    {
        bool status;
        try
        {
            status = redisClient->set("weather_mode", "auto");
        }
        catch (Error e)
        {
            qDebug() << "Weather " <<  e.what();
        }

        ui->lineEditWeatherTemp->setEnabled(false);
        ui->lineEditWeatherPress->setEnabled(false);
        ui->lineEditWeatherHumidity->setEnabled(false);

        WeatherAutoModeUi();
    }
    else
    {
        bool status;
        try
        {
            status = redisClient->set("weather_mode", "manual");
        }
        catch (Error e)
        {
            qDebug() << "Weather " <<  e.what();
        }

        ui->lineEditWeatherTemp->setEnabled(true);
        ui->lineEditWeatherPress->setEnabled(true);
        ui->lineEditWeatherHumidity->setEnabled(true);

        WeatherManualModeUi();
    }
}

void FrameOSD::on_pushButtonWeather_clicked()
{
    QString temperature = ui->lineEditWeatherTemp->text();
    QString pressure = ui->lineEditWeatherPress->text();
    QString humidity = ui->lineEditWeatherHumidity->text();

    bool ok;
    float temperature_float =temperature. toFloat(&ok);
    float pressure_float =pressure. toFloat(&ok);
    float humidity_float =humidity. toFloat(&ok);

    if ((temperature_float < -273) || (temperature_float > 273) )
    {
        QMessageBox::critical(this, "Fatal Error Temperature", "Invalid input text" );
        return;
    }

    if ((pressure_float < 100) || (pressure_float > 10000) )
    {
        QMessageBox::critical(this, "Fatal Error Pressure", "Invalid input text" );
        return;
    }

    if ((humidity_float < 0) || (humidity_float > 100) )
    {
        QMessageBox::critical(this, "Fatal Error Humidity", "Invalid input text" );
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
    }
    catch (Error e)
    {
        qDebug() << "Weather " <<  e.what();
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
    }
    catch (Error e)
    {
        qDebug() << "Speed " <<  e.what();
    }

    qDebug() << Q_FUNC_INFO << speedmode;

    if(speedmode == "auto")
    {
        bool status = false;
        try
        {
            status = redisClient->exists("speed");
        }
        catch (Error e)
        {
            qDebug() << "Speed" << e.what();
        }

        if(status)
        {
            ui->lineEditSpeedSOG->setStyleSheet("color:rgb(0, 255, 0);");
            ui->lineEditSpeedCOG->setStyleSheet("color:rgb(0, 255, 0);");

            std::vector<std::string> speed;
            speed.reserve(2);
            redisClient->hmget("speed", {"sog", "cog"}, std::back_inserter(speed));

            speeddata.sog = QString::fromStdString(speed.at(0));
            speeddata.cog = QString::fromStdString(speed.at(1));

            qDebug() << Q_FUNC_INFO;

            ui->lineEditSpeedSOG->setText(speeddata.sog);
            ui->lineEditSpeedCOG->setText(speeddata.cog);
        }
        else
        {
            ui->lineEditSpeedSOG->setStyleSheet("color: rgb(255, 0, 0);");
            ui->lineEditSpeedCOG->setStyleSheet("color: rgb(255, 0, 0);");
        }
    }
    qDebug() << Q_FUNC_INFO;
}

void FrameOSD::on_comboBoxSpeedMode_activated(int index)
{
    if (index)
    {
        bool status;
        try
        {
            status = redisClient->set("speed_mode", "auto");
        }
        catch (Error e)
        {
            qDebug() << "Speed " <<  e.what();
        }

        ui->lineEditSpeedSOG->setEnabled(false);
        ui->lineEditSpeedCOG->setEnabled(false);

        SpeedAutoModeUi();
    }
    else
    {
        bool status;
        try
        {
            status = redisClient->set("speed_mode", "manual");
        }
        catch (Error e)
        {
            qDebug() << "Speed " <<  e.what();
        }

        ui->lineEditSpeedSOG->setEnabled(true);
        ui->lineEditSpeedCOG->setEnabled(true);

        SpeedManualModeUi();
    }
}

void FrameOSD::on_pushButtonSpeedApply_clicked()
{
    QString sog = ui->lineEditSpeedSOG->text();
    QString cog = ui->lineEditSpeedCOG->text();

    bool ok;
    float sog_float =sog. toFloat(&ok);
    float cog_float =cog. toFloat(&ok);

    if ((sog_float < -150) || (sog_float > 150) )
    {
        QMessageBox::critical(this, "Fatal Error SOG", "Invalid input text" );
        return;
    }

    if ((cog_float < 0) || (cog_float > 350) )
    {
        QMessageBox::critical(this, "Fatal Error COG", "Invalid input text" );
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
    }
    catch (Error e)
    {
        qDebug() << "Speed " <<  e.what();
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
    }
    catch (Error e)
    {
        qDebug() << "WaterSpeed " <<  e.what();
    }

    qDebug() << Q_FUNC_INFO << waterspeedmode;

    if(waterspeedmode == "auto")
    {
        bool status = false;
        try
        {
            status = redisClient->exists("waterspeed");
        }
        catch (Error e)
        {
            qDebug() << "WaterSpeed" << e.what();
        }

        if(status)
        {
            ui->lineEditWaterSOG->setStyleSheet("color:rgb(0, 255, 0);");
            ui->lineEditWaterCOG->setStyleSheet("color:rgb(0, 255, 0);");

            std::vector<std::string> waterspeed;
            waterspeed.reserve(2);
            redisClient->hmget("waterspeed", {"speed", "course"}, std::back_inserter(waterspeed));

            waterspeeddata.speed = QString::fromStdString(waterspeed.at(0));
            waterspeeddata.course = QString::fromStdString(waterspeed.at(1));

            qDebug() << Q_FUNC_INFO;

            ui->lineEditWaterSOG->setText(waterspeeddata.speed);
            ui->lineEditWaterCOG->setText(waterspeeddata.course);
        }
        else
        {
            ui->lineEditWaterSOG->setStyleSheet("color: rgb(255, 0, 0);");
            ui->lineEditWaterCOG->setStyleSheet("color: rgb(255, 0, 0);");
        }
    }
    qDebug() << Q_FUNC_INFO;
}

void FrameOSD::on_comboBoxWaterMode_activated(int index)
{
    qDebug () <<Q_FUNC_INFO;

    if (index)
    {
        bool status;
        status = false;
        try
        {
            status = redisClient->set("waterspeed_mode", "auto");
        }
        catch (Error e)
        {
            qDebug() << "WaterSpeed " <<  e.what();
        }

        ui->lineEditWaterSOG->setEnabled(false);
        ui->lineEditWaterCOG->setEnabled(false);

        WaterSpeedAutoModeUi();
    }
    else
    {
        bool status;
        status = false;
        try
        {
            status = redisClient->set("waterspeed_mode", "manual");
        }
        catch (Error e)
        {
            qDebug() << "WaterSpeed " <<  e.what();
        }

        ui->lineEditWaterSOG->setEnabled(true);
        ui->lineEditWaterCOG->setEnabled(true);

        WaterSpeedManualModeUi();
    }
}

void FrameOSD::on_pushButtonWaterApply_clicked()
{
    QString speed = ui->lineEditWaterSOG->text();
    QString course = ui->lineEditWaterCOG->text();

    bool ok;
    float speed_float =speed. toFloat(&ok);
    float course_float =course. toFloat(&ok);

    if ((speed_float < -150) || (speed_float > 150) )
    {
        QMessageBox::critical(this, "Fatal Error Speed", "Invalid input text" );
        return;
    }

    if ((course_float < 0) || (course_float > 350) )
    {
        QMessageBox::critical(this, "Fatal Error Course", "Invalid input text" );
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
    }
    catch (Error e)
    {
        qDebug() << "WaterSpeed " <<  e.what();
    }
}
