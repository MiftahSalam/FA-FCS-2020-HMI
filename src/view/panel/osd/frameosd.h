#ifndef FRAMEOSD_H
#define FRAMEOSD_H

#include "src/usecase/osd/cms/osd_cms_input_mode.h"
#include "src/usecase/osd/cms/osd_cms_position_data.h"
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

private slots:
    void onChangePositionMode(bool manual_mode);
    void onChangePositionData(float lat, float lon);
    void onPositionDataResponse(BaseResponse<PositionModel> resp);

    void onChangeInputModeResponse(BaseResponse<InputModeModel> resp);

private:
    Ui::FrameOSD *ui;
    OSDCMSInputMode* _cmsMode;
    OSDCMSPositionData* _cmsPosition;
    InputModeModel currentMode;
    QString lastUpdateMode;

    void setup();
    void resetToPrevMode();
};

#endif // FRAMEOSD_H
