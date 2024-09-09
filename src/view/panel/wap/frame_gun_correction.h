#ifndef FRAME_GUN_CORRECTION_H
#define FRAME_GUN_CORRECTION_H

#include "src/model/engagement/engagement_data_model.h"
#include "src/usecase/engagement/cms/engagement_data_correction_40mm_service.h"
#include <QWidget>


namespace Ui {
class FrameGunCorrection;
}

class FrameGunCorrection : public QWidget
{
    Q_OBJECT

public:
    explicit FrameGunCorrection(QWidget *parent = nullptr);
    ~FrameGunCorrection();

private slots:
    void on_pushButtonCorrectionApply_clicked();

private:
    Ui::FrameGunCorrection *ui;

    EngagementDataCorrection40mmService *engageCorrService;

    bool validateInput();
    void setupDI();


};

#endif // FRAME_GUN_CORRECTION_H
