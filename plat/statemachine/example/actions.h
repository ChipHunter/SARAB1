#ifndef ACTIONS_H_
#define ACTIONS_H_

class actions {
public:
	actions();

  void startMotor();
  void setSpeed(int speed);
  void stopMotor();

private:
        int m_currSpeed;
  const int m_initSpeed;
  const int m_maxSpeed;

};

#endif /* ACTIONS_H_ */
