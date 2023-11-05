#ifndef FRAME_OSD_POSITION_H
#define FRAME_OSD_POSITION_H

#include "src/view/shared/frame_text_input.h"
#include <QWidget>

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

    void modify(const OSDPositionProp& prop);

private:
    Ui::FrameOSDPosition *ui;
};

#endif // FRAME_OSD_POSITION_H
