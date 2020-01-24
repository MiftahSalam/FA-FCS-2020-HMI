#include "frameosd.h"
#include "ui_frameosd.h"

FrameOSD::FrameOSD(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameOSD)
{
    ui->setupUi(this);

    redisClient = new Redis("tcp://127.0.0.1:6379");

    // ==== Gyro ==== //
    redisClient->set("inersia_mode", "auto");
    GyroAutoModeUi();
    ui->osdGryoComboBox->setCurrentIndex(1);

    // ==== GPS ==== //
    redisClient->set("position_mode", "auto");
    GpsAutoModeUi();
    ui->comboBoxGPSMode->setCurrentIndex(1);

    // ==== Wind ==== //
    redisClient->set("wind_mode", "auto");
    WindAutoModeUi();
    ui->comboBoxWindMode->setCurrentIndex(1);

    // ==== Weather ==== //
    redisClient->set("weather_mode", "auto");
    WeatherAutoModeUi();
    ui->comboBoxWeatherMode->setCurrentIndex(1);

    // ==== Speed ==== //
    redisClient->set("speed_mode", "auto");
    SpeedAutoModeUi();
    ui->comboBoxSpeedMode->setCurrentIndex(1);

    // ==== Water Speed ==== //
    redisClient->set("waterspeed_mode", "auto");
    WaterSpeedAutoModeUi();
    ui->comboBoxWaterMode->setCurrentIndex(1);

}

FrameOSD::~FrameOSD()
{
    delete ui;
}


// ==== Gyro ==== //

void FrameOSD::GyroAutoModeUi()
{
    ui->pushButtonGyroApply->setEnabled(false);
    ui->pushButtonGyroApply->setStyleSheet("color: rgb(20, 20, 20);");
}

void FrameOSD::GyroManualModeUi()
{
    ui->pushButtonGyroApply->setEnabled(true);
    ui->pushButtonGyroApply->setStyleSheet("");
    ui->osdGyroHeadingValue->setStyleSheet("color:white;");
    ui->lineEditGyroRoll->setStyleSheet("color:white;");
    ui->osdGyroPitchValue->setStyleSheet("color:white;");
}

