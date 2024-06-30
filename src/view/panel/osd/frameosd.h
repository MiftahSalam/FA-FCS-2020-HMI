#ifndef FRAMEOSD_H
#define FRAMEOSD_H

#include "src/usecase/osd/cms/osd_cms_input_mode.h"
#include "src/usecase/osd/cms/osd_cms_position_data.h"
#include "src/usecase/osd/cms/osd_cms_speed_data.h"
#include <QFrame>
#include <QDebug>
#include <QTimer>
#include <QSettings>

namespace Ui {
class FrameOSD;
}

class FrameOSD : public QFrame
{
    Q_OBJECT

public:
    explicit FrameOSD(QWidget *parent = 0);
    ~FrameOSD();

signals:
    void signalOnModeResponse(InputModeModel resp);

    void signalOnPositionDataResponse(PositionModel resp);
    void signalOnSpeedDataResponse(SpeedModel resp);
    //added by riyadhi
    void signalupdateAutoUi();

private slots:
    void onChangePositionMode(bool manual_mode);
    void onChangePositionData(float lat, float lon);
    void onPositionDataResponse(BaseResponse<PositionModel> resp);

    void onChangeSpeedMode(bool manual_mode);
    void onChangeSpeedData(float sog, float cog);
    void onSpeedDataResponse(BaseResponse<SpeedModel> resp);

    void onChangeInputModeResponse(BaseResponse<InputModeModel> resp);

private:
    Ui::FrameOSD *ui;
    OSDCMSInputMode* _cmsMode;
    OSDCMSPositionData* _cmsPosition;
    OSDCMSSpeedData* _cmsSpeed;
    InputModeModel currentMode;
    QString lastUpdateMode;

    void setup();
    void resetToPrevMode();
};

#endif // FRAMEOSD_H
