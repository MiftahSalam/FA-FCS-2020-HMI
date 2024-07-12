#include "frame_osd_wind.h"
#include "ui_frame_osd_wind.h"
#include "qtimer.h"
#include "src/di/di.h"
#include "src/shared/utils/utils.h"

#include <QMessageBox>

FrameOSDWind::FrameOSDWind(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDWind),
    _cmsWind(DI::getInstance()->getOSDCMSService()->getServiceOSDCMSWind()),
    _cmsMode(DI::getInstance()->getOSDCMSService()->getServiceOSDCMSMode()),
    _streamWind(DI::getInstance()->getServiceOSDStream()->getServiceOSDStreamWind())
{
    ui->setupUi(this);

    currentMode = OSD_MODE::AUTO;
    currentModeIndx = 0;
    afterResetModeIndx = false;
    ui->mode->setCurrentModeIndex(currentModeIndx);
    ui->pushButton->setEnabled(false);

    ui->inputSpeed->setInputEnable(false);
    ui->inputDirection->setStatusFailed();

    ui->inputSpeed->setInputEnable(false);
    ui->inputDirection->setStatusFailed();

    connect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDWind::onModeChange);

    timestamp = QDateTime::currentDateTime();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FrameOSDWind::onTimeout);
    timer->start(1000);

    connect(_cmsWind, &OSDCMSWindData::signal_setWindResponse, this, &FrameOSDWind::onDataResponse);
    connect(_cmsMode, &OSDCMSInputMode::signal_setModeResponse, this, &FrameOSDWind::onModeChangeResponse);
    connect(_streamWind, &OSDStreamWind::signalDataProcessed, this, &FrameOSDWind::onStreamReceive);
}

void FrameOSDWind::setup()
{
    OSDWindProp prop{
        "Wind",
        TextInputProp{
            "Speed:", "kts", "speedInput", "0"
        },
        TextInputProp{
            "Direction:", "deg", "directionInput", "0"
        }
    };
    ui->groupBox->setTitle(prop.title);
    ui->inputSpeed->setup(prop.speed);
    ui->inputDirection->setup(prop.direction);

    //todo request reset mode to auto
}

void FrameOSDWind::resetModeIndex()
{
    afterResetModeIndx = true;

    currentModeIndx = prevModeIndx;
}

void FrameOSDWind::onDataResponse(BaseResponse<WindModel> resp)
{
    qDebug()<<Q_FUNC_INFO<<"resp code:"<<resp.getHttpCode()<<"resp msg:"<<resp.getMessage();

    if (resp.getHttpCode() != 0) {
//        ui->mode->setCurrentModeIndex((int)OSD_MODE::AUTO);

        QMessageBox::warning(this, "Request Error", QString("Failed to change manual data with error: %1").arg(resp.getMessage()));
//        autoUiSetup();
        return;

        qDebug()<<Q_FUNC_INFO<<"resp data get Speed: "<<resp.getData().getSpeed()
             <<"resp data get Direction: "<<resp.getData().getDirection()
              ;
    }
}

