#ifndef CANMANAGER_H
#define CANMANAGER_H

#include <QObject>
#include <QSerialPort>

class CanManager : public QObject
{
    Q_OBJECT
signals:
    void messageMonitor(const QString &text);
    void distanceUpdated(double distM1, double distM2);
    void speedUpdated(double motor1Speed, double motor2Speed);
    void angleUpdated(float angle);

public:

    explicit CanManager(QObject *parent = nullptr);

    bool connectPort(const QString &portName);
    void disconnectPort();
    bool isConnected() const;
    void sendRobotCommand(uint8_t dirMotor1, uint8_t pwmMotor1, uint8_t dirMotor2, uint8_t pwmMotor2);
    void sendFrame(uint16_t id, QByteArray data);
    void onSerialDataReceived();
    void setZeroReference();
    void goToRestPosition();

private:

    QSerialPort serial;
};

#endif