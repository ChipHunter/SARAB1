#include "actions.h"
#include <iostream>

actions::actions() : m_currSpeed{0}, 
                     m_initSpeed{10}, 
                     m_maxSpeed{100} {
}

void actions::startMotor() {

  m_currSpeed = m_initSpeed;
  std::cout << "motor started" << std::endl;
  std::cout << "current speed is: " << m_currSpeed << std::endl;

}

void actions::stopMotor() {

  m_currSpeed = 0;
  std::cout << "motor stopped" << std::endl;
  std::cout << "current speed is: " << m_currSpeed << std::endl;

}

void actions::setSpeed(int speed) {

  m_currSpeed = speed;
  std::cout << "new speed is set" << std::endl;
  std::cout << "current speed is: " << m_currSpeed << std::endl;

}
