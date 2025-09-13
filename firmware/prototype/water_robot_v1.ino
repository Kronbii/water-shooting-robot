// Prototype control firmware (v1)
// Consolidated from `src/TestCode/Code_v2.0/Code_v2.0.ino`
#include <Servo.h>

// ---- Constants ----
#define G_ACCEL 9.81
#define PI_CONST 3.14159265
#define WATER_DENSITY 998
#define Y_BOTTLE 0.17
#define Z0 0.6

// ---- Globals ----
static double xTargets[4];
static double thetaTargets[4];
static double waterHeight; // z
static double openTime;    // tnoz
static double nozzleAngle;
static double baseHeight;
static int targetIndex;

// Geometry / nozzle
static const double nozzleArea = 7.0685e-6; // m^2
static const double baseArea   = 1.96349e-3; // m^2

// Actuator pins
const int SERVO_PIN = 10;
const int RELAY_PIN = 11;
const int STEP_PIN  = 4;
const int DIR_PIN   = 3;

Servo nozzle;

// ---- Helper Input Functions ----
void setDistancesCm(double d1, double d2, double d3, double d4) {
  xTargets[0] = d1 / 100.0;
  xTargets[1] = d2 / 100.0;
  xTargets[2] = d3 / 100.0;
  xTargets[3] = d4 / 100.0;
}

void setAnglesDeg(double a1, double a2, double a3, double a4) {
  thetaTargets[0] = a1;
  thetaTargets[1] = a2;
  thetaTargets[2] = a3;
  thetaTargets[3] = a4;
}

// ---- Actuation ----
void shoot(double durationSeconds) {
  digitalWrite(RELAY_PIN, HIGH);
  delay((unsigned long)(durationSeconds * 1000.0));
  digitalWrite(RELAY_PIN, LOW);
}

void rotateNozzle(double angleDeg) {
  nozzle.write((int)angleDeg);
}

void liftBaseSteps(long steps, bool dir) {
  digitalWrite(DIR_PIN, dir ? HIGH : LOW);
  for (long i = 0; i < steps; ++i) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(2000);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(2000);
  }
}

// Convert required vertical displacement (m) to step count (needs calibration)
long displacementToSteps(double deltaMeters) {
  // TODO: replace with measured lead screw pitch or mechanism conversion
  // Placeholder: assume 1 revolution (200 steps) moves 1 mm.
  // 1 mm = 0.001 m => steps = deltaMeters / 0.001 * 200
  return (long)(deltaMeters * 200000.0); // 200 / 0.001 = 200000
}

void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  nozzle.attach(SERVO_PIN);

  openTime = 1.0;
  waterHeight = Z0;
  nozzleAngle = 0.0;
  baseHeight = 0.0;
  targetIndex = 0;

  setDistancesCm(93, 93, 58, 23);
  setAnglesDeg(30, 60, 90, 120);

  Serial.begin(9600);
  Serial.println(F("Water Shooting Robot - Prototype v1"));
}

void loop() {
  if (targetIndex >= 4) {
    // Finished sequence, hold.
    delay(1000);
    return;
  }

  double x = xTargets[targetIndex];
  // Simple ballistic height relation (domain-specific)
  double requiredHeight = Y_BOTTLE + 0.25 * x * x * (1.0 / waterHeight);
  double delta = requiredHeight - baseHeight;
  bool dirUp = delta >= 0.0;

  long steps = displacementToSteps(fabs(delta));
  if (steps > 0) {
    liftBaseSteps(steps, dirUp);
  }
  baseHeight = requiredHeight;

  // Aim
  rotateNozzle(thetaTargets[targetIndex]);
  nozzleAngle = thetaTargets[targetIndex];
  delay(500);

  // Fire
  shoot(openTime);

  // Update water level (Torricelli inspired)
  double term = sqrt(waterHeight) - sqrt(G_ACCEL / 2.0) * (nozzleArea / baseArea) * openTime;
  waterHeight = term * term;

  // Debug telemetry
  Serial.print(F("Target "));
  Serial.print(targetIndex);
  Serial.print(F(" x(m):")); Serial.print(x, 3);
  Serial.print(F(" requiredH:")); Serial.print(requiredHeight, 3);
  Serial.print(F(" waterH:")); Serial.print(waterHeight, 3);
  Serial.print(F(" steps:")); Serial.print(steps);
  Serial.println();

  targetIndex++;
  delay(800);
}
