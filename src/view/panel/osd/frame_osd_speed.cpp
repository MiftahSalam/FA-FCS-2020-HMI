#include "frame_osd_speed.h"
#include "ui_frame_osd_speed.h"
#include "qtimer.h"
#include "src/di/di.h"

#include <QMessageBox>

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, FrameOSDSpeed)
#else
#include <QDebug>
#endif

FrameOSDSpeed::FrameOSDSpeed(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDSpeed),
    _serviceOSD(DI::getInstance()->getServiceOSD())
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

    connect(_serviceOSD, &OSDService::signal_processedSetResponseSpeed, this, &FrameOSDSpeed::onDataResponse);
    connect(_serviceOSD, &OSDService::signal_processedSetModeResponse, this, &FrameOSDSpeed::onModeChangeResponse);
    connect(_serviceOSD, &OSDService::signal_processedAutoDataSpeed, this, &FrameOSDSpeed::onStreamReceive);

}

void FrameOSDSpeed::onModeChange(int index)
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

    _serviceOSD->setDataMode("speed", currentMode);
}

void FrameOSDSpeed::onAfterModeReset()
{
    afterResetModeIndx = false;
}

void FrameOSDSpeed::onTimeout()
{
    auto currError = _serviceOSD->getOSDAutoStatusSpeed();
    if (currError.getCode() == ERROR_CODE_MESSAGING_NOT_CONNECTED.first) {
        notConnectedUiSetup();
    } else if (currError.getCode() == ERROR_CODE_MESSAGING_NO_DATA.first) {
        noDataUiSetup();
    } else if (currError.getCode() == ERROR_CODE_MESSAGING_DATA_INVALID_FORMAT.first) {
        invalidDataUiSetup();
    }

    setErrorInput(currError);

    auto curMode = _serviceOSD->getDataMode();
    bool speedMode = curMode->speed();
    if ((OSD_MODE)speedMode != currentMode) {
        disconnect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDSpeed::onModeChange);
        if (speedMode) {
            ui->mode->setCurrentModeIndex(1);
            manualUiSetup();
            _serviceOSD->setManualDataSpeed(OSDSetSpeedRequest(
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
            "Speed:", "kts", "SpeedInput", "0"
        },
        TextInputProp{
            "Course:", "deg", "CourseInput", "0"
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

void FrameOSDSpeed::onModeChangeResponse(const QString datafisis, BaseResponse<InputModeResponseModel> resp, bool needConfirm)
{
    if (datafisis != "speed") {
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
    logger()->trace() << Q_FUNC_INFO << " -> resp data. speed mode: " << resp.getData().getSpeed();
#else
    qDebug() << Q_FUNC_INFO << "resp code:" << "resp data speed mode: " << resp.getData().getSpeed();
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

void FrameOSDSpeed::onDataResponse(SpeedResponseModel resp)
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
                      << " -> getSpeed: " << resp.getSpeed()
                      << ", getCourse: " << resp.getCourse();
#else
    qDebug()<<Q_FUNC_INFO
           <<"resp data getSpeed: "<<resp.getData().getSpeed()
          <<"resp data getCourse: "<<resp.getData().getCourse()
            ;
#endif
}

void FrameOSDSpeed::onStreamReceive(SpeedStreamModel model)
{
    auto currentMode = (OSD_MODE)_serviceOSD->getDataMode()->speed();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }

    auto currStreamErr = _serviceOSD->getOSDAutoStatusSpeed();

    //validity SOG COG stream check
    ui->inputSpeed->setValue(QString::number(model.getSpeed()));
    ui->inputCourse->setValue(QString::number(model.getCourse()));

    setErrorInput(currStreamErr);

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

        _serviceOSD->setManualDataSpeed(OSDSetSpeedRequest(speed, course));
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
    auto currentMode = (OSD_MODE)_serviceOSD->getDataMode()->speed();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }

    autoUiSetup();
}

void FrameOSDSpeed::noDataUiSetup()
{
    auto currentMode = (OSD_MODE)_serviceOSD->getDataMode()->speed();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }
    autoUiSetup();
}

void FrameOSDSpeed::invalidDataUiSetup()
{
    auto currentMode = (OSD_MODE)_serviceOSD->getDataMode()->speed();
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

    if (!ok)
    {
        QMessageBox::critical(this, "Fatal Error Speed", "Invalid input format.\nValid input : -150 to 150");
        return false;
    }

    if ((valuespeed < -150) || (valuespeed > 150))
    {
        QMessageBox::critical(this, "Fatal Error Speed", "Invalid input : out of range.\nValid input : -150 to 150");
        return false;
    }

    QString Course = ui->inputCourse->getCurrentValue();
    float valuecourse = Course.toFloat(&ok);

    if (!ok)
    {
        QMessageBox::critical(this, "Fatal Error Course", "Invalid input format.\nValid input : 0 to 360");
        return false;
    }

    if ((valuecourse < 0) || (valuecourse > 360))
    {
        QMessageBox::critical(this, "Fatal Error Course", "Invalid input : out of range.\nValid input : 0 to 360");
        return false;
    }

    return true;
}

void FrameOSDSpeed::setErrorInput(BaseError error)
{
    if (currentMode == OSD_MODE::MANUAL)
    {
        ui->inputSpeed->setToolTip("");
        ui->inputCourse->setToolTip("");
        return;
    }

    if (error.getCode() == ERROR_NO.first)
    {
        ui->inputSpeed->setStatusOk();
        ui->inputCourse->setStatusOk();

        ui->inputSpeed->setToolTip("");
        ui->inputCourse->setToolTip("");
    }
    else
    {
        ui->inputSpeed->setStatusFailed();
        ui->inputCourse->setStatusFailed();

        ui->inputSpeed->setToolTip(error.getMessage());
        ui->inputCourse->setToolTip(error.getMessage());
    }
}

FrameOSDSpeed::~FrameOSDSpeed()
{
    delete ui;
}
