//#include <Scheduler.h>

#include <Servo.h>

#include <TM1637Display.h> // this controls the seven-segment display and is not a standard library; 
                           // I found it here: https://github.com/avishorp/TM1637
#define CLK1 5   //choose these
#define DIO1 6   // these are for the seven segment display
#define CLK2 9
#define DIO2 10

TM1637Display TM1( CLK1, DIO1); 
TM1637Display TM2( CLK2, DIO2); // these are the seven segment display objects
Servo theservo;
const int photor1 = A2;
const int photor2 = A5;
const int laserpin1 = 1;
const int laserpin2 = 2;

bool p1swon = 0;  bool p2swon = 0;
bool firstwin1 = 0; bool firstwin2 = 0;
unsigned long wintime1;  unsigned long wintime2;
unsigned long starttime;
const int cycletime = 30000;
const float ilog2 = 1/log(2); const float logcycle = log(cycletime); int t;

void set() {
  theservo.attach(7);
  p1swon = 0;
  p2swon = 0;
  firstwin1 = 0;
  firstwin2 = 0;
  TM1.setBrightness(0x0f);
  TM2.setBrightness(0x0f);
  Serial.begin(9600);
  pinMode(photor1, INPUT);
  pinMode(photor2, INPUT);
  pinMode(laserpin1, OUTPUT);
  pinMode(laserpin2, OUTPUT);
  randomSeed(analogRead(A3));
  digitalWrite(laserpin1, LOW);
  digitalWrite(laserpin2, LOW);
  TM1.showNumberDec(3); //countdown
  TM2.showNumberDec(3);
  delay(1000);
  theservo.write(0);
  TM1.showNumberDec(2);
  TM2.showNumberDec(2);
  delay(1000);
  TM1.showNumberDec(1);
  TM2.showNumberDec(1);
  delay(1000);
  starttime = millis();
  //Scheduler.startLoop(timer);
}

void setup() {
  set();
}

void showtime() {
  t = (millis() - starttime)/100; //showtime();
  if ((p1swon == 1) && (firstwin1 == 0)) {
    wintime1 = t;
    firstwin1 = 1;
  }
  if ((p2swon == 1) && (firstwin2 == 0)) {
    wintime2 = t;
    firstwin2 = 1;
  }
  if ((p1swon == 0) && (p2swon ==0)) { 
      TM1.showNumberDec(t);
      TM2.showNumberDec(t);
      //delay(100);
    }
  if((p1swon == 1) && (p2swon == 1)) { 
      victorydance(wintime1,wintime2);
      }
  if ((p1swon == 1) && (p2swon ==0)) {
      TM1.showNumberDec(wintime1);
      TM2.showNumberDec(t);
      //delay(100);
      }
  if ((p1swon == 0) && (p2swon == 1)) { 
      TM1.showNumberDec(t);
      TM2.showNumberDec(wintime2);
      //delay(100);
      }
      //yield();
}

void loop() {
  if (analogRead(photor1) > 800 || p1swon == 1) { // check();
    p1swon = 1;
   // showtime();
  }
  if (analogRead(photor2) > 800 || p2swon == 1) {
    p2swon = 1;
  }
  int T = (millis()-starttime) % cycletime;  // motion();
  if (T < 100) {
    int arc = random(0,25);
    if (arc < 0) {
      arc = 360 + arc;
    }
    theservo.write(arc); //move every cycle
  }
  float v = ilog2*(log(cycletime-T)-logcycle); // log base 2 of (cycletime-t)/cycletime
  if (v - floor(v) < .05) { // warning blinker
    digitalWrite(laserpin1, LOW);
    digitalWrite(laserpin2, LOW);
  } else {
    digitalWrite(laserpin1, HIGH);
    digitalWrite(laserpin2, HIGH);
  }
  showtime();
  //Serial.println(analogRead(A2));
  //Serial.println(analogRead(A5));
}

void victorydance(unsigned long wintime1, unsigned long wintime2) {
     TM1.showNumberDec(wintime1);
     TM2.showNumberDec(wintime2);
     delay(1000);
     for (int i = 0; i < 10; i++) {
      if (i % 2 == 0) {
        TM1.setBrightness(0);
        TM2.setBrightness(0);
        TM1.showNumberDec(wintime1);
        TM2.showNumberDec(wintime2);
        //theservo.write(0);
        delay(100);
      } else {
        TM1.setBrightness(7);
        TM2.setBrightness(7);
        TM1.showNumberDec(wintime1);
        TM2.showNumberDec(wintime2);
        //theservo.write(180);
        delay(100);
      }
     }
     if (wintime1 >= wintime2) {
       TM2.setBrightness(7);
       TM1.setBrightness(0);
       TM1.showNumberDec(wintime1);
       TM2.showNumberDec(wintime2);
       digitalWrite(laserpin2, HIGH);
       digitalWrite(laserpin1, LOW);
     } else {
       TM1.setBrightness(7);
       TM2.setBrightness(0);
       TM1.showNumberDec(wintime1);
       TM2.showNumberDec(wintime2);
       digitalWrite(laserpin1, HIGH);
       digitalWrite(laserpin2, LOW);
     }
     delay(1000);
     set();
}





