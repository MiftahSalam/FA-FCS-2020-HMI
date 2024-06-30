#ifndef OSD_BASE_REPOSITORY_H
#define OSD_BASE_REPOSITORY_H

#include "src/view/shared/constant_ui.h"
#include <string>

class OSDBaseRepository
{
public:
    virtual void SetSource(const std::string &source) = 0;
    virtual void SetStatus(const std::string &status) = 0;
    virtual void SetMode(const OSD_MODE &mode) = 0;
};
#endif // OSD_BASE_REPOSITORY_H
