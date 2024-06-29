#ifndef OSD_BASE_ENTITY_H
#define OSD_BASE_ENTITY_H

#include <string>

class OSDBaseEntity
{
public:
    OSDBaseEntity(const std::string &source, const std::string &status, const bool &mode);

    std::string source() const;
    void setSource(const std::string &newSource);
    std::string status() const;
    void setStatus(const std::string &newStatus);

    bool manualMode() const;
    void setManualMode(bool newManualMode);

protected:
    std::string _source;
    std::string _status;
    bool _manualMode;
};
#endif // OSD_BASE_ENTITY_H
