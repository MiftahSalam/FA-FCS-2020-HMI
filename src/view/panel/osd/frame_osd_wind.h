#ifndef FRAME_OSD_WIND_H
#define FRAME_OSD_WIND_H

#include <QWidget>

#include "src/usecase/osd/osd_service.h"
#include "src/view/panel/osd/frame_osd_base.h"
#include "src/view/shared/frame_text_input.h"

namespace Ui {
class FrameOSDWind;
}

struct OSDWindProp
{
    QString title;
    TextInputProp speed;
    TextInputProp direction;
};

class FrameOSDWind : public QWidget, public FrameOSDBase<WindStreamModel, WindModel>
{
    Q_OBJECT

public:
    explicit FrameOSDWind(QWidget *parent = nullptr);
    ~FrameOSDWind();

    void setup() override;
    void resetModeIndex() override;

public slots:
    void onModeChangeResponse(const QString datafisis, BaseResponse<InputModeModel> resp, bool needConfirm) override;
    void onDataResponse(WindModel data) override;
    void onStreamReceive(WindStreamModel model) override;
    //added by riyadhi
    void onUpdateWindAutoUi();

signals:
    void signalChangeWindMode(bool manual_mode);
    void signalChangeWindData(float _speed, float _direction);

private slots:
    void onModeChange(int index) override;
    void onAfterModeReset() override;
    void onTimeout() override;

    void on_pushButton_clicked();

private:
    Ui::FrameOSDWind *ui;

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

#endif // FRAME_OSD_WIND_H
