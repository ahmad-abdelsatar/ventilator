#include <TimerOne.h>

struct valve_struct {
  bool air;
  bool  ox;
  bool out;
  
  };

bool outing;
int p_act;
int p1;  
int p3;
int ratio;

valve_struct valves;
volatile unsigned long blinkCount = 0; // use volatile for shared variables
unsigned long blinkCopy;  // holds a copy of the blinkCount

int air_v_pin = 4;
int ox_v_pin = 5;
int out_v_pin = 6;
int comp_pin = 7;

int p_act_pin = 0;

int ratio_pin = 1;

void setup() {
  // put your setup code here, to run once:
  pinMode(air_v_pin , OUTPUT);
  pinMode(ox_v_pin , OUTPUT);
  pinMode(out_v_pin , OUTPUT);
  pinMode(comp_pin , OUTPUT);
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(outSwitch); 
  Serial.begin(9600);
}

void loop() {

  
  // put your main code here, to run repeatedly:
  p_act = map(analogRead(p_act_pin), 0, 1023, 0, 300);
 // Serial.println("Actual Pressure = ");
//  Serial.println(p_act);
  ratio = map(analogRead(ratio_pin), 0, 1023, 0, 100);
//  Serial.println("Ratio = ");
 // Serial.println(String(ratio));
  p1 = 70;
  p3 = 200;
  valves = Mix(p_act, p1, p3, ratio, outing);
  outing = valves.out;
/*  Serial.println("Outing");
  Serial.println(outing);
  delay(100);
  Serial.println("Air Valve");
  Serial.println(valves.air);
  delay(100);
  Serial.println("Ox Valve");
  Serial.println(valves.ox);
  delay(100);
  Serial.println("Out Valve");
  Serial.println(valves.out);
  delay(100);*/
  digitalWrite(air_v_pin , valves.air);
  digitalWrite(ox_v_pin , valves.ox);
  digitalWrite(out_v_pin , valves.out);
  Serial.println(blinkCopy);
  digitalWrite(comp_pin , valves.air || valves.ox);  
}

struct valve_struct Mix(int p_act, int p1, int p3, int ratio , bool outing){

  valve_struct valves;
  
  
  int p2 = p1 + ((p3 - p1)*(100-ratio))/100 ;

  if ((p_act < p1) || ((p_act < p2) && !outing))
{
    outing = false;
    valves.air = true;
    valves.ox = false;
    valves.out = false;
}
  else if ((p_act >= p2) && (p_act < p3) && (!outing))
{
    valves.air = false;
    valves.ox = true;
    valves.out = false;
}
  else if (p_act >= p3)
{
    outing = true;
    valves.air = false;
    valves.ox = false;
    valves.out = true;
}
  else
{
  //  valves.air = false;
  //  valves.ox = false;
  //  valves.out = false;
}
  return valves;
}
void outSwitch() {
   blinkCount = blinkCount + 1;  // increase when LED turns on
 // unsigned long blinkCopy;  // holds a copy of the blinkCount
  noInterrupts();
  blinkCopy = blinkCount;
  interrupts();}
  if (blinkCopy<4)
  {
      digitalWrite(6 , HIGH);
  }
  
