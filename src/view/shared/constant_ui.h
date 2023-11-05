#ifndef CONSTANT_UI_H
#define CONSTANT_UI_H

#include <QString>

const QString COLOR_OK_STYLESHEET = "color:rgb(0, 255, 0);";
const QString COLOR_FAILED_STYLESHEET = "color:rgb(255, 0, 0);";
const QString COLOR_MANUAL_STYLESHEET = "color:rgb(255, 255, 255);";

enum OSD_MODE {
    AUTO = 0,
    MANUAL = 1
};

#endif // CONSTANT_UI_H
