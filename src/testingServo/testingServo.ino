//Includes the Arduino Stepper Library
#include <Stepper.h>

// Defines the number of steps per rotation
const int stepsPerRevolution = 400;

// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

void stepper(int stepperspeed, int steps){
  myStepper.setSpeed(stepperspeed);
  myStepper.step(steps);
}

void setup() {
    // Nothing to do (Stepper Library sets pins as outputs)
}

void loop() {
  stepper(5, stepsPerRevolution); //Turns the motor 2038 steps at a speed of 5 rpm. 

}
