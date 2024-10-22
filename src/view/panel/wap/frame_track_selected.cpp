#include "frame_track_selected.h"
#include "ui_frame_track_selected.h"

FrameTrackSelected::FrameTrackSelected(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameTrackSelected)
{
    ui->setupUi(this);
}

FrameTrackSelected::~FrameTrackSelected()
{
    delete ui;
}
