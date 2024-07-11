#ifndef GUNSTATUSCOMMAND_H
#define GUNSTATUSCOMMAND_H


class GunStatusCommandEntity
{
public:
    GunStatusCommandEntity(bool mount, bool single_shot, bool fire, bool proximity, bool siren);


    bool mount() const;
    void setMount(bool newMount);
    bool single_shot() const;
    void setSingle_shot(bool newSingle_shot);
    bool fire() const;
    void setFire(bool newFire);
    bool proximity() const;
    void setProximity(bool newProximity);
    bool siren() const;
    void setSiren(bool newSiren);

private:
    bool _mount;
    bool _single_shot;
    bool _fire;
    bool _proximity;
    bool _siren;
};

#endif // GUNSTATUSCOMMAND_H
