// Define stepper motor connections and steps per revolution:
#define dirPin 3
#define stepPin 4
#define stepsPerRevolution 200


void liftBase(double rev){
  for (int i = 0; i < rev*stepsPerRevolution; i++) {
    // These four lines result in 1 step:
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }
}
void setup() {
  // Declare pins as output:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}

void loop() {
 digitalWrite(dirPin,LOW);
 liftBase(5);
 delay(1000);
 digitalWrite(dirPin,HIGH);
 liftBase(5);
 delay(1000);
}
