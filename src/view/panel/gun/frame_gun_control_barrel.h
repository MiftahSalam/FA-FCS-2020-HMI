#ifndef FRAME_GUN_CONTROL_BARREL_H
#define FRAME_GUN_CONTROL_BARREL_H

#include "src/view/shared/frame_text_input.h"
#include <QFrame>

namespace Ui {
class FrameGunControlBarrel;
}

struct BarrelControlProp
{
    QString title;
    TextInputProp azimuth;
    TextInputProp elevation;
};

class FrameGunControlBarrel : public QFrame
{
    Q_OBJECT

public:
    explicit FrameGunControlBarrel(QWidget *parent = nullptr);
    ~FrameGunControlBarrel();

    void setup();

public slots:
//    void onModeChangeResponse(const QString datafisis, BaseResponse<InputModeModel> mode, bool needConfirm);
    void onDataResponse(BaseResponse<GyroModel> data);

signals:
    void signalChangeControlMode(bool manual_mode);
    void signalChangeBarrelData(float azimuth, float elevation);

private slots:
    void onModeChange(int index);
    void onTimeout();

    void on_pushButton_clicked();

private:
    Ui::FrameGunControlBarrel *ui;

    void manualUiSetup();
    void autoUiSetup();
    bool validateInput();

};

#endif // FRAME_GUN_CONTROL_BARREL_H
