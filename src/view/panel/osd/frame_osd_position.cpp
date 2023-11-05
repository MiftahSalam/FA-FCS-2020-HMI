#include "frame_osd_position.h"
#include "ui_frame_osd_position.h"
#include "src/di/di.h"

FrameOSDPosition::FrameOSDPosition(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDPosition),
    _cmsPos(DI::getInstance()->getOSDCMSService()->getServiceOSDCMSPosition())
{
    ui->setupUi(this);

    //init combobox mode (should be auto by default. make sure to sync with osd server)
    currentMode = OSD_MODE::AUTO;
    ui->mode->setCurrentModeIndex(0);
    autoUiSetup();
    connect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDPosition::onModeChange);
}

FrameOSDPosition::~FrameOSDPosition()
{
    delete ui;
}

void FrameOSDPosition::setup(const OSDPositionProp &prop)
{
    ui->groupBox->setTitle(prop.title);
    ui->inputLatitude->setup(prop.lat);
    ui->inputLongitude->setup(prop.lon);

}

void FrameOSDPosition::onModeChange(int index)
{
    currentMode = (OSD_MODE)index;
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
