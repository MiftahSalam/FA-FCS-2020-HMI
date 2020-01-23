#ifndef FRAMEWAP_H
#define FRAMEWAP_H

#include <QFrame>

namespace Ui {
class FrameWAP;
}

class FrameWAP : public QFrame
{
    Q_OBJECT

public:
    explicit FrameWAP(QWidget *parent = 0);
    ~FrameWAP();

private:
    Ui::FrameWAP *ui;
};

#endif // FRAMEWAP_H
