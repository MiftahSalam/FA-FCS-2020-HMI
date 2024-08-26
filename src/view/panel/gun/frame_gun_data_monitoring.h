#ifndef FRAME_GUN_DATA_MONITORING_H
#define FRAME_GUN_DATA_MONITORING_H

// #include "src/usecase/osd/cms/osd_cms_input_mode.h"
#include "src/usecase/gun/stream/gun_feedback_status_stream.h"
#include "src/usecase/gun/stream/gun_feedback_barrel_stream.h"
// #include "src/model/gun/gun_feedback_barrel_model.h"
#include "src/view/panel/gun/frame_gun_base.h"
#include "src/view/shared/frame_text_input.h"
#include <QFrame>
#include <QTimer>

namespace Ui {
class FrameGunDataMonitoring;
}

class FrameGunDataMonitoring : public QFrame/*, public FrameGunBase<GunFeedbackStatusModel, GunFeedbackStatusModel>*/
{
    Q_OBJECT

public:
    explicit FrameGunDataMonitoring(QWidget *parent = nullptr);
    ~FrameGunDataMonitoring();

public slots:
    void onStreamStatusReceive(GunFeedbackStatusModel model);
    void onStreamBarrelReceive(GunFeedbackBarrelModel model);

protected:
    QTimer *timer;

signals:
    // void signalChangeSpeedMode(bool manual_mode);
    // void signalChangeSpeedData(float speed, float course);

private slots:
    // void onModeChange(int index) override;
    // void onAfterModeReset() override;
    void onTimeout();

private:
    Ui::FrameGunDataMonitoring *ui;
    GunFeedbackStatusStream* gunStatusStream;
    GunFeedbackBarrelStream* gunBarrelStream;

    void barrelFailedUI();

    // void manualUiSetup() override;
    // void autoUiSetup() override;

    // void notConnectedUiSetup() override;
    // void noDataUiSetup() override;
    // void invalidDataUiSetup() override;

    // bool validateInput() override;
};

#endif // FRAME_GUN_DATA_MONITORING_H
