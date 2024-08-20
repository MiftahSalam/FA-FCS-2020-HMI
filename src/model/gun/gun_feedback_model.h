#ifndef GUN_FEEDBACK_MODEL_H
#define GUN_FEEDBACK_MODEL_H

#include <QJsonObject>

class GunFeedbackModel
{
public:
    GunFeedbackModel(bool opMode, bool remote, bool mount, bool barrelTemperature, bool gunReadyToStart,
                     bool gunReadyToFire, bool fireMode, bool blindArc, bool missAlignment, bool magazine);
    GunFeedbackModel(){}

    static GunFeedbackModel fromJsonObject(QJsonObject obj);

    bool getOpMode() const;
    bool getRemote() const;
    bool getMount() const;
    bool getBarrelTemperature() const;
    bool getGunReadyToStart() const;
    bool getGunReadyToFire() const;
    bool getFireMode() const;
    bool getBlindArc() const;
    bool getMissAlignment() const;
    bool getMagazine() const;

private:
    bool _opMode, _remote, _mount, _barrelTemp, _gunReadyToStart,
        _gunReadyToFire, _fireMode, _blindArc, _missAlignment, _magazine;
};

#endif // GUN_FEEDBACK_MODEL_H
