#include "frame_track_engage.h"
#include "ui_frame_track_engage.h"

FrameTrackEngage::FrameTrackEngage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameTrackEngage)
{
    ui->setupUi(this);
}

FrameTrackEngage::~FrameTrackEngage()
{
    delete ui;
}
