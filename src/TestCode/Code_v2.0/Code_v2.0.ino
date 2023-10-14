kbnbkj.jkbnjl.hhk.bukbkljhjkbuihkjho;uhughubn.ljnljho;uhhhmbghikygjkhgyikgyukghjklgkhgkjygkhbiughhjkgtjhgiygioygyigilu //////////LIBRARIES//////////
#include <Servo.h>
/////////////////////////////
//////////VARIABLES//////////
#define g 9.81
#define pi 3.14159265
#define row 998
#define yb 0.17
#define z0 0.6

double y0; // height of nozzle
double x[4]; //Distance as a function of time
double z; //height of water at any instant of time
double tnoz; //time opened of nozzle
double twater; //time taken to hit targethkghjvgftykfuhykkvutvkjgvhkvjhkjhuiguio;giylyg97y
double Aj = 7.0685*pow(10,-6); // Area of the nozzle
double Ab = 1.96349*pow(10,-3); //Area of the base
double theta[4];
double steps;

int i;
const int servoPin=10; //initialize
Servo nozzle;
const int relayH =11; //initialize
double nozAngle;

//stepper pins and variables
const int stepPin = 4; // initialize
const int dirPin = 3; // initialize
double baseHeight;
bool dir;
/////////////////////////////
//////////FUNCTIONS//////////
//function to input distance x in cm and convert to m
void inputX(double d1, double d2, double d3, double d4){
  x[0]=d1/100;
  x[1]=d2/100;
  x[2]=d3/100;
  x[3]=d4/100;
}

//function to input angle in degree
void inputAngle(double a1, double a2, double a3, double a4){
  theta[0]=a1;
  theta[1]=a2;
  theta[2]=a3;
  theta[3]=a4;
}

void shoot(double Stime){
  digitalWrite(relayH, HIGH);
  delay(Stime);
  digitalWrite(relayH, LOW);
}   

//Function to run the stepper --> check it
void rotateNoz(double angle){
nozzle.write(angle);
}

void liftBase(double steps, bool dir){
  digitalWrite(dirPin,dir);
  for (int i = 0; i < steps; i++) {
    // These four lines result in 1 step:
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }
}
///////////////////////////
///////////SETUP///////////
void setup() {
  pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);
  nozzle.attach(servoPin);
  //time for nozzle and height of water initial
  tnoz = 1;
  z=z0;
  nozAngle = 0;
  baseHeight = 0; // to be changed

  //input distance of soldiers
  inputX(93,93,58,23);
  inputAngle(30,60,90,120);

  //iterator
  i=0;

Serial.begin(9600);
}
////////////////////////
//////////LOOP//////////
void loop() {
  while (i!=4){
  y0 = yb + (1.000 / 4.000) * pow(x[i], 2) * (1.000 / z);
  
  //lift the base up to y0
  //tali3 y0 interms of stepper steps
  if(y0>baseHeight)
  dir = HIGH; // HIGH goes up
  else if(y0<baseHeight)
  dir = LOW; // LOW goes down

  steps = ((y0 - baseHeight)*10000)/(pi);

  //
//  Serial.print("Steps: ");
//  Serial.print(steps);
//  Serial.println();
//  Serial.print("y0: ");
//  Serial.println(y0);
  liftBase(steps, dir);
  delay(500);

  //rotate the nozzle
  rotateNoz(theta[i]);
  delay(500);
  //open the valve of tnoz seconds
  shoot(tnoz);
   
  //Update the value of the water height
  z = pow((sqrt(z)-(sqrt(g/2))*(Aj/Ab)*tnoz), 2);

  //increment 
  baseHeight = y0;
  nozAngle = theta[i];
  i++;
  delay(1000);
  }
}
