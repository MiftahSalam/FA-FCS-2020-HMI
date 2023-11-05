#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSettings>

#include "src/di/di.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, DI* di = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonExit_clicked();

private:
    Ui::MainWindow *ui;

    DI* _di;
};

#endif // MAINWINDOW_H
