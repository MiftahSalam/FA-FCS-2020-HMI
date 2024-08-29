#ifndef FRAME_GUN_CONTROL_STATUS_H
#define FRAME_GUN_CONTROL_STATUS_H

#include "src/model/base_response.h"
#include "src/model/gun/cms/gun_command_status_response.h"
#include "src/usecase/gun/gun_manager_service.h"
#include <QFrame>

namespace Ui {
class FrameGunControlStatus;
}

class FrameGunControlStatus : public QFrame
{
    Q_OBJECT

public:
    explicit FrameGunControlStatus(QWidget *parent = nullptr);
    ~FrameGunControlStatus();

    void setup();

public slots:
    void onStatusDataResponse(BaseResponse<GunCommandStatusResponse> resp);

private slots:
    void onModeCheck();

    void on_pushButtonControlMount_clicked(bool checked);

    void on_pushButtonControlSingleshoot_clicked(bool checked);

    void on_pushButtonControlFire_clicked(bool checked);

    void on_pushButtonControlProxFuze_clicked(bool checked);

    void on_pushButtonControlSiren_clicked(bool checked);

private:
    Ui::FrameGunControlStatus *ui;

    GunManagerService *gunService;

    GunManagerService::OPERATIONAL_STATUS currentOpMode;
    GunCommandStatusRequest previousStatus;

    void availableUiSetup(bool available);
    void updateMode();
    void resetStatus();

    void setupDI();
};

#endif // FRAME_GUN_CONTROL_STATUS_H
