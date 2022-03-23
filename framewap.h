#ifndef FRAMEWAP_H
#define FRAMEWAP_H

#ifdef WIN32
#include <QtRedis>
#else
#include "qredis/redis++.h"
#endif
#include <QFrame>
#include <QTimer>


namespace Ui {
class FrameWAP;
}

class FrameWAP : public QFrame
{
    Q_OBJECT

public:
    explicit FrameWAP(QWidget *parent = 0);
    ~FrameWAP();
    void setConfig (QString ConfigTrack, QString ConfigGun );
    void setAccessStatus (QString access_status);

private slots:

    void on_comboBoxWAPWeapon_activated(const QString &arg1);

    void on_comboBoxTrackEngTN_activated(const QString &arg1);

    void on_pushButtonTrackEngAssign_clicked();

    void on_comboBoxWAPMode_activated(const QString &arg1);

    void on_pushButtonCorrectionApply_clicked();

private:
    Ui::FrameWAP *ui;

    QList <int> tnList;
    QString ConfigGun;
    QString ConfigTrack;
#ifdef WIN32
    QtRedis *redisTrack;
    QtRedis *redisGun;
#else
    Redis *redisTrack;
    Redis *redisGun;
#endif
    QString currentAccessStatus;
    QString curStatusString;
    QString curEngStatus;

    void reconnecRedis();

};

#endif // FRAMEWAP_H
