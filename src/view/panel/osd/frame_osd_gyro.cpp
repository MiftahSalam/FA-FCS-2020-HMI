#include "frame_osd_gyro.h"
#include "ui_frame_osd_gyro.h"

//#include "src/view/panel/osd/frame_osd_mode.h"
//#include "src/view/shared/frame_text_input.h"

#include "ui_frame_osd_gyro.h"
#include "src/di/di.h"
#include "src/shared/utils/utils.h"

FrameOSDGyro::FrameOSDGyro(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDGyro)
{
    ui->setupUi(this);

    connect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDGyro::onModeChange);
}

void FrameOSDGyro::setup(const OSDGyroProp &prop)
{
    ui->groupBox->setTitle(prop.title);
    ui->inputPitch->setup(prop.pitch);
    ui->inputRoll->setup(prop.roll);
    ui->inputHeading->setup(prop.heading);

}

void FrameOSDGyro::onModeChange(int index)
{
    qDebug()<<"Status Gyro :" << index;

//    if (afterResetModeIndx) {
//        QTimer::singleShot(10, this, &FrameOSDPosition::onAfterModeReset);
//        return;
//    }

//    currentMode = (OSD_MODE)index;
//    switch (currentMode) {
//    case OSD_MODE::AUTO:
//        emit signalChangePositionMode(false);
//        break;
//    case OSD_MODE::MANUAL:
//        emit signalChangePositionMode(true);
//        break;
//    default:
//        break;
//    }
}

FrameOSDGyro::~FrameOSDGyro()
{
    delete ui;
}
