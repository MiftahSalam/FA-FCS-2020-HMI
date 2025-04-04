#ifndef FRAME_OSD_SPEED_H
#define FRAME_OSD_SPEED_H

#include "src/usecase/osd/osd_service.h"
#include "src/view/panel/osd/frame_osd_base.h"
#include "src/view/shared/frame_text_input.h"
#include <QWidget>

namespace Ui {
class FrameOSDSpeed;
}

struct OSDSpeedProp
{
    QString title;
    TextInputProp speed;
    TextInputProp course;
};

class FrameOSDSpeed : public QWidget, public FrameOSDBase<SpeedStreamModel, SpeedResponseModel>
{
    Q_OBJECT

public:
    explicit FrameOSDSpeed(QWidget *parent = nullptr);
    ~FrameOSDSpeed();

    void setup() override;
    void resetModeIndex() override;

public slots:
    void onModeChangeResponse(const QString datafisis, BaseResponse<InputModeResponseModel> mode, bool needConfirm) override;
    void onDataResponse(SpeedResponseModel data) override;
    void onStreamReceive(SpeedStreamModel model) override;
    void onUpdateSpeedAutoUi();

signals:
    void signalChangeSpeedMode(bool manual_mode);
    void signalChangeSpeedData(float speed, float course);

private slots:
    void onModeChange(int index) override;
    void onAfterModeReset() override;
    void onTimeout() override;

    void on_pushButton_clicked();

private:
    Ui::FrameOSDSpeed *ui;

    OSDService *_serviceOSD;

    void manualUiSetup() override;
    void autoUiSetup() override;

    void notConnectedUiSetup() override;
    void noDataUiSetup() override;
    void invalidDataUiSetup() override;

    bool validateInput() override;

    void setErrorInput(BaseError error);
};

#endif // FRAME_OSD_SPEED_H
