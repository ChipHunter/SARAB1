#include "states.h"

#include <iostream>

/*************IDLE STATE**************/

stateId idleState::entryPoint() {

  std::cout << "entering idle state" << std::endl;

  return noTransition();

}

void    idleState::exitPoint()  {

  std::cout << "leaving idle state" << std::endl;

}

stateId idleState::setSpeed(int speed)   {

  m_action.startMotor();

  return requestTransition(START);

}

stateId idleState::halt()       {
  
  return noTransition();

}

/*************START STATE**************/

stateId startState::entryPoint() {

  std::cout << "entering start state" << std::endl;

  return noTransition();

}


void    startState::exitPoint()  {

  std::cout << "leaving start state" << std::endl;

}

stateId startState::setSpeed(int speed) {
  
  m_action.setSpeed(speed);

  return requestTransition(CHANGESPEED);

}

stateId startState::halt() {
  
  m_action.stopMotor();

  return requestTransition(STOP);

}

/*************STOP STATE***************/

stateId stopState::entryPoint() {

  std::cout << "entering stop state" << std::endl;
  
  return requestTransition(IDLE);

}

void    stopState::exitPoint()  {

  std::cout << "leaving stop state" << std::endl;

}

stateId stopState::setSpeed(int speed)   {
  
  return noTransition();

}

stateId stopState::halt()       {

  return noTransition();

}

/*************CHANGE SPEED STATE********/

stateId changeSpeedState::entryPoint() {

  std::cout << "entering change speed state" << std::endl;

  return noTransition();

}

void    changeSpeedState::exitPoint()  {

  std::cout << "leaving change speed state" << std::endl;

}

stateId changeSpeedState::setSpeed(int speed)   {
  
  m_action.setSpeed(speed);

  return noTransition();

}

stateId changeSpeedState::halt() {

  m_action.stopMotor();

  return requestTransition(STOP);

}
