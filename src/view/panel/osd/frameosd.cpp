#include "frameosd.h"
#include "ui_frameosd.h"
//#include "global.h"
//#include "unistd.h"
//#include "math.h"

#include <QMessageBox>
#include <QRegExpValidator>

#define NUMBER_RX "[0-9.-]+$"


FrameOSD::FrameOSD(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameOSD)
{
    ui->setupUi(this);

    ui->widgetGyro->modify(OSDGyroProp{
                               "Gyro",
                               TextInputProp{
                                   "Heading:", "deg", "headingInput", "0.0"
                               },
                               TextInputProp{
                                   "Pitch:", "deg", "pitchInput", "0.0"
                               },
                               TextInputProp{
                                   "Roll:", "deg", "rollInput", "0.0"
                               },
                           });
    ui->widgetPosition->modify(OSDPositionProp{
                                   "Position",
                                   TextInputProp{
                                       "Latitude:", "deg", "latInput", "0.0"
                                   },
                                   TextInputProp{
                                       "Longitude:", "deg", "latInput", "0.0"
                                   },
                               });


}

FrameOSD::~FrameOSD()
{
    delete ui;
}
