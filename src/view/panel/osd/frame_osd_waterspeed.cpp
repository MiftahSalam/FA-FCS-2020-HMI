#include "frame_osd_waterspeed.h"
#include "ui_frame_osd_waterspeed.h"
#include "qtimer.h"
#include "src/di/di.h"
#include "src/shared/utils/utils.h"

#include <QMessageBox>

FrameOSDWaterSpeed::FrameOSDWaterSpeed(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDWaterSpeed),
    _cmsWS(DI::getInstance()->getOSDCMSService()->getServiceOSDCMSWaterSpeed()),
    _cmsMode(DI::getInstance()->getOSDCMSService()->getServiceOSDCMSMode()),
    _streamWS(DI::getInstance()->getServiceOSDStream()->getServiceOSDStreamWaterSpeed())
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

    connect(_cmsWS, &OSDCMSWaterSpeedData::signal_setWaterSpeedResponse, this, &FrameOSDWaterSpeed::onDataResponse);
    connect(_cmsMode, &OSDCMSInputMode::signal_setModeResponse, this, &FrameOSDWaterSpeed::onModeChangeResponse);
    connect(_streamWS, &OSDStreamWaterSpeed::signalDataProcessed, this, &FrameOSDWaterSpeed::onStreamReceive);
}

void FrameOSDWaterSpeed::setup()
{
    OSDWaterSpeedProp prop{
        "Water Speed",
        TextInputProp{
            "Speed:", "kts", "w_speedInput", "0"
        },
        TextInputProp{
            "Course:", "deg", "w_courseInput", "0"
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

void FrameOSDWaterSpeed::onDataResponse(BaseResponse<WaterSpeedModel> resp)
{
    qDebug()<<Q_FUNC_INFO<<"resp code:"<<resp.getHttpCode()<<"resp msg:"<<resp.getMessage();

    if (resp.getHttpCode() != 0) {
//        ui->mode->setCurrentModeIndex((int)OSD_MODE::AUTO);

        QMessageBox::warning(this, "Request Error", QString("Failed to change manual data with error: %1").arg(resp.getMessage()));
//        autoUiSetup();
        return;

        qDebug()<<Q_FUNC_INFO<<"resp data getWaterSpeed: "<<resp.getData().getW_Speed()
             <<"resp data getWaterCourse: "<<resp.getData().getW_Course()
              ;
    }
}

void FrameOSDWaterSpeed::onModeChangeResponse(const QString datafisis, BaseResponse<InputModeModel> resp, bool needConfirm)
{
    if (datafisis != "waterspeed") {
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

    qDebug()<<Q_FUNC_INFO<<"resp code:"<<"resp data waterspeed mode: "<<resp.getData().getWaterSpeed();

    switch (currentMode) {
    case OSD_MODE::AUTO:
        autoUiSetup();
        break;
    case OSD_MODE::MANUAL:
        manualUiSetup();
        // TODO: get manual data from server
        break;
    default:
        break;
    }
}

void FrameOSDWaterSpeed::onModeChange(int index)
{
    bool manual_mode;
    switch ((OSD_MODE)index) {
    case OSD_MODE::AUTO:
        manual_mode = false;
        currentMode = OSD_MODE::AUTO;
//        autoUiSetup();
        break;
    case OSD_MODE::MANUAL:
        manual_mode = true;
        currentMode = OSD_MODE::MANUAL;
//        manualUiSetup();
        break;
    default:
        break;
    }

    _cmsMode->setDataMode("waterspeed", manual_mode);
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
    qDebug()<<Q_FUNC_INFO;

    auto currError = _streamWS->check();
    if (currError.getCode() == ERROR_CODE_MESSAGING_NOT_CONNECTED.first) {
        notConnectedUiSetup();
    } else if (currError.getCode() == ERROR_CODE_MESSAGING_NO_DATA.first) {
        noDataUiSetup();
    } else if (currError.getCode() == ERROR_CODE_MESSAGING_DATA_INVALID_FORMAT.first) {
        invalidDataUiSetup();
    }

    auto curMode = _cmsMode->getDataMode();
    bool WSMode = curMode.getWaterSpeed();
    if ((OSD_MODE)WSMode != currentMode) {
        disconnect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDWaterSpeed::onModeChange);
        if (WSMode) {
            ui->mode->setCurrentModeIndex(1);
            manualUiSetup();
            _cmsWS->set(OSDSetWaterSpeedRequest(
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

void FrameOSDWaterSpeed::onStreamReceive(WaterSpeedModel model)
{
    qDebug()<<Q_FUNC_INFO<<"waterspeed: speed ->"<<model.getW_Speed()<<", course ->"<<model.getW_Course();
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getWaterSpeed();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }

    ui->inputWaterSpeed->setValue(QString::number(model.getW_Speed()));
    ui->inputWaterSpeed->setStatusOk();

    ui->inputWaterCourse->setValue(QString::number(model.getW_Course()));
    ui->inputWaterCourse->setStatusOk();
}

void FrameOSDWaterSpeed::onUpdateWaterSpeedAutoUi()
{
//    qDebug()<<"UPdate UIIII";
    autoUiSetup();
}

void FrameOSDWaterSpeed::notConnectedUiSetup()
{
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getWaterSpeed();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }
    autoUiSetup();
}

void FrameOSDWaterSpeed::noDataUiSetup()
{
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getWaterSpeed();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }
    autoUiSetup();
}

void FrameOSDWaterSpeed::invalidDataUiSetup()
{
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getWaterSpeed();
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

        _cmsWS->set(OSDSetWaterSpeedRequest(w_speed, w_course));
    } catch (...) {
        QMessageBox::critical(this, "Fatal Error Water Speed", "Invalid value input" );
    }
}

bool FrameOSDWaterSpeed::validateInput()
{
    QString wSpeed = ui->inputWaterSpeed->getCurrentValue();
    float value_wSpeed = wSpeed.toFloat();

    if ((value_wSpeed < -150) || (value_wSpeed > 150))
    {
        QMessageBox::critical(this, "Fatal Error Water Speed", "Invalid input : out of range.\nValid input : -150 to 150");
        return false;
    }

    QString wCourse = ui->inputWaterCourse->getCurrentValue();
    float value_wCourse = wCourse.toFloat();

    if ((value_wCourse < 0) || (value_wCourse > 360))
    {
        QMessageBox::critical(this, "Fatal Error Water Course", "Invalid input : out of range.\nValid input : 0 to 360");
        return false;
    }

    return true;
}


FrameOSDWaterSpeed::~FrameOSDWaterSpeed()
{
    delete ui;
}
