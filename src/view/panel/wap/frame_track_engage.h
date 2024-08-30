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

private slots:
    void on_pushButtonTrackEngAssign_clicked();

    void onComboBoxTrackEngTNChange(int index);

    void onComboBoxTrackEngWeaponChange(int index);

private:
    Ui::FrameTrackEngage *ui;

    void availableUiSetup(bool available);
    void resetMode();
};

#endif // FRAME_TRACK_ENGAGE_H
