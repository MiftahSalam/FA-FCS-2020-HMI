#include "frame_osd_position.h"
#include "qtimer.h"

#include "ui_frame_osd_position.h"
#include "src/di/di.h"
#include "src/shared/utils/utils.h"

FrameOSDPosition::FrameOSDPosition(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDPosition),
    _cmsPos(DI::getInstance()->getOSDCMSService()->getServiceOSDCMSPosition())
{
    ui->setupUi(this);

    //init combobox mode (should be auto by default. make sure to sync with osd server)
    currentMode = OSD_MODE::AUTO;
    currentModeIndx = 0;
    afterResetModeIndx = false;
    ui->mode->setCurrentModeIndex(currentModeIndx);
    ui->pushButton->setEnabled(false);

    ui->inputLatitude->setInputEnable(false);
    ui->inputLatitude->setStatusFailed();

    ui->inputLongitude->setInputEnable(false);
    ui->inputLongitude->setStatusFailed();
    connect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDPosition::onModeChange);
}

FrameOSDPosition::~FrameOSDPosition()
{
    delete ui;
}

void FrameOSDPosition::setup()
{
    OSDPositionProp prop{
        "Position",
        TextInputProp{
            "Latitude:", "deg", "latInput", Utils::latDecToStringDegree(12.544)
        },
        TextInputProp{
            "Longitude:", "deg", "latInput", "0.0"
        },
    };
    ui->groupBox->setTitle(prop.title);
    ui->inputLatitude->setup(prop.lat);
    ui->inputLongitude->setup(prop.lon);

}

void FrameOSDPosition::resetModeIndex()
{
    afterResetModeIndx = true;
    ui->mode->setCurrentModeIndex(currentModeIndx);
}

void FrameOSDPosition::onModeChangeResponse(InputModeModel mode)
{
    Q_UNUSED(mode); //temporary
    //handle response
    qDebug()<<Q_FUNC_INFO;

    if (mode.getPosition()) {
        currentModeIndx = int(OSD_MODE::MANUAL);
        manualUiSetup();
    } else {
        currentModeIndx = int(OSD_MODE::AUTO);
        autoUiSetup();
    }
}

void FrameOSDPosition::onModeChange(int index)
{
    if (afterResetModeIndx) {
        QTimer::singleShot(10, this, &FrameOSDPosition::onAfterModeReset);
        return;
    }

    currentMode = (OSD_MODE)index;
    switch (currentMode) {
    case OSD_MODE::AUTO:
        emit signalChangePositionMode(false);
        break;
    case OSD_MODE::MANUAL:
        emit signalChangePositionMode(true);
        break;
    default:
        break;
    }
}

void FrameOSDPosition::onAfterModeReset()
{
    afterResetModeIndx = false;
}

void FrameOSDPosition::autoUiSetup()
{
    ui->pushButton->setEnabled(false);

    ui->inputLatitude->setInputEnable(false);
    ui->inputLatitude->setStatusFailed();

    ui->inputLongitude->setInputEnable(false);
    ui->inputLongitude->setStatusFailed();
}

void FrameOSDPosition::manualUiSetup()
{
    ui->pushButton->setEnabled(true);

    ui->inputLatitude->setInputEnable(true);
    ui->inputLatitude->setModeManual();

    ui->inputLongitude->setInputEnable(true);
    ui->inputLongitude->setModeManual();
}
