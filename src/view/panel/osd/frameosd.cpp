#include "frameosd.h"
#include "src/shared/utils/utils.h"
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

    setup();
}

FrameOSD::~FrameOSD()
{
    delete ui;
}

void FrameOSD::setup()
{
    ui->widgetGyro->setup(OSDGyroProp{
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
    ui->widgetPosition->setup(OSDPositionProp{
                                   "Position",
                                   TextInputProp{
                                       "Latitude:", "deg", "latInput", Utils::latDecToStringDegree(12.544)
                                   },
                                   TextInputProp{
                                       "Longitude:", "deg", "latInput", "0.0"
                                   },
                               });

}
