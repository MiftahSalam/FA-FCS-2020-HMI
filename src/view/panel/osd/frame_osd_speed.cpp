#include "frame_osd_speed.h"
#include "ui_frame_osd_speed.h"
#include "qtimer.h"
#include "src/di/di.h"
#include "src/shared/utils/utils.h"

#include <QMessageBox>

FrameOSDSpeed::FrameOSDSpeed(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDSpeed),
    _cmsSpeed(DI::getInstance()->getOSDCMSService()->getServiceOSDCMSSpeed()),
    _streamSpeed(DI::getInstance()->getServiceOSDStream()->getServiceOSDStreamSpeed())
{
    ui->setupUi(this);

    currentMode = OSD_MODE::AUTO;
    currentModeIndx = 0;
    afterResetModeIndx = false;
    ui->mode->setCurrentModeIndex(currentModeIndx);
    ui->pushButton->setEnabled(false);

    ui->inputKnots->setInputEnable(false);
    ui->inputCourse->setStatusFailed();

    ui->inputKnots->setInputEnable(false);
    ui->inputCourse->setStatusFailed();
    connect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDSpeed::onModeChange);

    timestamp = QDateTime::currentDateTime();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FrameOSDSpeed::onTimeout);
    timer->start(1000);

    connect(_streamSpeed, &OSDStreamSpeed::signalDataProcessed, this, &FrameOSDSpeed::onStreamReceive);

}

FrameOSDSpeed::~FrameOSDSpeed()
{
    delete ui;
}

void FrameOSDSpeed::setup()
{
    OSDSpeedProp prop{
        "Speed",
        TextInputProp{
            "SOG:", "kts", "sogInput", "0.0"
        },
        TextInputProp{
            "COG:", "deg", "latInput", "0.0"
        },
    };
    ui->groupBox->setTitle(prop.title);
    ui->inputKnots->setup(prop.sog);
    ui->inputCourse->setup(prop.cog);

}

void FrameOSDSpeed::resetModeIndex()
{
    afterResetModeIndx = true;
    ui->mode->setCurrentModeIndex(currentModeIndx);
}

void FrameOSDSpeed::onModeChangeResponse(InputModeModel mode)
{
    Q_UNUSED(mode); //temporary
    //handle response
    qDebug()<<Q_FUNC_INFO;

    if (mode.getSpeed()) {
        currentModeIndx = int(OSD_MODE::MANUAL);
        manualUiSetup();
        ui->pushButton->click();
    } else {
        currentModeIndx = int(OSD_MODE::AUTO);
        autoUiSetup();
    }
}

void FrameOSDSpeed::onDataResponse(SpeedModel data)
{
    Q_UNUSED(data); //temporary
    //todo handle response
    qDebug()<<Q_FUNC_INFO;
}

void FrameOSDSpeed::onStreamReceive(SpeedModel model)
{
    qDebug()<<Q_FUNC_INFO<<"speed: sog ->"<<model.getSpeed_OG()<<", cog ->"<<model.getCourse_OG();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }

    ui->inputKnots->setValue(QString::number(model.getSpeed_OG()));
    ui->inputKnots->setStatusOk();

    ui->inputCourse->setValue(QString::number(model.getCourse_OG()));
    ui->inputCourse->setStatusOk();

}

void FrameOSDSpeed::onUpdateAutoUi()
{
    autoUiSetup();;
}

void FrameOSDSpeed::onModeChange(int index)
{
    if (afterResetModeIndx) {
        QTimer::singleShot(10, this, &FrameOSDSpeed::onAfterModeReset);
        return;
    }

    currentMode = (OSD_MODE)index;
    switch (currentMode) {
    case OSD_MODE::AUTO:
        emit signalChangeSpeedMode(false);
        break;
    case OSD_MODE::MANUAL:
        emit signalChangeSpeedMode(true);
        manualUiSetup();
        break;
    default:
        break;
    }
}

void FrameOSDSpeed::onAfterModeReset()
{
    afterResetModeIndx = false;
}

void FrameOSDSpeed::onTimeout()
{
    //update ui
    qDebug()<<Q_FUNC_INFO;
    auto currError = _streamSpeed->check();
    if (currError.getCode() == ERROR_CODE_MESSAGING_NOT_CONNECTED.first) {
        notConnectedUiSetup();
    } else if (currError.getCode() == ERROR_CODE_MESSAGING_NO_DATA.first) {
        noDataUiSetup();
    } else if (currError.getCode() == ERROR_CODE_MESSAGING_DATA_INVALID_FORMAT.first) {
        invalidDataUiSetup();
    }
}

void FrameOSDSpeed::manualUiSetup()
{
    ui->pushButton->setEnabled(true);

    ui->inputKnots->setInputEnable(true);
    ui->inputKnots->setModeManual();

    ui->inputCourse->setInputEnable(true);
    ui->inputCourse->setModeManual();
}

void FrameOSDSpeed::autoUiSetup()
{
    ui->pushButton->setEnabled(false);

    ui->inputKnots->setInputEnable(false);
    ui->inputKnots->setStatusFailed();

    ui->inputCourse->setInputEnable(false);
    ui->inputCourse->setStatusFailed();
}

void FrameOSDSpeed::notConnectedUiSetup()
{
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }

    autoUiSetup();
}

void FrameOSDSpeed::noDataUiSetup()
{
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }

    autoUiSetup();
}

void FrameOSDSpeed::invalidDataUiSetup()
{
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }

    autoUiSetup();

}

bool FrameOSDSpeed::validateInput()
{
    QString speed = ui->inputKnots->getCurrentValue();
    float valuespeed = speed.toFloat();

    if ((valuespeed < -150) || (valuespeed > 150))
    {
        QMessageBox::critical(this, "Fatal Error Heading", "Invalid input : out of range.\nValid input : -150 - 150");
        return false;
    }

    QString course = ui->inputCourse->getCurrentValue();
    float valuecourse = course.toFloat();

    if ((valuecourse < 0) || (valuecourse > 360))
    {
        QMessageBox::critical(this, "Fatal Error Pitch", "Invalid input : out of range.\nValid input : 0 - 360");
        return false;
    }

    return true;
}

