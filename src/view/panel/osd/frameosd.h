#ifndef FRAMEOSD_H
#define FRAMEOSD_H

#include <QFrame>
#include <QDebug>
#include <QTimer>
#include <QSettings>

namespace Ui {
class FrameOSD;
}

class FrameOSD : public QFrame
{
    Q_OBJECT

public:
    explicit FrameOSD(QWidget *parent = 0);
    ~FrameOSD();

private:
    Ui::FrameOSD *ui;


};

#endif // FRAMEOSD_H
