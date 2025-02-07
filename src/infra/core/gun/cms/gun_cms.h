#ifndef GUN_CMS_H
#define GUN_CMS_H

#include <QObject>

#include "src/infra/core/gun/cms/mode/gun_cms_command_barrel_mode.h"
#include "src/shared/config/gun_cms_config.h"

class GunCMS : public QObject
{
    Q_OBJECT
public:
    explicit GunCMS(QObject *parent = nullptr, GunCmsConfig *cmsConfig = nullptr);

    GunCmsCommandBarrelMode *getGunCMSMode() const;

private:
    GunCmsConfig *cfgCms;
    GunCmsCommandBarrelMode *gunCMSMode;
};

#endif // GUN_CMS_H
