#include "frame_osd_speed.h"
#include "ui_frame_osd_speed.h"

FrameOSDSpeed::FrameOSDSpeed(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDSpeed)
{
    ui->setupUi(this);

    //test
    OSDSpeedProp prop{
        "Speed",
        TextInputProp{
            "SOG:", "kts", "SpeedInput", "0"
        },
        TextInputProp{
            "COG:", "deg", "CourseInput", "0"
        }
    };
    ui->groupBox->setTitle(prop.title);
    ui->inputSpeed->setup(prop.speed);
    ui->inputCourse->setup(prop.course);
}

FrameOSDSpeed::~FrameOSDSpeed()
{
    delete ui;
}

