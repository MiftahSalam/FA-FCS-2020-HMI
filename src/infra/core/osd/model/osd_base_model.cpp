#include "osd_base_model.h"

OSDBaseModel::OSDBaseModel() : _err(ErrUnknown())
{
}

OSDBaseModel::OSDBaseModel(
    const std::string &source,
    const std::string &status,
    const BaseError &err,
    const OSD_MODE &mode)
    : _source(source),
    _status(status),
    _err(err),
    _mode(mode)
{}

std::string OSDBaseModel::source() const
{
    return _source;
}

void OSDBaseModel::setSource(const std::string &newSource)
{
    _source = newSource;
}

std::string OSDBaseModel::status() const
{
    return _status;
}

void OSDBaseModel::setStatus(const std::string &newStatus)
{
    _status = newStatus;
}

OSD_MODE OSDBaseModel::mode() const
{
    return _mode;
}

void OSDBaseModel::setMode(OSD_MODE newManualMode)
{
    _mode = newManualMode;
}

BaseError OSDBaseModel::err() const
{
    return _err;
}

void OSDBaseModel::setErr(const BaseError &newErr)
{
    _err = newErr;
}
