#ifndef FRAME_OSD_POSITION_H
#define FRAME_OSD_POSITION_H

#include <QWidget>

#include "src/usecase/osd/cms/osd_cms_input_mode.h"
#include "src/usecase/osd/stream/osd_stream_position.h"
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

class FrameOSDPosition : public QWidget, public FrameOSDBase<PositionModel, PositionModel>
{
    Q_OBJECT

public:
    explicit FrameOSDPosition(QWidget *parent = nullptr);
    ~FrameOSDPosition();

    void setup() override;
    void resetModeIndex() override;

public slots:
    void onModeChangeResponse(const QString datafisis, BaseResponse<InputModeModel> resp, bool needConfirm) override;
    void onDataResponse(BaseResponse<PositionModel> data) override;
    void onStreamReceive(PositionModel model) override;
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
    OSDCMSPositionData *_cmsPos;
    OSDCMSInputMode *_cmsMode;
    OSDStreamPosition* _streamPos;

    // FrameOSDBase interface
    void manualUiSetup() override;
    void autoUiSetup() override;

    void notConnectedUiSetup() override;
    void noDataUiSetup() override;
    void invalidDataUiSetup() override;

    bool validateInput() override;
};

#endif // FRAME_OSD_POSITION_H
