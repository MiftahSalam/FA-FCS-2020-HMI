#include "frame_text_input.h"
#include "src/view/shared/constant_ui.h"
#include "ui_frame_text_input.h"

FrameTextInput::FrameTextInput(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameTextInput)
{
    ui->setupUi(this);
}

void FrameTextInput::setup(const TextInputProp &prop)
{
    ui->labelTitle->setText(prop.title);
    ui->labelInfo->setText(prop.info);
    ui->textInput->setText(prop.defaultInputText);

    setStatusFailed();
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

FrameTextInput::~FrameTextInput()
{
    delete ui;
}
