#ifndef FRAMETDA_H
#define FRAMETDA_H

#include <QFrame>
#include <QTimer>
#include <QPainter>
#include <QStatusBar>

namespace Ui {
class FrameTDA;
}

class FrameTDA : public QFrame
{
    Q_OBJECT

public:
    explicit FrameTDA(QWidget *parent = 0);
    ~FrameTDA();
    void setHeading(QString heading);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:

private:
    Ui::FrameTDA *ui;
    QTimer *timer;

    QStatusBar *statusBarSelectedTrack;

    float currentHeading;
};

#endif // FRAMETDA_H
