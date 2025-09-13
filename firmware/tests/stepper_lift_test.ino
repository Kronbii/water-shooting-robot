// Simple stepper lift test extracted from legacy sketches
#define DIR_PIN 3
#define STEP_PIN 4
#define STEPS_PER_REV 200

void liftRevolutions(double rev){
  long total = (long)(rev * STEPS_PER_REV);
  for(long i=0;i<total;i++){
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(2000);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(2000);
  }
}

void setup(){
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
}

void loop(){
  digitalWrite(DIR_PIN, LOW);
  liftRevolutions(5);
  delay(1000);
  digitalWrite(DIR_PIN, HIGH);
  liftRevolutions(5);
  delay(1000);
}
