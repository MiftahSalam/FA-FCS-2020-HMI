#ifndef FRAME_GUN_CORRECTION_H
#define FRAME_GUN_CORRECTION_H

#include "src/model/engagement/engagement_data_model.h"
#include "src/usecase/engagement/weapon_track_assign_service.h"
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

    void setup();

private slots:
    void on_pushButtonCorrectionApply_clicked();
    void on_engageCorrResponse(BaseResponse<EngagementCorrectionSetResponse> resp);

private:
    Ui::FrameGunCorrection *ui;

    WeaponTrackAssignService *engageService;

    bool validateInput();
    void setupDI();


};

#endif // FRAME_GUN_CORRECTION_H
