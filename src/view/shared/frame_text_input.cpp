#include "frame_text_input.h"
#include "ui_frame_text_input.h"

FrameTextInput::FrameTextInput(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameTextInput)
{
    ui->setupUi(this);
}

void FrameTextInput::modify(const TextInputProp &prop)
{
    ui->labelTitle->setText(prop.title);
    ui->labelInfo->setText(prop.info);
    ui->textInput->setText(prop.defaultInputText);

}

FrameTextInput::~FrameTextInput()
{
    delete ui;
}
