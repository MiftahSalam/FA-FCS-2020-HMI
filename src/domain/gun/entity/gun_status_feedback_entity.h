#ifndef GUNSTATUSFEEDBACKENTITY_H
#define GUNSTATUSFEEDBACKENTITY_H


class GunStatusFeedbackEntity
{
public:
    GunStatusFeedbackEntity(
            bool opMode,
            bool remote,
            bool mount,
            bool barrelTemperature,
            bool gunReadyToStart,
            bool gunReadyToFire,
            bool fireMode,
            bool blindArc,
            bool missAlignment,
            bool magazine
            );
    GunStatusFeedbackEntity();

    bool opMode() const;
    void setOpMode(bool newOpMode);
    bool remote() const;
    void setRemote(bool newRemote);
    bool mount() const;
    void setMount(bool newMount);
    bool barrelTemperature() const;
    void setBarrelTemperature(bool newBarrelTemperature);
    bool gunReadyToStart() const;
    void setGunReadyToStart(bool newGunReadyToStart);
    bool gunReadyToFire() const;
    void setGunReadyToFire(bool newGunReadyToFire);
    bool fireMode() const;
    void setFireMode(bool newFireMode);
    bool blindArc() const;
    void setBlindArc(bool newBlindArc);
    bool missAlignment() const;
    void setMissAlignment(bool newMissAlignment);
    bool magazine() const;
    void setMagazine(bool newMagazine);

private:
    bool _opMode;
    bool _remote;
    bool _mount;
    bool _barrelTemperature;
    bool _gunReadyToStart;
    bool _gunReadyToFire;
    bool _fireMode;
    bool _blindArc;
    bool _missAlignment;
    bool _magazine;
};

#endif // GUNSTATUSFEEDBACKENTITY_H
