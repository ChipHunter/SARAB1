#include "motor.h"
#include "actions.h"

motor::motor(actions& actions) : m_pre{IDLE},
                      m_curr{IDLE},
                      m_actions(actions),
                      m_idle{actions},
                      m_start{actions},
                      m_stop{actions},
                      m_changeSpeed{actions}
{

  m_states[IDLE]        = &m_idle;
  m_states[START]       = &m_start;
  m_states[STOP]        = &m_stop;
  m_states[CHANGESPEED] = &m_changeSpeed;

}

stateId motor::changeState(stateId next)
{

	if (m_curr != next) {
    current()->exitPoint();
    m_curr = next;
    changeState(current()->entryPoint());
  }

  return m_curr;

}

stateId motor::start(stateId next) {
  

}

stateId motor::stop(stateId next) {


}

stateId motor::setSpeed(int speed)     { return changeState(current()->setSpeed(speed)); }
stateId motor::halt()                  { return changeState(current()->halt());          }

