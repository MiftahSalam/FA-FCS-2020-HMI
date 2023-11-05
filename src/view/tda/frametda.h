#ifndef FRAMETDA_H
#define FRAMETDA_H

#include <QFrame>
#include <QTimer>
#include <QPainter>
#include <QStatusBar>
#include <QObject>
#include <QPoint>
#include <QSettings>
#include <QMenu>

// ==== TDA ==== //
namespace Ui {
class FrameTDA;
}

class FrameTDA : public QFrame
{
    Q_OBJECT

public:
    explicit FrameTDA(QWidget *parent = 0);
    ~FrameTDA();

protected:

private slots:

private:
   Ui::FrameTDA *ui;

};

#endif // FRAMETDA_H
