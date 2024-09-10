#ifndef FRAME_ENGAGE_DATA_H
#define FRAME_ENGAGE_DATA_H

#include <QWidget>
#include "src/usecase/engagement/weapon_track_assign_service.h"
#include "src/usecase/engagement/stream/engagement_data_40mm_stream.h"

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

protected:
    QTimer *timer;

private slots:
    void onTimeOut();
    void onAssignmentResponseData(BaseResponse<TrackAssignResponse> resp, bool assign);

private:
    Ui::FrameEngageData *ui;

    EngagementData40mmStream *engagementDataStream;
    WeaponTrackAssignService *wtaService;

};

#endif // FRAME_ENGAGE_DATA_H
