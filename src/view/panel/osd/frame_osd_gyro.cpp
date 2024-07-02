#include "frame_osd_gyro.h"
#include "qtimer.h"

#include "ui_frame_osd_gyro.h"
#include "src/di/di.h"
#include "src/shared/utils/utils.h"

#include <QMessageBox>

FrameOSDGyro::FrameOSDGyro(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDGyro),
    _cmsGyro(DI::getInstance()->getOSDCMSService()->getServiceOSDCMSGyro()),
    _streamGyro(DI::getInstance()->getServiceOSDStream()->getServiceOSDStreamGyro())
{
    ui->setupUi(this);

//    currentMode = OSD_MODE::AUTO;
    currentModeIndx = 0;
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

    connect(_streamGyro, &OSDStreamGyro::signalDataProcessed, this, &FrameOSDGyro::onStreamReceive);
}

void FrameOSDGyro::onModeChange(int index)
{
    qDebug()<<"Status Gyro :" << index;

    if (afterResetModeIndx) {
        QTimer::singleShot(10, this, &FrameOSDGyro::onAfterModeReset);
        return;
    }

    auto currentMode = (OSD_MODE)index;
    switch (currentMode) {
    case OSD_MODE::AUTO:
        emit signalChangeGyroMode(false);
        break;
    case OSD_MODE::MANUAL:
        emit signalChangeGyroMode(true);
        manualUiSetup();
        break;
    default:
        break;
    }
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
    ui->mode->setCurrentModeIndex(currentModeIndx);
}

void FrameOSDGyro::onModeChangeResponse(BaseResponse<InputModeModel> resp, bool needConfirm)
{
    qDebug()<<Q_FUNC_INFO<<"resp code:"<<resp.getHttpCode()
           <<"resp msg:"<<resp.getMessage()
         <<"resp data position mode: "<<resp.getData().getPosition()
          ;

    if (resp.getHttpCode() != 0) {
        resetModeIndex();
        QMessageBox::warning(this, "Request Error", QString("Failed to input mode with error: %1").arg(resp.getMessage()));

        return;
    }

//    prevMode = currentMode;
    prevModeIndx = currentModeIndx;
}

void FrameOSDGyro::onDataResponse(BaseResponse<GyroModel> data)
{
    Q_UNUSED(data); //temporary
    //todo handle response
    qDebug()<<Q_FUNC_INFO;
}

void FrameOSDGyro::onStreamReceive(GyroModel model)
{
    qDebug()<<Q_FUNC_INFO<<"Inertia: Heading ->"<<model.getHeading()
           <<", Pitch ->"<<model.getPicth()<<", Roll ->"<<model.getRoll();
//    if (currentMode == OSD_MODE::MANUAL) {
//        return;
//    }

    //validity pitch roll stream check

    ui->inputHeading->setValue(QString::number(model.getHeading()));
    ui->inputHeading->setStatusOk();

    ui->inputPitch->setValue(QString::number(model.getPicth()));
    if (model.getPicth() == 90){
        ui->inputPitch->setStatusFailed();
    }else{
    ui->inputPitch->setStatusOk();
    }

    ui->inputRoll->setValue(QString::number(model.getRoll()));
    if (model.getRoll() == 90){
        ui->inputRoll->setStatusFailed();
    }else{
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

        emit signalChangeGyroData(heading, pitch, roll);
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
//    if (currentMode == OSD_MODE::MANUAL) {
//        return;
//    }

    autoUiSetup();
}

void FrameOSDGyro::noDataUiSetup()
{
//    if (currentMode == OSD_MODE::MANUAL) {
//        return;
//    }

    autoUiSetup();
}

void FrameOSDGyro::invalidDataUiSetup()
{
//    if (currentMode == OSD_MODE::MANUAL) {
//        return;
//    }

    autoUiSetup();
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
