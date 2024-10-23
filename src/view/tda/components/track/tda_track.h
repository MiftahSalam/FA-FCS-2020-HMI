#ifndef TDA_TRACK_H
#define TDA_TRACK_H

#include <QWidget>
#include <QLabel>
#include <QAction>
#include <QMenu>

#include "src/view/tda/components/track/tda_track_icon.h"
#include "src/view/tda/components/track/tda_track_id.h"
#include "src/view/tda/components/track/track_param.h"
#include "src/view/tda/events/tda_event_filter.h"
#include "src/shared/utils/track/track_utils.h"

class TdaTrack : public QWidget
{
    Q_OBJECT
public:
    explicit TdaTrack(QWidget *parent = nullptr, QSize size = QSize(10,10));

    const TrackParam *getTrackData() const;
    void updateTrackData(TrackParam param);
    void setTrackNumber(int number);
    void buildUI(TrackParam *param);
//    void setDesigLIODEnable (bool enabled);
//    void setDesigWeaponEnable (QString weapon, bool enabled);
    void setSelected(bool select);

signals:
    void identityChange_Signal(int tn,TrackUtils::Identity identity);
    void trackSelected_Signal(int tn);
    void trackDoubleClicked_Signal(int tn);
//    void envChangeSignal(int tn,TrackUtils::Environment identity);
//    void desig_request_signal(int tn,bool desig,QString desig_mode);

private slots:
    void RC_track(QPoint pos);
    void identityChange();
//    void environmentChange();
//    void desig_change();
//    void desig_direct_change();
//    void desig_request(QString desig_mode);
//    void desig_feedback(int tn,bool approve,QString desig_mode);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:    
    TdaTrackIcon *trackIconLabel;
    TdaTrackId *trackIdLabel;
    TdaEventFilter *rc_radarevent;
    TrackParam *trackData;
    QAction *identityAction[TrackUtils::IDENTITY_COUNT];
    QAction *envAction[TrackUtils::ENVIRONMENT_COUNT];
    QAction *desigAction[TrackUtils::DESIG_COUNT];
    QAction *desigDirectAction[TrackUtils::DESIG_DIRECT_COUNT];
    QMenu *identitySubMenu;
//    QMenu *envSubMenu;
//    QMenu *desigSubMenu;
//    QMenu *desigDirectSubMenu;
    int curCheckedIdentity;
    int curCheckedEnv;
    int curCheckedDesig;
//    int cur_checked_desig_direct;
};

#endif // TDA_TRACK_H
