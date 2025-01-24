#ifndef OSD_SERVICE_H
#define OSD_SERVICE_H

#include <QObject>

#include "src/infra/core/base_response.h"
#include "src/infra/core/osd/cms/osd_cms.h"
#include "src/infra/core/osd/model/inertia/gyro_model.h"
#include "src/infra/core/osd/model/input_mode/input_mode_model.h"
#include "src/infra/store/osd/osd_repository.h"

class OSDService : public QObject
{
    Q_OBJECT
public:
    OSDService(OSDService &other) = delete;
    void operator=(const OSDService&) = delete;
    ~OSDService() override;

    static OSDService* getInstance(
        QObject *parent,
        OSDRepository* osdRepo,
        OSDCMS* osdCms
        );

signals:
    void signal_processedSetModeResponse(const QString datafisis, BaseResponse<InputModeModel> response, bool needConfirm);

public slots:
    void onUpdateManualDataGyro(BaseResponse<GyroModel> response);
    void onUpdateInputMode(const QString datafisis, BaseResponse<InputModeModel> response, bool needConfirm);

protected:
    OSDService(
        QObject *parent = nullptr,
        OSDRepository* osdRepo = nullptr,
        OSDCMS* osdCms = nullptr
        );

private:
    static OSDService *instance;

    OSDRepository *repoOsd;
    OSDCMS *cmsOsd;
};

#endif // OSD_SERVICE_H
