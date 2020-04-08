#ifndef FRAMEWAP_H
#define FRAMEWAP_H

#include <redis++.h>
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

private:
    Ui::FrameWAP *ui;

    QString ConfigGun;
    QString ConfigTrack;
    Redis *redisTrack;
    Redis *redisGun;
    QString currentAccessStatus;
};

#endif // FRAMEWAP_H
