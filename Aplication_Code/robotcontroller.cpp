#include "robotcontroller.h"
#include <QDebug>

RobotController::RobotController(CanManager *canManager, QObject *parent) : QObject(parent), can(canManager){}

void RobotController::forward(int speed1){can->sendRobotCommand(1, speed1, 0, 0);}
void RobotController::backward(int speed1){can->sendRobotCommand(2, speed1, 0, 0);}

void RobotController::right(int speed2){can->sendRobotCommand(0, 0, 1, speed2);}
void RobotController::left(int speed2){can->sendRobotCommand( 0, 0, 2, speed2);}

void RobotController::forwardLeft(int speed1, int speed2){can->sendRobotCommand(1, speed1, 2, speed2);}
void RobotController::forwardRight(int speed1, int speed2){can->sendRobotCommand(1, speed1, 1, speed2);}

void RobotController::backwardLeft(int speed1, int speed2){can->sendRobotCommand(2, speed1, 2, speed2);}
void RobotController::backwardRight(int speed1, int speed2){can->sendRobotCommand(2, speed1, 1, speed2);}

void RobotController::stop(){can->sendRobotCommand(0, 0, 0, 0);}

void RobotController::setZeroReference(){can->setZeroReference();}
void RobotController::goToRestPosition(){can->goToRestPosition();}