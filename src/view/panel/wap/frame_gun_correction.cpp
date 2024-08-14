#include "frame_gun_correction.h"
#include "ui_frame_gun_correction.h"

FrameGunCorrection::FrameGunCorrection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameGunCorrection)
{
    ui->setupUi(this);
}

FrameGunCorrection::~FrameGunCorrection()
{
    delete ui;
}
