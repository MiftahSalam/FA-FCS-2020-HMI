#include "frame_osd_waterspeed.h"
#include "ui_frame_osd_waterspeed.h"
#include "qtimer.h"
#include "src/di/di.h"

#include <QMessageBox>

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, FrameOSDWaterSpeed)
#else
#include <QDebug>
#endif

FrameOSDWaterSpeed::FrameOSDWaterSpeed(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDWaterSpeed),
    _serviceOSD(DI::getInstance()->getServiceOSD())
{
    ui->setupUi(this);

    //init combobox mode (should be auto by default. make sure to sync with osd server)
    currentMode = OSD_MODE::AUTO;
    currentModeIndx = 0;
    afterResetModeIndx = false;
    ui->mode->setCurrentModeIndex(currentModeIndx);
    ui->pushButton->setEnabled(false);

    ui->inputWaterSpeed->setInputEnable(false);
    ui->inputWaterSpeed->setStatusFailed();

    ui->inputWaterCourse->setInputEnable(false);
    ui->inputWaterCourse->setStatusFailed();

    connect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDWaterSpeed::onModeChange);

    timestamp = QDateTime::currentDateTime();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FrameOSDWaterSpeed::onTimeout);
    timer->start(1000);

    connect(_serviceOSD, &OSDService::signal_processedSetResponseWaterSpeed, this, &FrameOSDWaterSpeed::onDataResponse);
    connect(_serviceOSD, &OSDService::signal_processedSetModeResponse, this, &FrameOSDWaterSpeed::onModeChangeResponse);
    connect(_serviceOSD, &OSDService::signal_processedAutoDataWaterSpeed, this, &FrameOSDWaterSpeed::onStreamReceive);
}

void FrameOSDWaterSpeed::setup()
{
    OSDWaterSpeedProp prop{
        "Water Speed",
        TextInputProp{
            "Drift:", "kts", "w_speedInput", "0"
        },
        TextInputProp{
            "Set:", "deg", "w_courseInput", "0"
        }
    };
    ui->groupBox->setTitle(prop.title);
    ui->inputWaterSpeed->setup(prop.waterspeed);
    ui->inputWaterCourse->setup(prop.watercourse);

    //todo request reset mode to auto
}

void FrameOSDWaterSpeed::resetModeIndex()
{
    afterResetModeIndx = true;

    currentModeIndx = prevModeIndx;
}

void FrameOSDWaterSpeed::onDataResponse(WaterSpeedResponseModel resp)
{
#ifdef USE_LOG4QT
    logger()->debug() << Q_FUNC_INFO << " -> resp code: " << resp.err().getCode() << ", resp msg: " << QString::fromStdString(resp.status());
#else
    qDebug() << Q_FUNC_INFO << "resp code:" << resp.getHttpCode() << "resp msg:" << resp.getMessage();
#endif

    if (resp.err().getCode() != 0) {

        QMessageBox::warning(this, "Request Error", QString("Failed to change manual data with error: %1").arg(resp.err().getMessage()));
        return;
    }

#ifdef USE_LOG4QT
    logger()->debug() << Q_FUNC_INFO
                      << " -> getSpeed: " << resp.getSpeed()
                      << ", getCourse: " << resp.getCourse();
#else
    qDebug()<<Q_FUNC_INFO<<"resp data getWaterSpeed: "<<resp.getData().getSpeed()
           <<"resp data getWaterCourse: "<<resp.getData().getCourse()
             ;
#endif
}

void FrameOSDWaterSpeed::onModeChangeResponse(const QString datafisis, BaseResponse<InputModeResponseModel> resp, bool needConfirm)
{
    if (datafisis != "water_speed") {
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
    logger()->trace() << Q_FUNC_INFO << " -> resp data. waterspeed mode: " << resp.getData().getWaterSpeed();
#else
    qDebug()<<Q_FUNC_INFO<<"resp code:"<<"resp data waterspeed mode: "<<resp.getData().getWaterSpeed();
#endif

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

void FrameOSDWaterSpeed::onModeChange(int index)
{
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

    _serviceOSD->setDataMode("water_speed", currentMode);
}

void FrameOSDWaterSpeed::onAfterModeReset()
{
    afterResetModeIndx = false;
}

void FrameOSDWaterSpeed::autoUiSetup()
{
    ui->pushButton->setEnabled(false);

    ui->inputWaterSpeed->setInputEnable(false);
    ui->inputWaterSpeed->setStatusFailed();

    ui->inputWaterCourse->setInputEnable(false);
    ui->inputWaterCourse->setStatusFailed();
}

void FrameOSDWaterSpeed::manualUiSetup()
{
    ui->pushButton->setEnabled(true);

    ui->inputWaterSpeed->setInputEnable(true);
    ui->inputWaterSpeed->setModeManual();

    ui->inputWaterCourse->setInputEnable(true);
    ui->inputWaterCourse->setModeManual();
}

void FrameOSDWaterSpeed::onTimeout()
{
    //update ui
    //    qDebug()<<Q_FUNC_INFO;

    auto currError = _serviceOSD->getOSDAutoStatusWaterSpeed();
    if (currError.getCode() == ERROR_CODE_MESSAGING_NOT_CONNECTED.first) {
        notConnectedUiSetup();
    } else if (currError.getCode() == ERROR_CODE_MESSAGING_NO_DATA.first) {
        noDataUiSetup();
    } else if (currError.getCode() == ERROR_CODE_MESSAGING_DATA_INVALID_FORMAT.first) {
        invalidDataUiSetup();
    }

    setErrorInput(currError);

    auto curMode = _serviceOSD->getDataMode();
    bool WSMode = curMode->waterSpeed();
    if ((OSD_MODE)WSMode != currentMode) {
        disconnect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDWaterSpeed::onModeChange);
        if (WSMode) {
            ui->mode->setCurrentModeIndex(1);
            manualUiSetup();
            _serviceOSD->setManualDataWaterSpeed(OSDSetWaterSpeedRequest(
                            ui->inputWaterSpeed->getCurrentValue().toFloat(),
                            ui->inputWaterCourse->getCurrentValue().toFloat()
                            ));
        } else {
            ui->mode->setCurrentModeIndex(0);
            autoUiSetup();
        }
        connect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDWaterSpeed::onModeChange);

        currentMode = (OSD_MODE)WSMode;
    }
}