void FrameOSDWind::onModeChangeResponse(const QString datafisis, BaseResponse<InputModeModel> resp, bool needConfirm)
{
    qDebug()<<"data fisi"<<datafisis;
    if (datafisis != "wind") {
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

    qDebug()<<Q_FUNC_INFO<<"resp code:"<<"resp data wind mode: "<<resp.getData().getWind();

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

void FrameOSDWind::onModeChange(int index)
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

    _cmsMode->setDataMode("wind", manual_mode);
}

void FrameOSDWind::onAfterModeReset()
{
    afterResetModeIndx = false;
}

void FrameOSDWind::autoUiSetup()
{
    ui->pushButton->setEnabled(false);

    ui->inputSpeed->setInputEnable(false);
    ui->inputSpeed->setStatusFailed();

    ui->inputDirection->setInputEnable(false);
    ui->inputDirection->setStatusFailed();
}

void FrameOSDWind::manualUiSetup()
{
    ui->pushButton->setEnabled(true);

    ui->inputSpeed->setInputEnable(true);
    ui->inputSpeed->setModeManual();

    ui->inputDirection->setInputEnable(true);
    ui->inputDirection->setModeManual();
}

void FrameOSDWind::onTimeout()
{
    //update ui
    qDebug()<<Q_FUNC_INFO;

    auto currError = _streamWind->check();
    if (currError.getCode() == ERROR_CODE_MESSAGING_NOT_CONNECTED.first) {
        notConnectedUiSetup();
    } else if (currError.getCode() == ERROR_CODE_MESSAGING_NO_DATA.first) {
        noDataUiSetup();
    } else if (currError.getCode() == ERROR_CODE_MESSAGING_DATA_INVALID_FORMAT.first) {
        invalidDataUiSetup();
    }

    auto curMode = _cmsMode->getDataMode();
    bool WindMode = curMode.getWind();
    if ((OSD_MODE)WindMode != currentMode) {
        disconnect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDWind::onModeChange);
        if (WindMode) {
            ui->mode->setCurrentModeIndex(1);
            manualUiSetup();
            _cmsWind->set(OSDSetWindRequest(
                            ui->inputSpeed->getCurrentValue().toFloat(),
                            ui->inputDirection->getCurrentValue().toFloat()
                            ));
        } else {
            ui->mode->setCurrentModeIndex(0);
            autoUiSetup();
        }
        connect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDWind::onModeChange);

        currentMode = (OSD_MODE)WindMode;
    }
}

void FrameOSDWind::onStreamReceive(WindModel model)
{
    qDebug()<<Q_FUNC_INFO<<"wind: speed ->"<<model.getSpeed()<<", direction ->"<<model.getDirection();
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getWind();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }



    ui->inputSpeed->setValue(QString::number(model.getSpeed()));
//    ui->inputSpeed->setStatusOk();

    ui->inputDirection->setValue(QString::number(model.getDirection()));
//    ui->inputDirection->setStatusOk();
    validateInputStream();
}

void FrameOSDWind::onUpdateWindAutoUi()
{
//    qDebug()<<"UPdate UIIII";
    autoUiSetup();
}

void FrameOSDWind::notConnectedUiSetup()
{
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getWind();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }
    autoUiSetup();
}

void FrameOSDWind::noDataUiSetup()
{
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getWind();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }
    autoUiSetup();
}

void FrameOSDWind::invalidDataUiSetup()
{
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getWind();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }
    autoUiSetup();
}

void FrameOSDWind::on_pushButton_clicked()
{
    if (!validateInput()) {
        return;
    }

    try {
        float _speed = ui->inputSpeed->getCurrentValue().toFloat();
        float _direction = ui->inputDirection->getCurrentValue().toFloat();

        _cmsWind->set(OSDSetWindRequest(_speed, _direction));
    } catch (...) {
        QMessageBox::critical(this, "Fatal Error Water Speed", "Invalid value input" );
    }
}

bool FrameOSDWind::validateInput()
{
    bool ok;
    QString _speed = ui->inputSpeed->getCurrentValue();
    float value_speed = _speed.toFloat(&ok);

    if ((value_speed < -150) || (value_speed > 150) || (!ok))
    {
        QMessageBox::critical(this, "Fatal Error Water Speed", "Invalid input : out of range.\nValid input : -150 to 150");
        return false;
    }

    QString _direction = ui->inputDirection->getCurrentValue();
    float value_direction = _direction.toFloat(&ok);

    if ((value_direction < 0) || (value_direction > 360) || (!ok))
    {
        QMessageBox::critical(this, "Fatal Error Water Course", "Invalid input : out of range.\nValid input : 0 to 360");
        return false;
    }

    return true;
}

void FrameOSDWind::validateInputStream()
{
    bool ok;
    QString _speed = ui->inputSpeed->getCurrentValue();
    float value_speed = _speed.toFloat(&ok);

    if ((value_speed < -150) || (value_speed > 150) || (!ok))
    {
        ui->inputSpeed->setStatusFailed();
    }else{
        ui->inputSpeed->setStatusOk();
    }

    QString _direction = ui->inputDirection->getCurrentValue();
    float value_direction = _direction.toFloat(&ok);

    if ((value_direction < 0) || (value_direction > 360) || (!ok))
    {
        ui->inputDirection->setStatusFailed();
    }else{
        ui->inputDirection->setStatusOk();
    }
}

FrameOSDWind::~FrameOSDWind()
{
    delete ui;
}
