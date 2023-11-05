#include "framegun.h"
#include "ui_framegun.h"

FrameGun::FrameGun(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameGun)
{
    ui->setupUi(this);
}

FrameGun::~FrameGun()
{
    delete ui;
}
