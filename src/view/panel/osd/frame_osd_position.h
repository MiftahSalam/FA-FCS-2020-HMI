#ifndef FRAME_OSD_POSITION_H
#define FRAME_OSD_POSITION_H

#include <QWidget>

#include "src/di/di.h"
#include "src/view/shared/frame_text_input.h"

namespace Ui {
class FrameOSDPosition;
}

struct OSDPositionProp
{
    QString title;
    TextInputProp lat;
    TextInputProp lon;
};

class FrameOSDPosition : public QWidget
{
    Q_OBJECT

public:
    explicit FrameOSDPosition(QWidget *parent = nullptr);
    ~FrameOSDPosition();

    void setup(const OSDPositionProp& prop);

private:
    Ui::FrameOSDPosition *ui;
    DI* di;
};

#endif // FRAME_OSD_POSITION_H
