#include "frame_osd_weather.h"
#include "ui_frame_osd_weather.h"
#include "qtimer.h"
#include "src/di/di.h"

#include <QMessageBox>

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, FrameOSDWeather)
#else
#include <QDebug>
#endif

FrameOSDWeather::FrameOSDWeather(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDWeather),
    _serviceOSD(DI::getInstance()->getServiceOSD())
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

    connect(_serviceOSD, &OSDService::signal_processedSetResponseWeather, this, &FrameOSDWeather::onDataResponse);
    connect(_serviceOSD, &OSDService::signal_processedSetModeResponse, this, &FrameOSDWeather::onModeChangeResponse);
    connect(_serviceOSD, &OSDService::signal_processedAutoDataWeather, this, &FrameOSDWeather::onStreamReceive);
}

void FrameOSDWeather::onModeChange(int index)
{
    qDebug()<<"Status Weather :" << index;

    switch ((OSD_MODE)index) {
    case OSD_MODE::AUTO:
        currentMode = OSD_MODE::AUTO;
        break;
    case OSD_MODE::MANUAL:
        currentMode = OSD_MODE::MANUAL;
        break;
    default:
        break;
    }
    _serviceOSD->setDataMode("weather", currentMode);
}

void FrameOSDWeather::onAfterModeReset()
{
    afterResetModeIndx = false;
}

void FrameOSDWeather::onTimeout()
{
    auto currError = _serviceOSD->getOSDAutoStatusWeather();
    if (currError.getCode() == ERROR_CODE_MESSAGING_NOT_CONNECTED.first) {
        notConnectedUiSetup();
    } else if (currError.getCode() == ERROR_CODE_MESSAGING_NO_DATA.first) {
        noDataUiSetup();
    } else if (currError.getCode() == ERROR_CODE_MESSAGING_DATA_INVALID_FORMAT.first) {
        invalidDataUiSetup();
    }

    setErrorInput(currError);

    auto curMode = _serviceOSD->getDataMode();
    bool weatherMode = curMode->weather();
    if ((OSD_MODE)weatherMode != currentMode) {
        disconnect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDWeather::onModeChange);
        if (weatherMode) {
            ui->mode->setCurrentModeIndex(1);
            manualUiSetup();
            _serviceOSD->setManualDataWeather(OSDSetWeatherRequest(
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

void FrameOSDWeather::onModeChangeResponse(const QString datafisis, BaseResponse<InputModeResponseModel> resp, bool needConfirm)
{
    if (datafisis != "weather") {
        return;
    }

#ifdef USE_LOG4QT
    logger()->debug() << Q_FUNC_INFO << " -> resp code: " << resp.getHttpCode()
                      << ", resp msg: " << resp.getMessage()
                      << ", needConfirm: " << needConfirm
                         ;
#else
    qDebug() << Q_FUNC_INFO << "resp code:" << resp.getHttpCode() << "resp msg:" << resp.getMessage();
    qDebug() << Q_FUNC_INFO << "needConfirm:" << needConfirm;
#endif

    if (resp.getHttpCode() != 0) {
        resetModeIndex();
        if (needConfirm) {
            QMessageBox::warning(this, "Request Error", QString("Failed to input mode with error: %1").arg(resp.getMessage()));
        }
        return;
    }

#ifdef USE_LOG4QT
    logger()->trace() << Q_FUNC_INFO << " -> resp data. weather mode: " << resp.getData().getWeather();
#else
    qDebug()<<Q_FUNC_INFO<<"resp code:"<<"resp data weather mode: "<<resp.getData().getWeather();
#endif

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

void FrameOSDWeather::onDataResponse(WeatherResponseModel resp)
{
    //todo handle response
#ifdef USE_LOG4QT
    logger()->debug() << Q_FUNC_INFO << " -> resp code: " << resp.err().getCode() << ", resp msg: " << QString::fromStdString(resp.status());
#else
    qDebug() << Q_FUNC_INFO << "resp code:" << resp.getHttpCode() << "resp msg:" << resp.getMessage();
#endif

    if (resp.err().getCode() != 0)
    {
        QMessageBox::warning(this, "Request Error", QString("Failed to change manual data with error: %1").arg(resp.err().getMessage()));
        return;
    }

#ifdef USE_LOG4QT
    logger()->debug() << Q_FUNC_INFO
                      << " -> getHumidity: " << resp.getHumidity()
                      << ", getPressure: " << resp.getPressure()
                      << ", getTemperature: " << resp.getTemperature();
#else
    qDebug()<<Q_FUNC_INFO
           <<"resp data getTemperature: "<<resp.getData().getTemperature()
          <<"resp data getPressure: "<<resp.getData().getPressure()
         <<"resp data getHumidity: "<<resp.getData().getHumidity()
           ;
#endif
}

void FrameOSDWeather::onStreamReceive(WeatherStreamModel model)
{
    auto currentMode = (OSD_MODE)_serviceOSD->getDataMode()->weather();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }

    auto currStreamErr = _serviceOSD->getOSDAutoStatusWeather();

    // validity temp hum press stream check
    ui->inputTemp->setValue(QString::number(model.getTemperature()));
    ui->inputPress->setValue(QString::number(model.getPressure()));
    ui->inputHum->setValue(QString::number(model.getHumidity()));

    setErrorInput(currStreamErr);
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

        _serviceOSD->setManualDataWeather(OSDSetWeatherRequest(temp, press, hum));
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
    auto currentMode = (OSD_MODE)_serviceOSD->getDataMode()->weather();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }
    autoUiSetup();
}

void FrameOSDWeather::noDataUiSetup()
{
    auto currentMode = (OSD_MODE)_serviceOSD->getDataMode()->weather();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }
    autoUiSetup();
}

void FrameOSDWeather::invalidDataUiSetup()
{
    auto currentMode = (OSD_MODE)_serviceOSD->getDataMode()->weather();
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

    if ((valuetemp < -50) || (valuetemp > 100) || (!ok))
    {
        QMessageBox::critical(this, "Fatal Error Temperature", "Invalid input : out of range.\nValid input : -50 to 100");
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

void FrameOSDWeather::setErrorInput(BaseError error)
{
    if (currentMode == OSD_MODE::MANUAL)
    {
        ui->inputTemp->setToolTip("");
        ui->inputPress->setToolTip("");
        ui->inputHum->setToolTip("");
        return;
    }

    if (error.getCode() == ERROR_NO.first)
    {
        ui->inputTemp->setStatusOk();
        ui->inputPress->setStatusOk();
        ui->inputHum->setStatusOk();

        ui->inputTemp->setToolTip("");
        ui->inputPress->setToolTip("");
        ui->inputHum->setToolTip("");
    }
    else
    {
        ui->inputTemp->setStatusFailed();
        ui->inputPress->setStatusFailed();
        ui->inputHum->setStatusFailed();

        ui->inputTemp->setToolTip(error.getMessage());
        ui->inputPress->setToolTip(error.getMessage());
        ui->inputHum->setToolTip(error.getMessage());
    }
}

FrameOSDWeather::~FrameOSDWeather()
{
    delete ui;
}
