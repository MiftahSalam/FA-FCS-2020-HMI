#ifndef FRAME_OSD_POSITION_H
#define FRAME_OSD_POSITION_H

#include <QWidget>

#include "src/usecase/osd/osd_service.h"
#include "src/view/panel/osd/frame_osd_base.h"
#include "src/view/shared/frame_text_input.h"

namespace Ui {
class FrameOSDPosition;
}

struct OSDPositionProp
{
    QString title;
    TextInputProp lat;
    TextInputProp lon;
};

class FrameOSDPosition : public QWidget, public FrameOSDBase<PositionStreamModel, PositionResponseModel>
{
    Q_OBJECT

public:
    explicit FrameOSDPosition(QWidget *parent = nullptr);
    ~FrameOSDPosition();

    void setup() override;
    void resetModeIndex() override;

public slots:
    void onModeChangeResponse(const QString datafisis, BaseResponse<InputModeResponseModel> resp, bool needConfirm) override;
    void onDataResponse(PositionResponseModel data) override;
    void onStreamReceive(PositionStreamModel model) override;
    //added by riyadhi
    void onUpdatePositionAutoUi();

signals:
    void signalChangePositionMode(bool manual_mode);
    void signalChangePositionData(float lat, float lon);

private slots:
    void onModeChange(int index) override;
    void onAfterModeReset() override;
    void onTimeout() override;

    void on_pushButton_clicked();

private:
    Ui::FrameOSDPosition *ui;

    OSDService *_serviceOSD;

    // FrameOSDBase interface
    void manualUiSetup() override;
    void autoUiSetup() override;

    void notConnectedUiSetup() override;
    void noDataUiSetup() override;
    void invalidDataUiSetup() override;

    bool validateInput() override;

    void setErrorInput(BaseError error);
};

#endif // FRAME_OSD_POSITION_H
