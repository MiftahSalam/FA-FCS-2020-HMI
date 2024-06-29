#ifndef OSDCMS_H
#define OSDCMS_H

#include <QNetworkReply>
#include <QObject>

#include "src/domain/osd/repository/osd_base_repository.h"
#include "src/shared/config/osd_cms_config.h"
#include "src/usecase/osd/cms/osd_cms_input_mode.h"
#include "src/usecase/osd/cms/osd_cms_position_data.h"

class OSDCMS : public QObject
{
    Q_OBJECT
public:
    OSDCMS(QObject *parent = nullptr, OSDCmsConfig *cmsConfig = nullptr);

    OSDCMSPositionData *getServiceOSDCMSPosition() const;
    OSDCMSInputMode *getServiceOSDCMSMode() const;
    OSDBaseRepository *getRepoOSDPosition() const;

private:
    OSDCmsConfig *cfgCms;
    OSDCMSPositionData *serviceOSDCMSPosition;
    OSDCMSInputMode *serviceOSDCMSMode;
    OSDBaseRepository *repoOSDPosition;
};

#endif // OSDCMS_H
