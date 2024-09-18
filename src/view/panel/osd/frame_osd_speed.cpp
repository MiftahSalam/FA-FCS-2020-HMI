#include "frame_osd_speed.h"
#include "ui_frame_osd_speed.h"
#include "qtimer.h"
#include "src/di/di.h"

#include <QMessageBox>

FrameOSDSpeed::FrameOSDSpeed(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDSpeed),
    _cmsSpeed(DI::getInstance()->getOSDCMSService()->getServiceOSDCMSSpeed()),
    _cmsMode(DI::getInstance()->getOSDCMSService()->getServiceOSDCMSMode()),
    _streamSpeed(DI::getInstance()->getServiceOSDStream()->getServiceOSDStreamSpeed())
{
    ui->setupUi(this);

    currentModeIndx = 0;
    currentMode = OSD_MODE::AUTO;
    afterResetModeIndx = false;
    ui->mode->setCurrentModeIndex(currentModeIndx);
    ui->pushButton->setEnabled(false);

    ui->inputSpeed->setInputEnable(false);
    ui->inputSpeed->setStatusFailed();

    ui->inputCourse->setInputEnable(false);
    ui->inputCourse->setStatusFailed();

    connect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDSpeed::onModeChange);

    timestamp = QDateTime::currentDateTime();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FrameOSDSpeed::onTimeout);
    timer->start(1000);

    connect(_cmsSpeed, &OSDCMSSpeedData::signal_setSpeedResponse, this, &FrameOSDSpeed::onDataResponse);
    connect(_cmsMode, &OSDCMSInputMode::signal_setModeResponse, this, &FrameOSDSpeed::onModeChangeResponse);
    connect(_streamSpeed, &OSDStreamSpeed::signalDataProcessed, this, &FrameOSDSpeed::onStreamReceive);

}

void FrameOSDSpeed::onModeChange(int index)
{
    qDebug()<<"Status Speed :" << index;

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

    _cmsMode->setDataMode("speed", manual_mode);
    // on_pushButton_clicked();
}

void FrameOSDSpeed::onAfterModeReset()
{
    afterResetModeIndx = false;
}

void FrameOSDSpeed::onTimeout()
{
//    qDebug()<<Q_FUNC_INFO;

    auto currError = _streamSpeed->check();
    if (currError.getCode() == ERROR_CODE_MESSAGING_NOT_CONNECTED.first) {
        notConnectedUiSetup();
    } else if (currError.getCode() == ERROR_CODE_MESSAGING_NO_DATA.first) {
        noDataUiSetup();
    } else if (currError.getCode() == ERROR_CODE_MESSAGING_DATA_INVALID_FORMAT.first) {
        invalidDataUiSetup();
    }

    auto curMode = _cmsMode->getDataMode();
    bool speedMode = curMode.getSpeed();
    if ((OSD_MODE)speedMode != currentMode) {
        disconnect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDSpeed::onModeChange);
        if (speedMode) {
            ui->mode->setCurrentModeIndex(1);
            manualUiSetup();
            _cmsSpeed->set(OSDSetSpeedRequest(
                              ui->inputSpeed->getCurrentValue().toFloat(),
                              ui->inputCourse->getCurrentValue().toFloat()
                              ));
        } else {
            ui->mode->setCurrentModeIndex(0);
            autoUiSetup();
        }
        connect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDSpeed::onModeChange);

        currentMode = (OSD_MODE)speedMode;
    }
}

void FrameOSDSpeed::setup()
{
        OSDSpeedProp prop{
            "Speed",
            TextInputProp{
                "SOG:", "kts", "SpeedInput", "0"
            },
            TextInputProp{
                "COG:", "deg", "CourseInput", "0"
            }
        };
        ui->groupBox->setTitle(prop.title);
        ui->inputSpeed->setup(prop.speed);
        ui->inputCourse->setup(prop.course);
}

void FrameOSDSpeed::resetModeIndex()
{
    afterResetModeIndx = true;
    currentModeIndx = prevModeIndx;
}

