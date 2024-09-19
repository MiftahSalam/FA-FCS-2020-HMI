#ifndef FRAME_ENGAGE_DATA_H
#define FRAME_ENGAGE_DATA_H

#include <QWidget>
#include "src/usecase/engagement/weapon_track_assign_service.h"
#include "src/usecase/engagement/stream/engagement_data_40mm_stream.h"
#include "src/usecase/weapon_assign/weapon_assign_service.h"

namespace Ui {
class FrameEngageData;
}

class FrameEngageData : public QWidget
{
    Q_OBJECT

public:
    explicit FrameEngageData(QWidget *parent = nullptr);
    ~FrameEngageData();

public slots:
    void OnStreamEngegementDataReceived(EngagementDataModel model);

private slots:
    void onTimeOut();
    void onAssignmentResponseData(BaseResponse<TrackAssignResponse> resp, bool assign);
    void onAssignModeChange(const QString &weapon, const WeaponAssign::WeaponAssignMode &mode);

private:
    Ui::FrameEngageData *ui;
    QTimer *timer;

    EngagementData40mmStream *engagementDataStream;
    WeaponTrackAssignService *wtaService;
    WeaponAssignService *waService;
    BaseError currError;

    void resetEngage(const QString weapon);
};

#endif // FRAME_ENGAGE_DATA_H
