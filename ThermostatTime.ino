
#include <Time.h>

//pins
char TMP36 = A6;
char buttonPin = 3; //pinouts, button need 10k pulldown, int1
char MotorEnable = 5;
char MotorDir = 6;

//temperature sets, in F
byte stdT = 61; //Standby temp
byte warmT = 75; //for when I'm cold
byte morningT = 69; //Morning Temp
byte eveningT = 64; //Evening Temp

//current time hours 0-23, minutes 0-59, i.e. 11AM = 11 
int hours = 9;
int minutes = 00;

//times for change temps

byte morningHour = 5; //5AM
byte dayHour = 8; //8AM
byte eveningHour = 16; //4PM
byte nightHour = 21; //9PM

//some containers
float tempSum; //a holder for intermediate temperatures
float tempC; //the temp in c
float tempF;


//some state holders
byte buttonState = 0; //button starts unpressed
byte heat = 0; //equals 1 when heat is on
int mTime = 500; //time motor is on in ms
byte Tempstate; //a container for the temperature setpoint

void setup()
{
  // start serial port
  //Serial.begin(9600); //for debug
 
//*** set up pins

  pinMode(TMP36, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(MotorEnable, OUTPUT);
  pinMode(MotorDir, OUTPUT);
  
  setTime(hours,minutes,0,1,5,2014); //hr, min, sec, day, month, year when uploading
  
  attachInterrupt(1, buttonPress, RISING); //button change
 
  

}

void loop()
{
  
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
 
 //****** find the temp
 
  //Serial.print("Requesting temperatures...");
  
  for (int i = 0; i < 300; i++) { //take 240 measures and sum
  tempSum += analogRead(TMP36);  
  delay(1000); //this will take a total of 5 minutes
  }
  
  tempSum /= 300.0; //average reading
  float Volts = tempSum * (5.0/1024.0); //map to a temperature
  tempSum = 0.0; //reset
  tempC = (Volts-0.5) * 100.0;
  tempF = (tempC *(9.0/5.0)) + 32.0;
  
  tempF += 5.0; //an apparently needed calibration step
  
  /*
  Serial.println("Calc'ed temp");
  Serial.print(tempC);
  Serial.println(" C");
  Serial.print(tempF);
  Serial.println(" F");
  
  Serial.print("buttonState=");
  Serial.println(buttonState);
  Serial.print("heatstate=");
  Serial.println(heat);*/
  
  //***** What time is it? and what should we do about it?
  
  if ((hour() > nightHour) || (hour() < morningHour)) { //middle of night
    Tempstate = stdT; //set to coolest temp
  }
  else if (morningHour <= hour() < dayHour) {//it's morning
    Tempstate = morningT;
  }
  else if (dayHour <= hour() < eveningHour) {//it's the day
    Tempstate = stdT;
  }
  else if (eveningHour <= hour() <= nightHour) { //it's the evening
    Tempstate = eveningT;
  }
   //by the way, this only matters when the button hasn't been pushed
  
  /*Serial.print("time: ");
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  Serial.print(second());
  Serial.println(" ");
  Serial.print("Temperature set point : ");
  Serial.println(Tempstate);*/
  
  //***** decision proccess based on above
  
  if (buttonState == 0) { //button not pressed, set cool
    if (tempF < (Tempstate-1)) { //if it's cooler than set point 
      if (heat == 0) { //and heat is off
          heatON(); //turn on heat
      }}
      
    if (tempF >= Tempstate) { //warmer than set
      if (heat == 1) {  //AND the heat is actually on
      heatOFF(); //turn heat off
    }}
  }

  else { //buttonState == 1, set warm
    if (tempF < warmT) {
      if (heat == 0) {  //heat's not on
          heatON(); 
      }}
      
    if (tempF >= warmT) {
        buttonState = 0; //reset back to cool as soon as we get to temp  
        //digitalWrite(LEDpin, LOW); //high temp setting back to zero
      if (heat == 1) { //and heat is on
          heatOFF();
      }}
    }
  
    
}

//move the knob

void heatON() {
  
  digitalWrite(MotorEnable, HIGH);
  digitalWrite(MotorDir, LOW); //clockwise, ON
  delay(mTime);
  digitalWrite(MotorEnable, LOW);
  heat = 1; //set heat bit to ON
  
}

void heatOFF() {
  
  digitalWrite(MotorEnable, HIGH);
  digitalWrite(MotorDir, HIGH); //counterclockwise, OFF
  delay(mTime + 10); //added 10 ms to make sure it turns all the way back. No saftey
  digitalWrite(MotorEnable, LOW);
  digitalWrite(MotorDir, LOW);
  heat = 0; //heat is now set off
  
}

//button press

void buttonPress() {
  
  buttonState = 1; //change button state to 1
}