void FrameOSDSpeed::onModeChangeResponse(const QString datafisis, BaseResponse<InputModeModel> resp, bool needConfirm)
{
    if (datafisis != "speed") {
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

    qDebug()<<Q_FUNC_INFO<<"resp code:"<<"resp data speed mode: "<<resp.getData().getSpeed();

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

void FrameOSDSpeed::onDataResponse(BaseResponse<SpeedModel> resp)
{
    //todo handle response
    qDebug()<<Q_FUNC_INFO<<"resp code:"<<resp.getHttpCode()<<"resp msg:"<<resp.getMessage();

    if (resp.getHttpCode() != 0) {
        QMessageBox::warning(this, "Request Error", QString("Failed to change manual data with error: %1").arg(resp.getMessage()));
        return;
    }

    qDebug()<<Q_FUNC_INFO
           <<"resp data getSpeed: "<<resp.getData().getSpeed()
          <<"resp data getCourse: "<<resp.getData().getCourse()
           ;
}

void FrameOSDSpeed::onStreamReceive(SpeedModel model)
{
//    qDebug()<<Q_FUNC_INFO<<"Speed: SOG ->"<<model.getSpeed()
//           <<", COG ->"<<model.getCourse();

    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getSpeed();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }

    auto currStreamErr = _streamSpeed->check();

    //validity SOG COG stream check
    ui->inputSpeed->setValue(QString::number(model.getSpeed()));
    ui->inputCourse->setValue(QString::number(model.getCourse()));

    if (currStreamErr.getCode() == ERROR_NO.first)
    {
        ui->inputSpeed->setStatusOk();
        ui->inputCourse->setStatusOk();
    }
    else
    {
        ui->inputSpeed->setStatusFailed();
        ui->inputCourse->setStatusFailed();
    }
}

void FrameOSDSpeed::onUpdateSpeedAutoUi()
{
    autoUiSetup();
}

void FrameOSDSpeed::on_pushButton_clicked()
{
    if (!validateInput()) {
        return;
    }

    try {
        float speed = ui->inputSpeed->getCurrentValue().toFloat();
        float course = ui->inputCourse->getCurrentValue().toFloat();

        _cmsSpeed->set(OSDSetSpeedRequest(speed, course));
    } catch (...) {
        QMessageBox::critical(this, "Fatal Error Speed", "Invalid value input" );
    }
}

void FrameOSDSpeed::manualUiSetup()
{
    ui->pushButton->setEnabled(true);

    ui->inputSpeed->setInputEnable(true);
    ui->inputSpeed->setModeManual();

    ui->inputCourse->setInputEnable(true);
    ui->inputCourse->setModeManual();
}

void FrameOSDSpeed::autoUiSetup()
{
    ui->pushButton->setEnabled(false);

    ui->inputSpeed->setInputEnable(false);
    ui->inputSpeed->setStatusFailed();

    ui->inputCourse->setInputEnable(false);
    ui->inputCourse->setStatusFailed();
}

void FrameOSDSpeed::notConnectedUiSetup()
{
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getSpeed();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }

    autoUiSetup();
}

void FrameOSDSpeed::noDataUiSetup()
{
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getSpeed();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }
    autoUiSetup();
}

void FrameOSDSpeed::invalidDataUiSetup()
{
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getSpeed();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }
    autoUiSetup();
}

bool FrameOSDSpeed::validateInput()
{
    bool ok;

    QString Speed = ui->inputSpeed->getCurrentValue();
    float valuespeed = Speed.toFloat(&ok);

    if ((valuespeed < -150) || (valuespeed > 150) || (!ok))
    {
        QMessageBox::critical(this, "Fatal Error Speed", "Invalid input : out of range.\nValid input : -150 to 150");
        return false;
    }

    QString Course = ui->inputCourse->getCurrentValue();
    float valuecourse = Course.toFloat(&ok);

    if ((valuecourse < 0) || (valuecourse > 360) || (!ok))
    {
        QMessageBox::critical(this, "Fatal Error Course", "Invalid input : out of range.\nValid input : 0 to 360");
        return false;
    }

    return true;
}

FrameOSDSpeed::~FrameOSDSpeed()
{
    delete ui;
}
