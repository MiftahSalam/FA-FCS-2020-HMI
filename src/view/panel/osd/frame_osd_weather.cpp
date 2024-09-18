#include "frame_osd_weather.h"
#include "ui_frame_osd_weather.h"
#include "qtimer.h"
#include "src/di/di.h"

#include <QMessageBox>

FrameOSDWeather::FrameOSDWeather(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDWeather),
    _cmsWeather(DI::getInstance()->getOSDCMSService()->getServiceOSDCMSWeather()),
    _cmsMode(DI::getInstance()->getOSDCMSService()->getServiceOSDCMSMode()),
    _streamWeather(DI::getInstance()->getServiceOSDStream()->getServiceOSDStreamWeather())
{
    ui->setupUi(this);

    currentModeIndx = 0;
    currentMode = OSD_MODE::AUTO;
    afterResetModeIndx = false;
    ui->mode->setCurrentModeIndex(currentModeIndx);
    ui->pushButton->setEnabled(false);

    ui->inputTemp->setInputEnable(false);
    ui->inputTemp->setStatusFailed();

    ui->inputHum->setInputEnable(false);
    ui->inputHum->setStatusFailed();

    ui->inputPress->setInputEnable(false);
    ui->inputPress->setStatusFailed();

    connect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDWeather::onModeChange);

    timestamp = QDateTime::currentDateTime();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FrameOSDWeather::onTimeout);
    timer->start(1000);

    connect(_cmsWeather, &OSDCMSWeatherData::signal_setWeatherResponse, this, &FrameOSDWeather::onDataResponse);
    connect(_cmsMode, &OSDCMSInputMode::signal_setModeResponse, this, &FrameOSDWeather::onModeChangeResponse);
    connect(_streamWeather, &OSDStreamWeather::signalDataProcessed, this, &FrameOSDWeather::onStreamReceive);
}

void FrameOSDWeather::onModeChange(int index)
{
    qDebug()<<"Status Weather :" << index;

    bool manual_mode;
    switch ((OSD_MODE)index) {
    case OSD_MODE::AUTO:
        manual_mode = false;
        currentMode = OSD_MODE::AUTO;
        break;
    case OSD_MODE::MANUAL:
        manual_mode = true;
        currentMode = OSD_MODE::MANUAL;
        break;
    default:
        break;
    }
    _cmsMode->setDataMode("weather", manual_mode);
}

void FrameOSDWeather::onAfterModeReset()
{
    afterResetModeIndx = false;
}

void FrameOSDWeather::onTimeout()
{
//    qDebug()<<Q_FUNC_INFO;

    auto currError = _streamWeather->check();
    if (currError.getCode() == ERROR_CODE_MESSAGING_NOT_CONNECTED.first) {
        notConnectedUiSetup();
    } else if (currError.getCode() == ERROR_CODE_MESSAGING_NO_DATA.first) {
        noDataUiSetup();
    } else if (currError.getCode() == ERROR_CODE_MESSAGING_DATA_INVALID_FORMAT.first) {
        invalidDataUiSetup();
    }

    auto curMode = _cmsMode->getDataMode();
    bool weatherMode = curMode.getWeather();
    if ((OSD_MODE)weatherMode != currentMode) {
        disconnect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDWeather::onModeChange);
        if (weatherMode) {
            ui->mode->setCurrentModeIndex(1);
            manualUiSetup();
            _cmsWeather->set(OSDSetWeatherRequest(
                ui->inputTemp->getCurrentValue().toFloat(),
                ui->inputPress->getCurrentValue().toFloat(),
                ui->inputHum->getCurrentValue().toFloat()
                ));
        } else {
            ui->mode->setCurrentModeIndex(0);
            autoUiSetup();
        }
        connect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDWeather::onModeChange);
        currentMode = (OSD_MODE)weatherMode;
    }
}

void FrameOSDWeather::setup()
{
    OSDWeatherProp prop{
        "Weather",
        TextInputProp{
            "Temperature:", "'C", "TempInput", "0"
        },
        TextInputProp{
            "Pressure:", "mBar", "PressInput", "0"
        },
        TextInputProp{
            "Humidity:", "%", "HumInput", "0"
        }
    };
    ui->groupBox->setTitle(prop.title);
    ui->inputTemp->setup(prop.Temp);
    ui->inputPress->setup(prop.Press);
    ui->inputHum->setup(prop.Hum);
}

void FrameOSDWeather::resetModeIndex()
{
    afterResetModeIndx = true;
    currentModeIndx = prevModeIndx;
}

void FrameOSDWeather::onModeChangeResponse(const QString datafisis, BaseResponse<InputModeModel> resp, bool needConfirm)
{
    if (datafisis != "weather") {
        return;
    }

    qDebug()<<Q_FUNC_INFO<<"resp code:"<<resp.getHttpCode()<<"resp msg:"<<resp.getMessage();
    qDebug()<<Q_FUNC_INFO<<"needConfirm:"<<needConfirm;

    if (resp.getHttpCode() != 0) {
        resetModeIndex();
        if (needConfirm) {
            QMessageBox::warning(this, "Request Error", QString("Failed to input mode with error: %1").arg(resp.getMessage()));
        }
        return;
    }

    qDebug()<<Q_FUNC_INFO<<"resp code:"<<"resp data weather mode: "<<resp.getData().getWeather();

    //    auto currentMode = (OSD_MODE)resp.getData().getInersia();
    switch (currentMode) {
    case OSD_MODE::AUTO:
        autoUiSetup();
        break;
    case OSD_MODE::MANUAL:
        manualUiSetup();
        on_pushButton_clicked();
        break;
    default:
        break;
    }
}

