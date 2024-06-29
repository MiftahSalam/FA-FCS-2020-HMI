#include "osd_base_entity.h"

OSDBaseEntity::OSDBaseEntity(const std::string &source, const std::string &status, const bool &mode) : _source(source),
    _status(status), _manualMode(mode)
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

bool OSDBaseEntity::manualMode() const
{
    return _manualMode;
}

void OSDBaseEntity::setManualMode(bool newManualMode)
{
    _manualMode = newManualMode;
}
