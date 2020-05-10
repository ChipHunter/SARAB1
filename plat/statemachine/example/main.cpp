#include "motor.h"
#include "actions.h"

int main() {
  
  actions actions; 
  motor motor(actions);

  
  motor.setSpeed(50);
  motor.halt();
  
  motor.setSpeed(50);
  motor.setSpeed(20);
  motor.setSpeed(30);

  motor.halt();

return 0;
}
