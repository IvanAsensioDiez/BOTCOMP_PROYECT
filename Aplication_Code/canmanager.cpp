#include "canmanager.h"

#include <QDebug>

CanManager::CanManager(QObject *parent) : QObject(parent){
    connect(&serial, &QSerialPort::readyRead, this, &CanManager::onSerialDataReceived);
}

bool CanManager::connectPort(const QString &portName){
    serial.setPortName(portName);
    serial.setBaudRate(2000000);
    serial.setDataBits(QSerialPort::Data8); // 8 bits
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop); // 1 stop bit
    serial.setFlowControl(QSerialPort::NoFlowControl);
    bool opened = serial.open(QIODevice::ReadWrite);

    if(opened){emit messageMonitor("Connected to" + portName);}
    else      {emit messageMonitor("ERROR opening " + portName);}

    return opened;
}

void CanManager::sendFrame(uint16_t id, QByteArray data){
    if(!serial.isOpen()){return;}

    QByteArray frame;

    frame.append(char(0xAA)); // BEGINNING
    frame.append(char(0xC8));

    // ID 0x0100
    frame.append(char(id & 0xFF));
    frame.append(char((id >> 8) & 0xFF));

    while(data.size() < 8) { data.append(char(0x00)); }

    frame.append(data.left(8));
    frame.append(char(0x55)); // END

    QString text = "TX: " + frame.toHex(' '); emit messageMonitor(text); // Text string containing the frame in hexadecimal.

    serial.write(frame); // Send the frame via the serial port and return the number of bytes written.
    serial.flush(); // Forces the immediate transmission of pending data in the serial port buffer.
}

void CanManager::onSerialDataReceived(){
    QByteArray data = serial.readAll();

    QString text = "RX: " + data.toHex(' '); emit messageMonitor(text);

    int distancePos = data.indexOf(QByteArray::fromHex("010002000008")); // Distance message (ID 0x200)

    if(distancePos >= 0){
        int32_t distM1;
        int32_t distM2;

        memcpy(&distM1, data.constData() + distancePos + 6, 4); // Copies 4 bytes from the CAN frame.
        memcpy(&distM2, data.constData() + distancePos + 10, 4);

        emit distanceUpdated(distM1 / 1000.0, distM2 / 1000.0);
    }

    int anglePosition = data.indexOf(QByteArray::fromHex("010102000002")); // Angle (ID 0x201)

    if(anglePosition >= 0){
        int16_t anguloRaw;

        memcpy(&anguloRaw, data.constData() + anglePosition + 6, 2);
        float angle = -anguloRaw / 10.0f; emit angleUpdated(angle);
    }

    int speedPos = data.indexOf(QByteArray::fromHex("010202000008")); // Speed message (ID 0x202)

    if(speedPos >= 0){
        int32_t speedMotor1;
        int32_t speedMotor2;
        memcpy(&speedMotor1, data.constData() + speedPos + 6, 4);
        memcpy(&speedMotor2, data.constData() + speedPos + 10, 4);

        emit speedUpdated(speedMotor1 / 1000.0, speedMotor2 / 1000.0);
    }
}

void CanManager::sendRobotCommand(uint8_t dirMotor1, uint8_t pwmMotor1, uint8_t dirMotor2, uint8_t pwmMotor2){
    QByteArray data;

    data.append(char(dirMotor1));
    data.append(char(pwmMotor1));

    data.append(char(dirMotor2));
    data.append(char(pwmMotor2));

    sendFrame(0x0100, data);
}

void CanManager::setZeroReference(){ QByteArray data; data.append(char(0)); sendFrame(0x0101, data); }

void CanManager::goToRestPosition(){ QByteArray data; data.append(char(1)); sendFrame(0x0101, data); }
