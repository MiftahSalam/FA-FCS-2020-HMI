#include "frame_osd_gyro.h"
#include "qtimer.h"

#include "ui_frame_osd_gyro.h"
#include "src/di/di.h"

#include <QMessageBox>

FrameOSDGyro::FrameOSDGyro(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDGyro),
    _cmsGyro(DI::getInstance()->getOSDCMSService()->getServiceOSDCMSGyro()),
    _cmsMode(DI::getInstance()->getOSDCMSService()->getServiceOSDCMSMode()),
    _streamGyro(DI::getInstance()->getServiceOSDStream()->getServiceOSDStreamGyro())
{
    ui->setupUi(this);

    currentModeIndx = 0;
    currentMode = OSD_MODE::AUTO;
    afterResetModeIndx = false;
    ui->mode->setCurrentModeIndex(currentModeIndx);
    ui->pushButton->setEnabled(false);

    ui->inputHeading->setInputEnable(false);
    ui->inputHeading->setStatusFailed();

    ui->inputPitch->setInputEnable(false);
    ui->inputPitch->setStatusFailed();

    ui->inputRoll->setInputEnable(false);
    ui->inputRoll->setStatusFailed();

    connect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDGyro::onModeChange);

    timestamp = QDateTime::currentDateTime();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FrameOSDGyro::onTimeout);
    timer->start(1000);

    connect(_cmsGyro, &OSDCMSGyroData::signal_setGyroResponse, this, &FrameOSDGyro::onDataResponse);
    connect(_cmsMode, &OSDCMSInputMode::signal_setModeResponse, this, &FrameOSDGyro::onModeChangeResponse);
    connect(_streamGyro, &OSDStreamGyro::signalDataProcessed, this, &FrameOSDGyro::onStreamReceive);
}

void FrameOSDGyro::onModeChange(int index)
{
    qDebug()<<"Status Gyro :" << index;

    bool manual_mode;
    switch ((OSD_MODE)index) {
    case OSD_MODE::AUTO:
        manual_mode = false;
        break;
    case OSD_MODE::MANUAL:
        manual_mode = true;
        break;
    default:
        break;
    }

    _cmsMode->setDataMode("inertia", manual_mode);
}

void FrameOSDGyro::onAfterModeReset()
{
    afterResetModeIndx = false;
}

void FrameOSDGyro::onTimeout()
{
    qDebug()<<Q_FUNC_INFO;

    auto currError = _streamGyro->check();
    if (currError.getCode() == ERROR_CODE_MESSAGING_NOT_CONNECTED.first) {
        notConnectedUiSetup();
    } else if (currError.getCode() == ERROR_CODE_MESSAGING_NO_DATA.first) {
        noDataUiSetup();
    } else if (currError.getCode() == ERROR_CODE_MESSAGING_DATA_INVALID_FORMAT.first) {
        invalidDataUiSetup();
    }

    auto curMode = _cmsMode->getDataMode();
    bool inertiaMode = curMode.getInersia();
    if ((OSD_MODE)inertiaMode != currentMode) {
        disconnect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDGyro::onModeChange);
        if (inertiaMode) {
            ui->mode->setCurrentModeIndex(1);
            manualUiSetup();
            _cmsGyro->set(OSDSetGyroRequest(
                              ui->inputHeading->getCurrentValue().toFloat(),
                              ui->inputPitch->getCurrentValue().toFloat(),
                              ui->inputRoll->getCurrentValue().toFloat()
                              ));
        } else {
            ui->mode->setCurrentModeIndex(0);
            autoUiSetup();
        }
        connect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDGyro::onModeChange);

        currentMode = (OSD_MODE)inertiaMode;
    }
}

FrameOSDGyro::~FrameOSDGyro()
{
    delete ui;
}

void FrameOSDGyro::setup()
{
    OSDGyroProp prop{
        "Inertia",
        TextInputProp{
            "Heading:", "deg", "HeadingInput", "0"
        },
        TextInputProp{
            "Pitch:", "deg", "HeadingInput", "0"
        },
        TextInputProp{
            "Roll:", "deg", "RollInput", "0"
        }
    };
    ui->groupBox->setTitle(prop.title);
    ui->inputHeading->setup(prop.heading);
    ui->inputPitch->setup(prop.pitch);
    ui->inputRoll->setup(prop.roll);
}

void FrameOSDGyro::resetModeIndex()
{
    afterResetModeIndx = true;
    currentModeIndx = prevModeIndx;
}

