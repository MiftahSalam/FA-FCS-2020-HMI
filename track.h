#ifndef TRACK_H
#define TRACK_H

#include <QWidget>
#include <QLabel>
#include <QAction>
#include <QMenu>

#include "rceventhandler.h"
#include "tda_global.h"


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
    void identity_change_signal(int tn,Identity identity);
    void env_change_signal(int tn,Environment identity);
    void selected_req_signal(int tn);
//    void desig_request_signal(int tn,bool desig,QString desig_mode);

private slots:

    void RC_track(QPoint pos);
    void toolTipHandler(const QPoint pos);
    void identity_change();
    void environment_change();
    void selected_req_change();
//    void desig_change();
//    void desig_direct_change();
//    void desig_request(QString desig_mode);
//    void desig_feedback(int tn,bool approve,QString desig_mode);

private:
    QLabel *symbol;
    QLabel *no_track;
    QString current_symbol_image;
    RCEventHandler *rc_radarevent;
    trackParam trackDat;

    QString identity2String(Identity identity);
    QString env2String(Environment env);
//    QString desig2String(Desig desig);
//    QString desigDirect2String(DesigDirect desig_direct);
    QString fileImageLocation(Identity identity, Environment env);

    QAction *identityAction[IDENTITY_COUNT];
    QAction *envAction[ENVIRONMENT_COUNT];
    QAction *desigAction[DESIG_COUNT];
    QAction *desigDirectAction[DESIG_DIRECT_COUNT];
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
