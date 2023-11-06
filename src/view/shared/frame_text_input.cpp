#include "frame_text_input.h"
#include "src/view/shared/constant_ui.h"
#include "ui_frame_text_input.h"

FrameTextInput::FrameTextInput(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameTextInput)
{
    ui->setupUi(this);
}

void FrameTextInput::setup(const TextInputProp &props)
{
    ui->labelTitle->setText(props.title);
    ui->labelInfo->setText(props.info);
    ui->textInput->setText(props.defaultInputText);

    setStatusFailed();
}

void FrameTextInput::setInputEnable(bool enable)
{
    ui->textInput->setEnabled(enable);
}

void FrameTextInput::setStatusOk()
{
    ui->textInput->setStyleSheet(COLOR_OK_STYLESHEET);
}

void FrameTextInput::setStatusFailed()
{
    ui->textInput->setStyleSheet(COLOR_FAILED_STYLESHEET);

}

void FrameTextInput::setModeManual()
{
    ui->textInput->setStyleSheet(COLOR_MANUAL_STYLESHEET);
}

void FrameTextInput::setValue(QString val)
{
    setStatusOk();
    ui->textInput->setText(val);
}

FrameTextInput::~FrameTextInput()
{
    delete ui;
}
