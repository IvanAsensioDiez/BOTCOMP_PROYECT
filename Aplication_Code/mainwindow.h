#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "robotcontroller.h"
#include "canmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_btnConnect_clicked();

    void on_btnForward_clicked();
    void on_btnBackward_clicked();

    void on_btnLeft_clicked();
    void on_btnRight_clicked();

    void on_btnForwardLeft_clicked();
    void on_btnForwardRight_clicked();
    void on_btnBackwardLeft_clicked();
    void on_btnBackwardRight_clicked();

    void on_btnStop_clicked();

    void on_btnSetZero_clicked();
    void on_btnGoToRest_clicked();

private:

    Ui::MainWindow *ui;

    void selectButton(QPushButton *button);

    CanManager canManager;
    RobotController robot;
};

#endif