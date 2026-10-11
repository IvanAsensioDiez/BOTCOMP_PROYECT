#ifndef ROBOTCONTROLLER_H
#define ROBOTCONTROLLER_H

#include <QObject>

#include "canmanager.h"

class RobotController : public QObject
{
    Q_OBJECT

public:

    explicit RobotController(CanManager *canManager, QObject *parent = nullptr);

    void forward(int speed1);
    void backward(int speed1);

    void left(int speed2);
    void right(int speed2);

    void forwardLeft(int speed1,int speed2);
    void forwardRight(int speed1,int speed2);

    void backwardLeft(int speed1,int speed2);
    void backwardRight(int speed1,int speed2);

    void stop();

    void setZeroReference();
    void goToRestPosition();

private:
    CanManager *can;
};

#endif