void FrameOSDWaterSpeed::onStreamReceive(WaterSpeedStreamModel model)
{
    //    qDebug()<<Q_FUNC_INFO<<"waterspeed: speed ->"<<model.getSpeed()<<", course ->"<<model.getCourse();
    auto currentMode = (OSD_MODE)_serviceOSD->getDataMode()->waterSpeed();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }

    auto currStreamErr = _serviceOSD->getOSDAutoStatusWaterSpeed();

    // validity Speed Course stream check
    ui->inputWaterSpeed->setValue(QString::number(model.getSpeed()));
    ui->inputWaterCourse->setValue(QString::number(model.getCourse()));

    setErrorInput(currStreamErr);
}

void FrameOSDWaterSpeed::onUpdateWaterSpeedAutoUi()
{
    //    qDebug()<<"UPdate UIIII";
    autoUiSetup();
}

void FrameOSDWaterSpeed::notConnectedUiSetup()
{
    auto currentMode = (OSD_MODE)_serviceOSD->getDataMode()->waterSpeed();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }
    autoUiSetup();
}

void FrameOSDWaterSpeed::noDataUiSetup()
{
    auto currentMode = (OSD_MODE)_serviceOSD->getDataMode()->waterSpeed();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }
    autoUiSetup();
}

void FrameOSDWaterSpeed::invalidDataUiSetup()
{
    auto currentMode = (OSD_MODE)_serviceOSD->getDataMode()->waterSpeed();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }
    autoUiSetup();
}

void FrameOSDWaterSpeed::on_pushButton_clicked()
{
    if (!validateInput()) {
        return;
    }

    try {
        float w_speed = ui->inputWaterSpeed->getCurrentValue().toFloat();
        float w_course = ui->inputWaterCourse->getCurrentValue().toFloat();

        _serviceOSD->setManualDataWaterSpeed(OSDSetWaterSpeedRequest(w_speed, w_course));
    } catch (...) {
        QMessageBox::critical(this, "Fatal Error Water Speed", "Invalid value input" );
    }
}

bool FrameOSDWaterSpeed::validateInput()
{
    bool ok;
    QString wSpeed = ui->inputWaterSpeed->getCurrentValue();
    float value_wSpeed = wSpeed.toFloat(&ok);

    if (!ok)
    {
        QMessageBox::critical(this, "Fatal Error Water Drift", "Invalid input format.\nValid input : -150 to 150");
        return false;
    }

    if ((value_wSpeed < -150) || (value_wSpeed > 150))
    {
        QMessageBox::critical(this, "Fatal Error Drift", "Invalid input : out of range.\nValid input : -150 to 150");
        return false;
    }

    QString wCourse = ui->inputWaterCourse->getCurrentValue();
    float value_wCourse = wCourse.toFloat(&ok);

    if (!ok)
    {
        QMessageBox::critical(this, "Fatal Error Set", "Invalid input format.\nValid input : 0 to 360");
        return false;
    }

    if ((value_wCourse < 0) || (value_wCourse > 360))
    {
        QMessageBox::critical(this, "Fatal Error Set", "Invalid input : out of range.\nValid input : 0 to 360");
        return false;
    }

    return true;
}

void FrameOSDWaterSpeed::setErrorInput(BaseError error)
{
    if (currentMode == OSD_MODE::MANUAL)
    {
        ui->inputWaterSpeed->setToolTip("");
        ui->inputWaterCourse->setToolTip("");
        return;
    }

    if (error.getCode() == ERROR_NO.first)
    {
        ui->inputWaterSpeed->setStatusOk();
        ui->inputWaterCourse->setStatusOk();

        ui->inputWaterSpeed->setToolTip("");
        ui->inputWaterCourse->setToolTip("");
    }
    else
    {
        ui->inputWaterSpeed->setStatusFailed();
        ui->inputWaterCourse->setStatusFailed();

        ui->inputWaterSpeed->setToolTip(error.getMessage());
        ui->inputWaterCourse->setToolTip(error.getMessage());
    }
}

FrameOSDWaterSpeed::~FrameOSDWaterSpeed()
{
    delete ui;
}
