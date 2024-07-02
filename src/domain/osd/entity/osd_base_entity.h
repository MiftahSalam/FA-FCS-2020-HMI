#ifndef OSD_BASE_ENTITY_H
#define OSD_BASE_ENTITY_H

#include "src/view/shared/constant_ui.h"
#include <string>

class OSDBaseEntity
{
public:
    OSDBaseEntity(const std::string &source, const std::string &status, const OSD_MODE &mode);

    std::string source() const;
    void setSource(const std::string &newSource);
    std::string status() const;
    void setStatus(const std::string &newStatus);

    OSD_MODE mode() const;
    void setMode(OSD_MODE newManualMode);

protected:
    std::string _source;
    std::string _status;
    OSD_MODE _mode;
};
#endif // OSD_BASE_ENTITY_H
