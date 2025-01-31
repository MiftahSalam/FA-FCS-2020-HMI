#ifndef OSD_BASE_MODEL_H
#define OSD_BASE_MODEL_H

#include "src/domain/osd/entity/osd_base_entity.h"
#include "src/shared/common/errors/err_base.h"
#include <string>

class OSDBaseModel
{
public:
    OSDBaseModel();
    OSDBaseModel(
        const std::string &source,
        const std::string &status,
        const BaseError &err,
        const OSD_MODE &mode
        );

    std::string source() const;
    void setSource(const std::string &newSource);
    std::string status() const;
    void setStatus(const std::string &newStatus);

    OSD_MODE mode() const;
    void setMode(OSD_MODE newManualMode);

    BaseError err() const;
    void setErr(const BaseError &newErr);

protected:
    std::string _source;
    std::string _status;
    BaseError _err;
    OSD_MODE _mode;
};

#endif // OSD_BASE_MODEL_H
