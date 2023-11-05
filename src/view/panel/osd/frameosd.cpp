#include "frameosd.h"
#include "ui_frameosd.h"
//#include "global.h"
//#include "unistd.h"
//#include "math.h"

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

    /*
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
    */
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

void FrameOSD::GpsAutoModeUi()
{

}

void FrameOSD::GpsManualModeUi()
{

}

void FrameOSD::GyroTimerTimeOut()
{
}

void FrameOSD::on_osdGryoComboBox_activated(int index)
{
}

void FrameOSD::on_pushButtonGyroApply_clicked()
{
}

void FrameOSD::on_comboBoxGPSMode_activated(int index)
{
}

void FrameOSD::GpsTimerTimeOut()
{

}

void FrameOSD::on_pushButtonGPSApply_clicked()
{
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
}

void FrameOSD::on_comboBoxSpeedMode_activated(int index)
{
}

void FrameOSD::on_pushButtonSpeedApply_clicked()
{
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
}

void FrameOSD::on_comboBoxWaterMode_activated(int index)
{
}

void FrameOSD::on_pushButtonWaterApply_clicked()
{
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
}

void FrameOSD::on_comboBoxWindMode_activated(int index)
{
}

void FrameOSD::on_pushButtonWindApply_clicked()
{
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
}

void FrameOSD::on_comboBoxWeatherMode_activated(int index)
{
}

void FrameOSD::on_pushButtonWeather_clicked()
{
}
