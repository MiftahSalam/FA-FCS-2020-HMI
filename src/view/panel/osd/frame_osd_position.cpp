#include "frame_osd_position.h"
#include "ui_frame_osd_position.h"

FrameOSDPosition::FrameOSDPosition(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDPosition)
{
    ui->setupUi(this);
}

FrameOSDPosition::~FrameOSDPosition()
{
    delete ui;
}

void FrameOSDPosition::modify(const OSDPositionProp &prop)
{
    ui->groupBox->setTitle(prop.title);
    ui->inputLatitude->modify(prop.lat);
    ui->inputLongitude->modify(prop.lon);

}
