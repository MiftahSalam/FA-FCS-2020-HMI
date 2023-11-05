#ifndef FRAME_TRACK_ENGAGE_H
#define FRAME_TRACK_ENGAGE_H

#include <QWidget>

namespace Ui {
class FrameTrackEngage;
}

class FrameTrackEngage : public QWidget
{
    Q_OBJECT

public:
    explicit FrameTrackEngage(QWidget *parent = nullptr);
    ~FrameTrackEngage();

private:
    Ui::FrameTrackEngage *ui;
};

#endif // FRAME_TRACK_ENGAGE_H
