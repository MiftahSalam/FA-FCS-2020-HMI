#ifndef FRAME_GUN_DATA_MONITORING_H
#define FRAME_GUN_DATA_MONITORING_H

#include "src/usecase/gun/gun_manager_service.h"
#include "src/usecase/gun/stream/gun_feedback_status_stream.h"
#include "src/usecase/gun/stream/gun_feedback_barrel_stream.h"
#include <QFrame>
#include <QTimer>

namespace Ui {
class FrameGunDataMonitoring;
}

class FrameGunDataMonitoring : public QFrame
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

private slots:
    void onTimeout();

private:
    Ui::FrameGunDataMonitoring *ui;
    GunFeedbackStatusStream* gunStatusStream;
    GunFeedbackBarrelStream* gunBarrelStream;
    GunManagerService* gunManagerService;

    void updateTechStatus();
    void updateBarrelStatus();

    void notConnectedUiSetupBarrel();
    void noDataUiSetupBarrel();
    void noDataUiSetupStatus();
    void invalidDataUiSetupBarrel();
    void offlineUiSetup();
    void updateOperationalUiSetup();
};

#endif // FRAME_GUN_DATA_MONITORING_H