void FrameOSD::GyroTimerTimeOut()
{
    auto inersia_mode = redisClient->get("inersia_mode");
    QString inersiamode = QString::fromStdString(*inersia_mode);
    qDebug() << Q_FUNC_INFO << inersiamode;

    if(inersiamode == "auto")
    {
        if(redisClient->exists("inersia"))
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
    qDebug () <<Q_FUNC_INFO;

    if (index)
    {
        redisClient->set("inersia_mode", "auto");
        ui->osdGyroHeadingValue->setEnabled(false);
        ui->lineEditGyroRoll->setEnabled(false);
        ui->osdGyroPitchValue->setEnabled(false);

        GyroAutoModeUi();
    }
    else
    {
        redisClient->set("inersia_mode", "manual");
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

    std::unordered_map<std::string, std::string> data_map =
    {
        {"heading", heading.toStdString()},
        {"roll", roll.toStdString()},
        {"pitch", pitch.toStdString()},
    };

    redisClient->hmset("inersia",data_map.begin(), data_map.end());
}

// ==== GPS ==== //

void FrameOSD::GpsAutoModeUi()
{
    ui->pushButtonGPSApply->setEnabled(false);
    ui->pushButtonGPSApply->setStyleSheet("color: rgb(20, 20, 20);");
}

void FrameOSD::GpsManualModeUi()
{
    ui->pushButtonGPSApply->setEnabled(true);
    ui->pushButtonGPSApply->setStyleSheet("");
    ui->lineEditGpsLat->setStyleSheet("color:white;");
    ui->lineEditGpsLong->setStyleSheet("color:white;");
}

void FrameOSD::GpsTimerTimeOut()
{
    auto position_mode = redisClient->get("position_mode");
    QString positionmode = QString::fromStdString(*position_mode);
    qDebug() << Q_FUNC_INFO << positionmode;

    if(positionmode == "auto")
    {
        if(redisClient->exists("position"))
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
    auto position_mode = redisClient->get("position_mode");
    qDebug() <<Q_FUNC_INFO;

    if (index)
    {
        redisClient->set("position_mode", "auto");
        ui->lineEditGpsLat->setEnabled(false);
        ui->lineEditGpsLong->setEnabled(false);
        GpsAutoModeUi();
    }
    else
    {
        redisClient->set("position_mode", "manual");
        ui->lineEditGpsLat->setEnabled(true);
        ui->lineEditGpsLong->setEnabled(true);

        GpsManualModeUi();
    }
}

void FrameOSD::on_pushButtonGPSApply_clicked()
{
    QString latitude = ui->lineEditGpsLat->text();
    QString longitude = ui->lineEditGpsLong->text();

    std::unordered_map<std::string, std::string> data_map =
    {
        {"latitude", latitude.toStdString()},
        {"longitude", longitude.toStdString()},
    };

    redisClient->hmset("position",data_map.begin(), data_map.end());
}


// ==== Wind ==== //

void FrameOSD::WindAutoModeUi()
{
    ui->pushButtonWindApply->setEnabled(false);
    ui->pushButtonWindApply->setStyleSheet("color: rgb(20, 20, 20);");
}

void FrameOSD::WindManualModeUi()
{
    ui->pushButtonWindApply->setEnabled(true);
    ui->pushButtonWindApply->setStyleSheet("");
    ui->lineEditWindDir->setStyleSheet("color:white;");
    ui->lineEditWindSpeed->setStyleSheet("color:white;");
}

void FrameOSD::WindTimerTimeOut()
{
    auto wind_mode = redisClient->get("wind_mode");
    QString windmode = QString::fromStdString(*wind_mode);
    qDebug() << Q_FUNC_INFO << windmode;

    if(windmode == "auto")
    {
        if(redisClient->exists("wind"))
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
    qDebug () <<Q_FUNC_INFO;

    if (index)
    {
        redisClient->set("wind_mode", "auto");
        ui->lineEditWindDir->setEnabled(false);
        ui->lineEditWindSpeed->setEnabled(false);

        WindAutoModeUi();
    }
    else
    {
        redisClient->set("wind_mode", "manual");
        ui->lineEditWindDir->setEnabled(true);
        ui->lineEditWindSpeed->setEnabled(true);

        WindManualModeUi();
    }
}

void FrameOSD::on_pushButtonWindApply_clicked()
{
    QString dir = ui->lineEditWindDir->text();
    QString speed = ui->lineEditWindSpeed->text();

    std::unordered_map<std::string, std::string> data_map =
    {
        {"dir", dir.toStdString()},
        {"speed", speed.toStdString()},
    };

    redisClient->hmset("wind",data_map.begin(), data_map.end());
}


// ==== Weather ==== //

void FrameOSD::WeatherAutoModeUi()
{
    ui->pushButtonWeather->setEnabled(false);
    ui->pushButtonWeather->setStyleSheet("color: rgb(20, 20, 20);");
}

void FrameOSD::WeatherManualModeUi()
{
    ui->pushButtonWeather->setEnabled(true);
    ui->pushButtonWeather->setStyleSheet("");
    ui->lineEditWeatherTemp->setStyleSheet("color:white;");
    ui->lineEditWeatherPress->setStyleSheet("color:white;");
    ui->lineEditWeatherHumidity->setStyleSheet("color:white;");
}

void FrameOSD::WeatherTimerTimeOut()
{
    auto weather_mode = redisClient->get("weather_mode");
    QString weathermode = QString::fromStdString(*weather_mode);
    qDebug() << Q_FUNC_INFO << weathermode;

    if(weathermode == "auto")
    {
        if(redisClient->exists("weather"))
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
    qDebug () <<Q_FUNC_INFO;

    if (index)
    {
        redisClient->set("weather_mode", "auto");
        ui->lineEditWeatherTemp->setEnabled(false);
        ui->lineEditWeatherPress->setEnabled(false);
        ui->lineEditWeatherHumidity->setEnabled(false);

        WeatherAutoModeUi();
    }
    else
    {
        redisClient->set("weather_mode", "manual");
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

    std::unordered_map<std::string, std::string> data_map =
    {
        {"temperature", temperature.toStdString()},
        {"pressure", pressure.toStdString()},
        {"humidity", humidity.toStdString()},
    };

    redisClient->hmset("weather",data_map.begin(), data_map.end());
}


// ==== Speed ==== //

void FrameOSD::SpeedAutoModeUi()
{
    ui->pushButtonSpeedApply->setEnabled(false);
    ui->pushButtonSpeedApply->setStyleSheet("color: rgb(20, 20, 20);");
}

void FrameOSD::SpeedManualModeUi()
{
    ui->pushButtonSpeedApply->setEnabled(true);
    ui->pushButtonSpeedApply->setStyleSheet("");
    ui->lineEditSpeedSOG->setStyleSheet("color:white;");
    ui->lineEditSpeedCOG->setStyleSheet("color:white;");
}

void FrameOSD::SpeedTimerTimeOut()
{
    auto speed_mode = redisClient->get("speed_mode");
    QString speedmode = QString::fromStdString(*speed_mode);
    qDebug() << Q_FUNC_INFO << speedmode;

    if(speedmode == "auto")
    {
        if(redisClient->exists("speed"))
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
    qDebug () <<Q_FUNC_INFO;

    if (index)
    {
        redisClient->set("speed_mode", "auto");
        ui->lineEditSpeedSOG->setEnabled(false);
        ui->lineEditSpeedCOG->setEnabled(false);

        SpeedAutoModeUi();
    }
    else
    {
        redisClient->set("speed_mode", "manual");
        ui->lineEditSpeedSOG->setEnabled(true);
        ui->lineEditSpeedCOG->setEnabled(true);

        SpeedManualModeUi();
    }
}

void FrameOSD::on_pushButtonSpeedApply_clicked()
{
    QString sog = ui->lineEditSpeedSOG->text();
    QString cog = ui->lineEditSpeedCOG->text();

    std::unordered_map<std::string, std::string> data_map =
    {
        {"sog", sog.toStdString()},
        {"cog", cog.toStdString()},
    };

    redisClient->hmset("speed",data_map.begin(), data_map.end());
}


// ==== Water Speed ==== //

void FrameOSD::WaterSpeedAutoModeUi()
{
    ui->pushButtonWaterApply->setEnabled(false);
    ui->pushButtonWaterApply->setStyleSheet("color: rgb(20, 20, 20);");
}

void FrameOSD::WaterSpeedManualModeUi()
{
    ui->pushButtonWaterApply->setEnabled(true);
    ui->pushButtonWaterApply->setStyleSheet("");
    ui->lineEditWaterSOG->setStyleSheet("color:white;");
    ui->lineEditWaterCOG->setStyleSheet("color:white;");
}

void FrameOSD::WaterSpeedTimerTimeOut()
{
    auto waterspeed_mode = redisClient->get("waterspeed_mode");
    QString waterspeedmode = QString::fromStdString(*waterspeed_mode);
    qDebug() << Q_FUNC_INFO << waterspeedmode;

    if(waterspeedmode == "auto")
    {
        if(redisClient->exists("waterspeed"))
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
        redisClient->set("waterspeed_mode", "auto");
        ui->lineEditWaterSOG->setEnabled(false);
        ui->lineEditWaterCOG->setEnabled(false);

        WaterSpeedAutoModeUi();
    }
    else
    {
        redisClient->set("waterspeed_mode", "manual");
        ui->lineEditWaterSOG->setEnabled(true);
        ui->lineEditWaterCOG->setEnabled(true);

        WaterSpeedManualModeUi();
    }
}

void FrameOSD::on_pushButtonWaterApply_clicked()
{
    QString speed = ui->lineEditWaterSOG->text();
    QString course = ui->lineEditWaterCOG->text();

    std::unordered_map<std::string, std::string> data_map =
    {
        {"speed", speed.toStdString()},
        {"course", course.toStdString()},
    };

    redisClient->hmset("waterspeed",data_map.begin(), data_map.end());
}
