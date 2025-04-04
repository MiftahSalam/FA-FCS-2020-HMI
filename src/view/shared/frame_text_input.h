#ifndef FRAME_TEXT_INPUT_H
#define FRAME_TEXT_INPUT_H

#include <QFrame>

namespace Ui {
class FrameTextInput;
}

struct TextInputProp
{
    QString title;
    QString info;
    QString name;
    QString defaultInputText;
};

class FrameTextInput : public QFrame
{
    Q_OBJECT

public:
    explicit FrameTextInput(QWidget *parent = nullptr);
    ~FrameTextInput();

    void setup(const TextInputProp& props);
    void setInputEnable(bool enable);
    void setStatusOk();
    void setStatusFailed();
    void setModeManual();
    void setValue(QString val);
    QString getCurrentValue() const;

private:
    Ui::FrameTextInput *ui;
};

#endif // FRAME_TEXT_INPUT_H
