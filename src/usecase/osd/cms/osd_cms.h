#ifndef OSDCMS_H
#define OSDCMS_H

#include <QNetworkReply>
#include <QObject>

#include "src/shared/config/osd_cms_config.h"
#include "src/usecase/osd/cms/osd_cms_position_data.h"

class OSDCMS : public QObject
{
    Q_OBJECT
public:
    OSDCMS(QObject *parent = nullptr, OSDCmsConfig *cmsConfig = nullptr);

    OSDCMSPositionData *getServiceOSDCMSPosition() const;

private:
    OSDCmsConfig *cfgCms;
    OSDCMSPositionData *serviceOSDCMSPosition;
};

#endif // OSDCMS_H
