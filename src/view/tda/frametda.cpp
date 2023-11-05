#include "frametda.h"
#include "ui_frametda.h"

#include <math.h>
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>

FrameTDA::FrameTDA(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameTDA)
{
    ui->setupUi(this);

}

FrameTDA::~FrameTDA()
{
    delete ui;
}
