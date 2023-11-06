#ifndef FRAMEOSD_H
#define FRAMEOSD_H

#include "src/usecase/osd/cms/osd_cms_input_mode.h"
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

signals:
    void signalOnResponse(InputModeModel resp);

private slots:
    void onChangePositionMode(bool manual_mode);
    void onChangeInputModeResponse(BaseResponse<InputModeModel> resp);

private:
    Ui::FrameOSD *ui;
    OSDCMSInputMode* _cmsMode;
    InputModeModel currentMode;
    QString lastUpdateMode;

    void setup();
    void resetToPrevMode();
};

#endif // FRAMEOSD_H
