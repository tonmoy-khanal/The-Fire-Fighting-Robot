#include <Servo.h>
#include <AFMotor.h>

float L = 0.00,R = 0.00,C = 0.00;
Servo myTower;

AF_DCMotor lmoto(1,MOTOR12_64KHZ);
AF_DCMotor rmoto(2,MOTOR12_64KHZ);
//consider the three sensors kept in the following orient
//4: 30deg left
//5: center
//6: 30deg right
int fl = A4;
int fc = A5;
int fr = A6;

int pump = 7;

int randint;

int mod(int x){
  if(x<0)return -x;
  else return x;
}

int detect(){
  float l = analogRead(fl);
  float c = analogRead(fc);
  float r = analogRead(fr);
  int side = (l>r)?(-l):r; //I assume servo assumes left side is negative angle
  //change if right is the negative angle
  int angle = 30*side/(mod(side)+c);
  //weighted average
  if((side > 50) || (c > 50))return angle;
  else return 1000;
}

void servoTurn(int deg){
  myTower.write(deg);
  delay(2000);
}

void pumpInit(){
  digitalWrite(pump,HIGH);
  while((analogRead(fl)>10)||(analogRead(fc)>10)||(analogRead(fr)>10)) delay(10);
  digitalWrite(pump,LOW);;
}

void fwd(){
  lmoto.run(FORWARD);
  rmoto.run(FORWARD);
}

void bck(){
  lmoto.run(BACKWARD);
  rmoto.run(BACKWARD);
}

void lt(){
  lmoto.run(FORWARD);
  rmoto.run(BACKWARD);
  delay(800);
  rmoto.run(FORWARD);
  lmoto.run(FORWARD);
}

void rt(){
  lmoto.run(FORWARD);
  rmoto.run(RELEASE);
  delay(800);
  rmoto.run(FORWARD);
  lmoto.run(FORWARD);
}

void setup() {
  myTower.attach(10);
  pinMode(fl,INPUT);
  pinMode(fc,INPUT);
  pinMode(fr,INPUT);
  pinMode(pump,INPUT);
  Serial.begin(9600);
  lmoto.setSpeed(255);
  rmoto.setSpeed(255);
  lmoto.run(RELEASE);
  rmoto.run(RELEASE);
  delay(5000);
  randomSeed(A0);
}


void loop(){
  while(!analogRead(fc)){
    randint = int(random(2));
    fwd();
    delay (5000);
    switch(randint){
      case 0:
        break;
      case 1:
        randint = int(random(2));
        switch(randint){
          case 0:
            lt();
            break;
          case 1:
            rt();
            break;
        }
    }
  }
  servoTurn(detect());
  pumpInit();
  bck();
  delay(5000);
}