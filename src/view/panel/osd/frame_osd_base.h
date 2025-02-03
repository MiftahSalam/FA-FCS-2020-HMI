#ifndef FRAME_OSD_BASE_H
#define FRAME_OSD_BASE_H

#include <QTimer>

#include "src/domain/osd/entity/osd_base_entity.h"
#include "src/infra/core/base_response.h"
#include "src/infra/core/osd/model/input_mode/input_mode_cms_response_model.h"

template <typename DataStreamModel, typename DataResponseModel> class FrameOSDBase
{
public:
    virtual void setup() = 0; //ui first initialization
    virtual void resetModeIndex() = 0; //handler when set mode request failed

public slots:
    virtual void onModeChangeResponse(const QString datafisis, BaseResponse<InputModeResponseModel> mode, bool needConfirm) = 0; //handler set mode response from server
    virtual void onStreamReceive(DataStreamModel model) = 0; //handler data stream input
    virtual void onDataResponse(DataResponseModel data) = 0; //handler manual data response from server

protected:
    QTimer *timer; //timer to update ui
    QDateTime timestamp;
    OSD_MODE currentMode;  //hold current data fisis mode
    OSD_MODE prevMode;  //hold previous data fisis mode
    int currentModeIndx; //hold current data fisis mode combobox index
    int prevModeIndx; //hold previous data fisis mode combobox index
    bool afterResetModeIndx; //flag for toggling set mode reset handler

private slots:
    virtual void onModeChange(int index) = 0; //handler ui combobox mode changed event
    virtual void onAfterModeReset() = 0;  //handler when set mode request failed event
    virtual void onTimeout() = 0;

private:
    virtual void manualUiSetup() = 0; //ui manual mode method
    virtual void autoUiSetup() = 0; //ui auto mode method
    virtual void notConnectedUiSetup() = 0; //ui not connected to data stream method
    virtual void noDataUiSetup() = 0; //ui no data from stream method
    virtual void invalidDataUiSetup() = 0; //ui invalid data from stream method
    virtual bool validateInput() = 0; //ui validate data manual user input method
};

#endif // FRAME_OSD_BASE_H
