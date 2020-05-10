#ifndef MOTOR_H_
#define MOTOR_H_

#include "states.h"

class motor {
public:
	motor(actions& action);

	stateId start(stateId next); //FIXME: why next?
	stateId stop(stateId next); //FIXME: why next?
	stateId setSpeed(int speed);
	stateId halt();

private:
  void initialAction() { /* what to do when start*/ }
  void finalAction  () { /* what to do at the end */ }

        state* current()       { return m_states[m_curr]; }
  const state* current() const { return m_states[m_curr]; } //FIXME: Why const?

  stateId changeState(stateId next);

private:
  const stateId          m_pre; //FIXME: why const?
        stateId          m_curr;
        actions          m_actions;
        idleState        m_idle;
        startState       m_start;
        stopState        m_stop;
        changeSpeedState m_changeSpeed;
        state*           m_states[UNDEF];
};

#endif /* TFTPSM_H_ */
