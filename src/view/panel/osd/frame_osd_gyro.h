#ifndef FRAME_OSD_GYRO_H
#define FRAME_OSD_GYRO_H

#include "src/usecase/osd/osd_service.h"
#include "src/view/panel/osd/frame_osd_base.h"
#include "src/view/shared/frame_text_input.h"

#include <QWidget>

namespace Ui {
class FrameOSDGyro;
}

struct OSDGyroProp
{
    QString title;
    TextInputProp heading;
    TextInputProp pitch;
    TextInputProp roll;
};

class FrameOSDGyro : public QWidget, public FrameOSDBase<GyroStreamModel, GyroResponseModel>
{
    Q_OBJECT

public:
    explicit FrameOSDGyro(QWidget *parent = nullptr);
    ~FrameOSDGyro();

    void setup() override;
    void resetModeIndex() override;

public slots:
    void onModeChangeResponse(const QString datafisis, BaseResponse<InputModeResponseModel> mode, bool needConfirm) override;
    void onDataResponse(GyroResponseModel data) override;
    void onStreamReceive(GyroStreamModel model) override;
    void onUpdateGyroAutoUi();

signals:
    void signalChangeGyroMode(bool manual_mode);
    void signalChangeGyroData(float heading, float pitch, float roll);

private slots:
    void onModeChange(int index) override;
    void onAfterModeReset() override;
    void onTimeout() override;

    void on_pushButton_clicked();

private:
    Ui::FrameOSDGyro *ui;

    OSDService *_serviceOSD;

    void manualUiSetup() override;
    void autoUiSetup() override;

    void notConnectedUiSetup() override;
    void noDataUiSetup() override;
    void invalidDataUiSetup() override;

    bool validateInput() override;

    void setErrorInput(BaseError error);
};

#endif // FRAME_OSD_GYRO_H
