#include "osd_base_entity.h"

OSDBaseEntity::OSDBaseEntity(const std::string &source, const std::string &status, const OSD_MODE &mode) : _source(source),
    _status(status), _mode(mode)
{}

std::string OSDBaseEntity::source() const
{
    return _source;
}

void OSDBaseEntity::setSource(const std::string &newSource)
{
    _source = newSource;
}

std::string OSDBaseEntity::status() const
{
    return _status;
}

void OSDBaseEntity::setStatus(const std::string &newStatus)
{
    _status = newStatus;
}

OSD_MODE OSDBaseEntity::mode() const
{
    return _mode;
}

void OSDBaseEntity::setMode(OSD_MODE newManualMode)
{
    _mode = newManualMode;
}
