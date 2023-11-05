#ifndef FRAME_BASE_OSD_H
#define FRAME_BASE_OSD_H

#include "src/view/shared/frame_text_input.h"
#include <QFrame>

namespace Ui {
class FrameBaseOSD;
}

struct BaseOSDProp
{
    QString title;
    TextInputProp heading;
    TextInputProp pitch;
    TextInputProp roll;
};

class FrameBaseOSD : public QFrame
{
    Q_OBJECT

public:
    explicit FrameBaseOSD(QWidget *parent = nullptr);
    ~FrameBaseOSD();

    void modify(const BaseOSDProp& prop);

private:
    Ui::FrameBaseOSD *ui;
};

#endif // FRAME_BASE_OSD_H
