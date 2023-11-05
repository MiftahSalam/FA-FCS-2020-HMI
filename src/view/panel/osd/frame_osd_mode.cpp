#include "frame_osd_mode.h"
#include "ui_frame_osd_mode.h"

FrameOSDMode::FrameOSDMode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDMode)
{
    ui->setupUi(this);

    connect(ui->comboBoxMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameOSDMode::signal_currentModeChange);
}

void FrameOSDMode::setCurrentModeIndex(int index)
{
    ui->comboBoxMode->setCurrentIndex(index);
}

FrameOSDMode::~FrameOSDMode()
{
    delete ui;
}
