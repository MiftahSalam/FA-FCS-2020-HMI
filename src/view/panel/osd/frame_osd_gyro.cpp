#include "frame_osd_gyro.h"
#include "qtimer.h"

#include "ui_frame_osd_gyro.h"
#include "src/di/di.h"

#include <QMessageBox>

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, FrameOSDGyro)
#else
#include <QDebug>
#endif

FrameOSDGyro::FrameOSDGyro(QWidget *parent) : QWidget(parent),
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
    bool manual_mode;
    switch ((OSD_MODE)index)
    {
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

    _cmsMode->setDataMode("inertia", manual_mode);
}

void FrameOSDGyro::onAfterModeReset()
{
    afterResetModeIndx = false;
}

void FrameOSDGyro::onTimeout()
{
    auto currError = _streamGyro->check();
    if (currError.getCode() == ERROR_CODE_MESSAGING_NOT_CONNECTED.first)
    {
        notConnectedUiSetup();
    }
    else if (currError.getCode() == ERROR_CODE_MESSAGING_NO_DATA.first)
    {
        noDataUiSetup();
    }
    else if (currError.getCode() == ERROR_CODE_MESSAGING_DATA_INVALID_FORMAT.first)
    {
        invalidDataUiSetup();
    }

    setErrorInput(currError);

    auto curMode = _cmsMode->getDataMode();
    bool inertiaMode = curMode->inersia();
    if ((OSD_MODE)inertiaMode != currentMode)
    {
        disconnect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDGyro::onModeChange);
        if (inertiaMode)
        {
            ui->mode->setCurrentModeIndex(1);
            manualUiSetup();
            _cmsGyro->set(OSDSetGyroRequest(
                              ui->inputHeading->getCurrentValue().toFloat(),
                              ui->inputPitch->getCurrentValue().toFloat(),
                              ui->inputRoll->getCurrentValue().toFloat()));
        }
        else
        {
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
            "Heading:", "deg", "HeadingInput", "0"},
        TextInputProp{
            "Pitch:", "deg", "HeadingInput", "0"},
        TextInputProp{
            "Roll:", "deg", "RollInput", "0"}};
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
    if (datafisis != "inertia")
    {
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

    if (resp.getHttpCode() != 0)
    {
        resetModeIndex();
        if (needConfirm)
        {
            QMessageBox::warning(this, "Request Error", QString("Failed to input mode with error: %1").arg(resp.getMessage()));
        }

        return;
    }

#ifdef USE_LOG4QT
    logger()->trace() << Q_FUNC_INFO << " -> resp data. inertia mode: " << resp.getData().getInersia();
#else
    qDebug() << Q_FUNC_INFO << "resp code:" << "resp data inertia mode: " << resp.getData().getInersia();
#endif

    //    auto currentMode = (OSD_MODE)resp.getData().getInersia();
    switch (currentMode)
    {
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

void FrameOSDGyro::onDataResponse(BaseResponse<GyroModel> resp)
{
    // todo handle response
#ifdef USE_LOG4QT
    logger()->debug() << Q_FUNC_INFO << " -> resp code: " << resp.getHttpCode() << ", resp msg: " << resp.getMessage();
#else
    qDebug() << Q_FUNC_INFO << "resp code:" << resp.getHttpCode() << "resp msg:" << resp.getMessage();
#endif

    if (resp.getHttpCode() != 0)
    {
        QMessageBox::warning(this, "Request Error", QString("Failed to change manual data with error: %1").arg(resp.getMessage()));
        return;
    }

#ifdef USE_LOG4QT
    logger()->debug() << Q_FUNC_INFO
                      << " -> getHeading: " << resp.getData().getHeading()
                      << ", getPicth: " << resp.getData().getPicth()
                      << ", getRoll: " << resp.getData().getRoll();
#else
    qDebug() << Q_FUNC_INFO
             << "resp data getHeading: " << resp.getData().getHeading()
             << "resp data getPicth: " << resp.getData().getPicth()
             << "resp data getRoll: " << resp.getData().getRoll();
#endif

}

void FrameOSDGyro::onStreamReceive(GyroModel model)
{
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode()->inersia();
    if (currentMode == OSD_MODE::MANUAL)
    {
        return;
    }

    auto currStreamErr = _streamGyro->check();

    // validity pitch roll stream check
    ui->inputHeading->setValue(QString::number(model.getHeading()));
    ui->inputPitch->setValue(QString::number(model.getPicth()));
    ui->inputRoll->setValue(QString::number(model.getRoll()));

    setErrorInput(currStreamErr);
}

void FrameOSDGyro::onUpdateGyroAutoUi()
{
    autoUiSetup();
}

void FrameOSDGyro::on_pushButton_clicked()
{
    if (!validateInput())
    {
        return;
    }

    try
    {
        float heading = ui->inputHeading->getCurrentValue().toFloat();
        float pitch = ui->inputPitch->getCurrentValue().toFloat();
        float roll = ui->inputRoll->getCurrentValue().toFloat();

        _cmsGyro->set(OSDSetGyroRequest(heading, pitch, roll));
    }
    catch (...)
    {
        QMessageBox::critical(this, "Fatal Error Inertia", "Invalid value input");
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
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode()->inersia();
    if (currentMode == OSD_MODE::MANUAL)
    {
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
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode()->inersia();
    if (currentMode == OSD_MODE::MANUAL)
    {
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
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode()->inersia();
    if (currentMode == OSD_MODE::MANUAL)
    {
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
    bool ok;

    QString Heading = ui->inputHeading->getCurrentValue();
    float valueheading = Heading.toFloat(&ok);

    if ((valueheading < 0) || (valueheading > 360) || (!ok))
    {
        QMessageBox::critical(this, "Fatal Error Heading", "Invalid input : out of range.\nValid input : 0-360");
        return false;
    }

    QString Pitch = ui->inputPitch->getCurrentValue();
    float valuepitch = Pitch.toFloat(&ok);

    if ((valuepitch < -30) || (valuepitch > 30) || (!ok))
    {
        QMessageBox::critical(this, "Fatal Error Pitch", "Invalid input : out of range.\nValid input : -30 - 30");
        return false;
    }

    QString Roll = ui->inputRoll->getCurrentValue();
    float valueroll = Roll.toFloat(&ok);

    if ((valueroll < -30) || (valueroll > 30) || (!ok))
    {
        QMessageBox::critical(this, "Fatal Error Roll", "Invalid input : out of range.\nValid input : -30 - 30");
        return false;
    }

    return true;
}

void FrameOSDGyro::setErrorInput(BaseError error)
{
    if (currentMode == OSD_MODE::MANUAL)
    {
        ui->inputHeading->setToolTip("");
        ui->inputPitch->setToolTip("");
        ui->inputRoll->setToolTip("");
        return;
    }

    if (error.getCode() == ERROR_NO.first)
    {
        ui->inputHeading->setStatusOk();
        ui->inputPitch->setStatusOk();
        ui->inputRoll->setStatusOk();

        ui->inputHeading->setToolTip("");
        ui->inputPitch->setToolTip("");
        ui->inputRoll->setToolTip("");
    }
    else if (error.getCode() == ERROR_CODE_OSD_DATA_PARTIALLY_INVALID.first)
    {
        ui->inputHeading->setStatusOk();
        ui->inputPitch->setStatusFailed();
        ui->inputRoll->setStatusFailed();

        ui->inputHeading->setToolTip("");
        ui->inputPitch->setToolTip(error.getMessage());
        ui->inputRoll->setToolTip(error.getMessage());
    }
    else
    {
        ui->inputHeading->setStatusFailed();
        ui->inputPitch->setStatusFailed();
        ui->inputRoll->setStatusFailed();

        ui->inputHeading->setToolTip(error.getMessage());
        ui->inputPitch->setToolTip(error.getMessage());
        ui->inputRoll->setToolTip(error.getMessage());
    }
}
