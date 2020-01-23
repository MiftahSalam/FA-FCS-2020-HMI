#include "frameosd.h"
#include "ui_frameosd.h"

FrameOSD::FrameOSD(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameOSD)
{
    ui->setupUi(this);
}

FrameOSD::~FrameOSD()
{
    delete ui;
}