void FrameOSDGyro::onModeChangeResponse(const QString datafisis, BaseResponse<InputModeModel> resp, bool needConfirm)
{
    if (datafisis != "inertia") {
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

    qDebug()<<Q_FUNC_INFO<<"resp code:"<<"resp data inertia mode: "<<resp.getData().getInersia();

    auto currentMode = (OSD_MODE)resp.getData().getInersia();
    switch (currentMode) {
    case OSD_MODE::AUTO:
        autoUiSetup();
        break;
    case OSD_MODE::MANUAL:
        manualUiSetup();
        break;
    default:
        break;
    }
}

void FrameOSDGyro::onDataResponse(BaseResponse<GyroModel> resp)
{
    //todo handle response
    qDebug()<<Q_FUNC_INFO<<"resp code:"<<resp.getHttpCode()<<"resp msg:"<<resp.getMessage();

    if (resp.getHttpCode() != 0) {
        QMessageBox::warning(this, "Request Error", QString("Failed to change manual data with error: %1").arg(resp.getMessage()));
        return;

    }

    qDebug()<<Q_FUNC_INFO
           <<"resp data getHeading: "<<resp.getData().getHeading()
          <<"resp data getPicth: "<<resp.getData().getPicth()
         <<"resp data getRoll: "<<resp.getData().getRoll()
           ;
}

void FrameOSDGyro::onStreamReceive(GyroModel model)
{
    qDebug()<<Q_FUNC_INFO<<"Inertia: Heading ->"<<model.getHeading()
           <<", Pitch ->"<<model.getPicth()<<", Roll ->"<<model.getRoll();

    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getInersia();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }

    //validity pitch roll stream check
    ui->inputHeading->setValue(QString::number(model.getHeading()));
    ui->inputHeading->setStatusOk();

    ui->inputPitch->setValue(QString::number(model.getPicth()));
    if (model.getPicth() == 90){
        ui->inputPitch->setStatusFailed();
    } else{
        ui->inputPitch->setStatusOk();
    }

    ui->inputRoll->setValue(QString::number(model.getRoll()));
    if (model.getRoll() == 90){
        ui->inputRoll->setStatusFailed();
    } else{
        ui->inputRoll->setStatusOk();
    }
}

void FrameOSDGyro::onUpdateGyroAutoUi()
{
    autoUiSetup();
}

void FrameOSDGyro::on_pushButton_clicked()
{
    if (!validateInput()) {
        return;
    }

    try {
        float heading = ui->inputHeading->getCurrentValue().toFloat();
        float pitch = ui->inputPitch->getCurrentValue().toFloat();
        float roll = ui->inputRoll->getCurrentValue().toFloat();

        _cmsGyro->set(OSDSetGyroRequest(heading, pitch, roll));
    } catch (...) {
        QMessageBox::critical(this, "Fatal Error Inertia", "Invalid value input" );
    }
}

void FrameOSDGyro::manualUiSetup()
{
    ui->pushButton->setEnabled(true);

    ui->inputHeading->setInputEnable(true);
    ui->inputHeading->setModeManual();

    ui->inputPitch->setInputEnable(true);
    ui->inputPitch->setModeManual();

    ui->inputRoll->setInputEnable(true);
    ui->inputRoll->setModeManual();
}

void FrameOSDGyro::autoUiSetup()
{
    ui->pushButton->setEnabled(false);

    ui->inputHeading->setInputEnable(false);
    ui->inputHeading->setStatusFailed();

    ui->inputPitch->setInputEnable(false);
    ui->inputPitch->setStatusFailed();

    ui->inputRoll->setInputEnable(false);
    ui->inputRoll->setStatusFailed();
}

void FrameOSDGyro::notConnectedUiSetup()
{
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getInersia();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }

    ui->pushButton->setEnabled(false);

    ui->inputHeading->setInputEnable(false);
    ui->inputHeading->setStatusFailed();

    ui->inputPitch->setInputEnable(false);
    ui->inputPitch->setStatusFailed();

    ui->inputRoll->setInputEnable(false);
    ui->inputRoll->setStatusFailed();
}

void FrameOSDGyro::noDataUiSetup()
{
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getInersia();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }

    ui->pushButton->setEnabled(false);

    ui->inputHeading->setInputEnable(false);
    ui->inputHeading->setStatusFailed();

    ui->inputPitch->setInputEnable(false);
    ui->inputPitch->setStatusFailed();

    ui->inputRoll->setInputEnable(false);
    ui->inputRoll->setStatusFailed();
}

void FrameOSDGyro::invalidDataUiSetup()
{
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getInersia();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }

    ui->pushButton->setEnabled(false);

    ui->inputHeading->setInputEnable(false);
    ui->inputHeading->setStatusFailed();

    ui->inputPitch->setInputEnable(false);
    ui->inputPitch->setStatusFailed();

    ui->inputRoll->setInputEnable(false);
    ui->inputRoll->setStatusFailed();
}

bool FrameOSDGyro::validateInput()
{
    QString Heading = ui->inputHeading->getCurrentValue();
    float valueheading = Heading.toFloat();

    if ((valueheading < 0) || (valueheading > 360))
    {
        QMessageBox::critical(this, "Fatal Error Heading", "Invalid input : out of range.\nValid input : 0-360");
        return false;
    }

    QString Pitch = ui->inputPitch->getCurrentValue();
    float valuepitch = Pitch.toFloat();

    if ((valuepitch < -30) || (valuepitch > 30))
    {
        QMessageBox::critical(this, "Fatal Error Pitch", "Invalid input : out of range.\nValid input : -30 - 30");
        return false;
    }

    QString Roll = ui->inputRoll->getCurrentValue();
    float valueroll = Roll.toFloat();

    if ((valueroll < -30) || (valueroll > 30))
    {
        QMessageBox::critical(this, "Fatal Error Roll", "Invalid input : out of range.\nValid input : -30 - 30");
        return false;
    }

    return true;

}
