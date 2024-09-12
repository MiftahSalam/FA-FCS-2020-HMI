#ifndef FRAME_GUN_CORRECTION_H
#define FRAME_GUN_CORRECTION_H

#include "src/usecase/engagement/weapon_track_assign_service.h"
#include "src/usecase/weapon_assign/weapon_assign_service.h"
#include <QWidget>


namespace Ui {
class FrameGunCorrection;
}

class FrameGunCorrection : public QWidget
{
    Q_OBJECT

public:
    explicit FrameGunCorrection(QWidget *parent = nullptr);
    ~FrameGunCorrection();

private slots:
    void on_pushButtonCorrectionApply_clicked();
    void on_engageCorrResponse(BaseResponse<EngagementCorrectionSetResponse> resp);
    void onAssignmentResponseData(BaseResponse<TrackAssignResponse> resp, bool assign);
    void onAssignModeChange(const QString &weapon, const WeaponAssign::WeaponAssignMode &mode);

private:
    Ui::FrameGunCorrection *ui;

    WeaponTrackAssignService *engageService;
    WeaponAssignService *waService;

    bool validateInput();
    void setupDI();
};

#endif // FRAME_GUN_CORRECTION_H
