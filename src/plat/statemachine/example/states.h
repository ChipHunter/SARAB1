#ifndef STATES_H_
#define STATES_H_

#include "actions.h"

enum stateId   { IDLE, START, CHANGESPEED, STOP, UNDEF };

class triggers
{
public:
	virtual ~triggers() { }

	virtual stateId setSpeed(int speed) = 0;
  virtual stateId halt()              = 0;
};

class state : public triggers {
public:
  state(stateId sid, actions& action) : m_sid(sid), m_action(action)  {}

  virtual stateId entryPoint()             { return noTransition(); }
  virtual void    exitPoint ()             {}

protected:
  stateId requestTransition(stateId sid) const       { return sid;  }
  stateId noTransition     ()            const       { return m_sid; }

  const stateId  m_sid;
        actions& m_action;

};

class idleState: public state {
public:
	idleState(actions& action) : state(IDLE, action) { }

	stateId entryPoint();
	void    exitPoint();
	stateId setSpeed(int speed);
	stateId halt();
};

class startState : public state {
public:
	startState(actions& action) : state(START, action) { }

	stateId entryPoint();
	void    exitPoint();
	stateId setSpeed(int speed);
	stateId halt();
};

class stopState : public state {
public:
	stopState(actions& action) : state(STOP, action) { }

	stateId entryPoint();
	void    exitPoint();
	stateId setSpeed(int speed);
	stateId halt();
};

class changeSpeedState : public state {
public:
  changeSpeedState(actions& action) : state(CHANGESPEED, action) { } 

  stateId entryPoint(); 
  void    exitPoint(); 
  stateId setSpeed(int speed);
	stateId halt();
};

#endif /* STATES_H_ */
