#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPortInfo>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), robot(&canManager){
    ui->setupUi(this);

    for (const QSerialPortInfo &port : QSerialPortInfo::availablePorts()){
        ui->comboPort->addItem(port.portName());
    }

    connect(&canManager, &CanManager::messageMonitor, this, [this](const QString &text){
        ui->txtMonitor->append(text);
    });
    connect(&canManager, &CanManager::distanceUpdated, this, [this](double d1, double d2){
        ui->lblDistM1->setText(QString("Distance: %1 m").arg(d1, 0, 'f', 3));
        ui->lblDistM2->setText(QString("Distance: %1 m").arg(d2, 0, 'f', 3));
    });
    connect(&canManager, &CanManager::speedUpdated, this, [this](double s1,double s2){
       ui->lblSpeedM1->setText(QString("Speed: %1 m/s").arg(s1, 0, 'f', 3));
       ui->lblSpeedM2->setText(QString("Speed: %1 m/s").arg(s2, 0, 'f', 3));
   });
    connect(&canManager, &CanManager::angleUpdated, this, [this](float angle){
        ui->lblAngle->setText(QString::number(angle, 'f', 1) + "°");
        ui->dialOrientation->setValue(static_cast<int>(angle));
    });
}
MainWindow::~MainWindow(){delete ui;}

void MainWindow::on_btnConnect_clicked(){
    QString puerto = ui->comboPort->currentText();

    qDebug() << "Trying to open:" << puerto;

    if(canManager.connectPort(puerto)){qDebug() << "CONNECTED"; ui->lblStatus->setText("Connected");}
    else                              {qDebug() << "ERROR"; ui->lblStatus->setText("Error");}
}

void MainWindow::on_btnForward_clicked(){
    selectButton(ui->btnForward);
    robot.forward(ui->spinMotor1->value());
}

void MainWindow::on_btnBackward_clicked(){
    selectButton(ui->btnBackward);
    robot.backward(ui->spinMotor1->value());
}

void MainWindow::on_btnLeft_clicked(){
    selectButton(ui->btnLeft);
    robot.left(ui->spinMotor2->value());
}

void MainWindow::on_btnRight_clicked(){
    selectButton(ui->btnRight);
    robot.right(ui->spinMotor2->value());
}

void MainWindow::on_btnForwardLeft_clicked(){
    selectButton(ui->btnForwardLeft);
    robot.forwardLeft(ui->spinMotor1->value(), ui->spinMotor2->value());
}

void MainWindow::on_btnForwardRight_clicked(){
    selectButton(ui->btnForwardRight);
    robot.forwardRight(ui->spinMotor1->value(), ui->spinMotor2->value());
}

void MainWindow::on_btnBackwardLeft_clicked(){
    selectButton(ui->btnBackwardLeft);
    robot.backwardLeft(ui->spinMotor1->value(), ui->spinMotor2->value());
}

void MainWindow::on_btnBackwardRight_clicked(){
    selectButton(ui->btnBackwardRight);
    robot.backwardRight(ui->spinMotor1->value(), ui->spinMotor2->value());
}

void MainWindow::on_btnStop_clicked(){selectButton(ui->btnStop); robot.stop();}

void MainWindow::on_btnSetZero_clicked(){robot.setZeroReference();}

void MainWindow::on_btnGoToRest_clicked(){robot.goToRestPosition();}

void MainWindow::selectButton(QPushButton *button)
{
    QList<QPushButton*> buttons =
        {
            ui->btnForwardLeft,
            ui->btnForward,
            ui->btnForwardRight,

            ui->btnLeft,
            ui->btnStop,
            ui->btnRight,

            ui->btnBackwardLeft,
            ui->btnBackward,
            ui->btnBackwardRight
        };

    for(QPushButton *b : buttons){b->setStyleSheet("");}
    button->setStyleSheet("QPushButton {""background-color: #2196F3;""color: white;""font-weight: bold;""}");
}