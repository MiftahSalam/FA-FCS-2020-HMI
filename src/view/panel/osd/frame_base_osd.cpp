#include "frame_base_osd.h"
#include "ui_frame_base_osd.h"

FrameBaseOSD::FrameBaseOSD(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameBaseOSD)
{
    ui->setupUi(this);
}


void FrameBaseOSD::modify(const BaseOSDProp &prop)
{
    ui->groupBox->setTitle(prop.title);
    ui->inputPitch->modify(prop.pitch);
    ui->inputRoll->modify(prop.roll);
    ui->inputHeading->modify(prop.heading);

}

FrameBaseOSD::~FrameBaseOSD()
{
    delete ui;
}
