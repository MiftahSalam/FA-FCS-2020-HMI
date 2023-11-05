#include "frame_osd_gyro.h"
#include "ui_frame_osd_gyro.h"

//#include "src/view/panel/osd/frame_osd_mode.h"
//#include "src/view/shared/frame_text_input.h"

FrameOSDGyro::FrameOSDGyro(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameOSDGyro)
{
    ui->setupUi(this);
}

void FrameOSDGyro::setup(const OSDGyroProp &prop)
{
    ui->groupBox->setTitle(prop.title);
    ui->inputPitch->setup(prop.pitch);
    ui->inputRoll->setup(prop.roll);
    ui->inputHeading->setup(prop.heading);

}

FrameOSDGyro::~FrameOSDGyro()
{
    delete ui;
}
