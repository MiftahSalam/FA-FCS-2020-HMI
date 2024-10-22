#ifndef FRAME_TRACK_ENGAGE_H
#define FRAME_TRACK_ENGAGE_H

#include "src/infra/store/track/track_repository_listener.h"
#include "src/usecase/engagement/weapon_track_assign_service.h"
#include "src/usecase/gun/gun_manager_service.h"
#include "src/usecase/weapon_assign/weapon_assign_service.h"
#include <QWidget>

namespace Ui {
class FrameTrackEngage;
}

class FrameTrackEngage : public QWidget, public TrackRepositoryListener
{
    Q_OBJECT

public:
    explicit FrameTrackEngage(QWidget *parent = nullptr);
    ~FrameTrackEngage();

    // TrackRepositoryListener interface
    void OnTracksAdded(std::list<TrackBaseEntity *> tnList) override;
    void OnTracksRemoved(std::list<int> tnIdList) override;
    void OnTrackPropertyChanged(int tn, TrackBaseEntity *track) override;
    void OnTrackSelectedChanged(int tn) override;

private slots:
    void on_pushButtonTrackEngAssign_clicked();

    void onComboBoxTrackEngTNChange(int index);

    void onComboBoxTrackEngWeaponChange(int index);

    void onGunCheck();
    void onAssignModeChange(const QString &weapon, const WeaponAssign::WeaponAssignMode &mode);
    void onAssignmentResponseData(BaseResponse<TrackAssignResponse> resp, bool assign);


private:
    Ui::FrameTrackEngage *ui;

    WeaponAssignService *_waService;
    WeaponTrackAssignService *_wtaService;
    GunManagerService *_gunService;

    GunBarrelModeEntity::MODE currentGunMode;

    void availableUiSetup(bool available);
    void noWeaponUI();
    void initWeaponList();
    void resetMode();
    void setupDI();
};
#endif // FRAME_TRACK_ENGAGE_H