void FrameOSDWeather::onDataResponse(BaseResponse<WeatherModel> resp)
{
    //todo handle response
    qDebug()<<Q_FUNC_INFO<<"resp code:"<<resp.getHttpCode()<<"resp msg:"<<resp.getMessage();

    if (resp.getHttpCode() != 0) {
        QMessageBox::warning(this, "Request Error", QString("Failed to change manual data with error: %1").arg(resp.getMessage()));
        return;
    }

    qDebug()<<Q_FUNC_INFO
             <<"resp data getTemperature: "<<resp.getData().getTemperature()
             <<"resp data getPressure: "<<resp.getData().getPressure()
             <<"resp data getHumidity: "<<resp.getData().getHumidity()
        ;
}

void FrameOSDWeather::onStreamReceive(WeatherModel model)
{
//    qDebug()<<Q_FUNC_INFO<<"Weather: Temperature ->"<<model.getTemperature()
//             <<", Pressure ->"<<model.getPressure()<<", Humidity ->"<<model.getHumidity();

    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getWeather();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }

    auto currStreamErr = _streamWeather->check();

    // validity temp hum press stream check
    ui->inputTemp->setValue(QString::number(model.getTemperature()));
    ui->inputPress->setValue(QString::number(model.getPressure()));
    ui->inputHum->setValue(QString::number(model.getHumidity()));

    if (currStreamErr.getCode() == ERROR_NO.first)
    {
        ui->inputTemp->setStatusOk();
        ui->inputPress->setStatusOk();
        ui->inputHum->setStatusOk();
    }
    else
    {
        ui->inputTemp->setStatusFailed();
        ui->inputPress->setStatusFailed();
        ui->inputHum->setStatusFailed();
    }
}

void FrameOSDWeather::on_pushButton_clicked()
{
    if (!validateInput()) {
        return;
    }

    try {
        float temp = ui->inputTemp->getCurrentValue().toFloat();
        float press = ui->inputPress->getCurrentValue().toFloat();
        float hum = ui->inputHum->getCurrentValue().toFloat();

        _cmsWeather->set(OSDSetWeatherRequest(temp, press, hum));
    } catch (...) {
        QMessageBox::critical(this, "Fatal Error Weather", "Invalid value input" );
    }
}

void FrameOSDWeather::manualUiSetup()
{
    ui->pushButton->setEnabled(true);

    ui->inputTemp->setInputEnable(true);
    ui->inputTemp->setModeManual();

    ui->inputPress->setInputEnable(true);
    ui->inputPress->setModeManual();

    ui->inputHum->setInputEnable(true);
    ui->inputHum->setModeManual();
}

void FrameOSDWeather::autoUiSetup()
{
    ui->pushButton->setEnabled(false);

    ui->inputTemp->setInputEnable(false);
    ui->inputTemp->setStatusFailed();

    ui->inputPress->setInputEnable(false);
    ui->inputPress->setStatusFailed();

    ui->inputHum->setInputEnable(false);
    ui->inputHum->setStatusFailed();
}

void FrameOSDWeather::notConnectedUiSetup()
{
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getWeather();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }
    autoUiSetup();
}

void FrameOSDWeather::noDataUiSetup()
{
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getWeather();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }
    autoUiSetup();
}

void FrameOSDWeather::invalidDataUiSetup()
{
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getWeather();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }
    autoUiSetup();
}

bool FrameOSDWeather::validateInput()
{
    bool ok;

    QString Temp = ui->inputTemp->getCurrentValue();
    float valuetemp = Temp.toFloat(&ok);

    if ((valuetemp < -273) || (valuetemp > 273) || (!ok))
    {
        QMessageBox::critical(this, "Fatal Error Temperature", "Invalid input : out of range.\nValid input : -273 to 273");
        return false;
    }

    QString Press = ui->inputPress->getCurrentValue();
    float valuepress = Press.toFloat(&ok);

    if ((valuepress < 100) || (valuepress > 1000) || (!ok))
    {
        QMessageBox::critical(this, "Fatal Error Pressure", "Invalid input : out of range.\nValid input : 100 to 1000");
        return false;
    }

    QString Hum = ui->inputHum->getCurrentValue();
    float valuehum = Hum.toFloat(&ok);

    if ((valuehum < 0) || (valuehum > 100) || (!ok))
    {
        QMessageBox::critical(this, "Fatal Error Humidity", "Invalid input : out of range.\nValid input : 0 to 100");
        return false;
    }
    return true;
}

FrameOSDWeather::~FrameOSDWeather()
{
    delete ui;
}
