#include "frame_osd_mode.h"
#include "ui_frame_osd_mode.h"

FrameOSDMode::FrameOSDMode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDMode)
{
    ui->setupUi(this);
}

void FrameOSDMode::setCurrentModeIndex(int index)
{
    ui->comboBoxMode->setCurrentIndex(index);
}

void FrameOSDMode::setEnableCombo(bool enable)
{
    ui->comboBoxMode->setEnabled(enable);
}

FrameOSDMode::~FrameOSDMode()
{
    delete ui;
}
