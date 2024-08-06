#ifndef TRACK_H
#define TRACK_H

#include <QWidget>
#include <QLabel>
#include <QAction>
#include <QMenu>

#include "src/view/tda/events/tda_event_filter.h"
#include "src/shared/utils/track/track_utils.h"
#include "tda_track_id.h"

class track : public QWidget
{
    Q_OBJECT
public:

    explicit track(QWidget *parent = 0, QSize size = QSize(10,10));

    void updateData(trackParam param);
    void setTrackNumber(int number);
    void buildUI(trackParam param);
//    void setDesigLIODEnable (bool enabled);
//    void setDesigWeaponEnable (QString weapon, bool enabled);
    void setSelected(bool select);

signals:
    void identity_change_signal(int tn,TrackUtils::Identity identity);
    void env_change_signal(int tn,TrackUtils::Environment identity);
//    void desig_request_signal(int tn,bool desig,QString desig_mode);

private slots:

    void RC_track(QPoint pos);
    void identity_change();
    void environment_change();
//    void desig_change();
//    void desig_direct_change();
//    void desig_request(QString desig_mode);
//    void desig_feedback(int tn,bool approve,QString desig_mode);

private:
    QLabel *symbol;
    QLabel *no_track;
    QString current_symbol_image;
    TdaEventFilter *rc_radarevent;
    trackParam trackDat;

    QString identity2String(TrackUtils::Identity identity);
    QString env2String(TrackUtils::Environment env);
//    QString desig2String(Desig desig);
//    QString desigDirect2String(DesigDirect desig_direct);
    QString fileImageLocation(TrackUtils::Identity identity, TrackUtils::Environment env);

    QAction *identityAction[TrackUtils::IDENTITY_COUNT];
    QAction *envAction[TrackUtils::ENVIRONMENT_COUNT];
    QAction *desigAction[TrackUtils::DESIG_COUNT];
    QAction *desigDirectAction[TrackUtils::DESIG_DIRECT_COUNT];
    QMenu *identitySubMenu;
    QMenu *envSubMenu;
//    QMenu *desigSubMenu;
//    QMenu *desigDirectSubMenu;
    int cur_checked_identity;
    int cur_checked_env;
    int cur_checked_desig;
//    int cur_checked_desig_direct;

};

#endif // TRACK_H
