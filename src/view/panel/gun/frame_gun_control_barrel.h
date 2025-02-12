#ifndef FRAME_GUN_CONTROL_BARREL_H
#define FRAME_GUN_CONTROL_BARREL_H

#include "src/infra/core/gun/stream/gun_status/gun_feedback_status_model.h"
#include "src/usecase/gun/gun_manager_service.h"
#include "src/infra/core/gun/stream/gun_coverage/gun_coverage_stream.h"
#include "src/infra/core/gun/stream/gun_status/gun_feedback_status_stream.h"
#include "src/view/shared/frame_text_input.h"
#include <QFrame>

namespace Ui {
class FrameGunControlBarrel;
}

struct BarrelControlProp
{
    QString title;
    TextInputProp azimuth;
    TextInputProp elevation;
};

class FrameGunControlBarrel : public QFrame
{
    Q_OBJECT

public:
    explicit FrameGunControlBarrel(QWidget *parent = nullptr);
    ~FrameGunControlBarrel();

    void setup();

public slots:
    void onModeChangeResponse(GunModeBarrelResponse resp, bool needConfirm);
    void onBarrelDataResponse(GunCommandBarrelResponse resp, bool needConfirm);

signals:
    void signalChangeControlMode(bool manual_mode);
    void signalChangeBarrelData(float azimuth, float elevation);

private slots:
    void onModeChange(int index);
    void onTimeout();
    void onStatusStreamUpdate(GunFeedbackStatusModel model);
    void onModeCheck();

    void on_pushButtonGunBarControlApply_clicked();

private:
    Ui::FrameGunControlBarrel *ui;

    struct BarrelLimit
    {
        float min;
        float max;
    };

    GunManagerService *gunService;
    GunFeedbackStatusStream *statusStream;
    GunCoverageStream *coverageStream;

    GunBarrelModeEntity::MODE currentMode;

    void noneModeUiSetup();
    void manualModeUiSetup();
    void autoModeUiSetup();
    bool validateInput();
    void updateMode();
    void resetBarrel(bool needConfirm);
    BarrelLimit calculateBarrelLimit();

    void setupDI();
};

#endif // FRAME_GUN_CONTROL_BARREL_H
