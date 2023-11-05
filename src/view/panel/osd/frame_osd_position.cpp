#include "frame_osd_position.h"
#include "ui_frame_osd_position.h"

FrameOSDPosition::FrameOSDPosition(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDPosition),
    di(DI::getInstance())
{
    ui->setupUi(this);

    //init combobox mode (should be auto by default. make sure to sync with osd server)
    ui->mode->setCurrentModeIndex(0);
    ui->mode->setEnableCombo(false);
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
