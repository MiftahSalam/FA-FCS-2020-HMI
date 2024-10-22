#ifndef FRAME_TRACK_SELECTED_H
#define FRAME_TRACK_SELECTED_H

#include <QWidget>

namespace Ui {
class FrameTrackSelected;
}

class FrameTrackSelected : public QWidget
{
    Q_OBJECT

public:
    explicit FrameTrackSelected(QWidget *parent = nullptr);
    ~FrameTrackSelected();

private:
    Ui::FrameTrackSelected *ui;
};

#endif // FRAME_TRACK_SELECTED_H
