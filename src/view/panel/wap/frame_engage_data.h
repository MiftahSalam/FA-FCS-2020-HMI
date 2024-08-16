#ifndef FRAME_ENGAGE_DATA_H
#define FRAME_ENGAGE_DATA_H

#include <QWidget>

namespace Ui {
class FrameEngageData;
}

class FrameEngageData : public QWidget
{
    Q_OBJECT

public:
    explicit FrameEngageData(QWidget *parent = nullptr);
    ~FrameEngageData();

private:
    Ui::FrameEngageData *ui;
};

#endif // FRAME_ENGAGE_DATA_H
