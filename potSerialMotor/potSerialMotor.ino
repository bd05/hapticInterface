int pinA1 = 5;
int pinA2 = 6;
// LED vars 
const int ledPin = 13;
 
// LED read vars
String inputString = "";         // a string to hold incoming data
boolean toggleComplete = false;  // whether the string is complete

// Potmeter vars
const int analogInPin = A0;
int sensorValue = 0;        // value read from the potmeter
int prevValue = 0;          // previous value from the potmeter 

//global variables
volatile int count = 0; //track how far the user has moved. gave it a random value for now to avoid neg numbers while testing
volatile float angularDist = 0;
volatile int pwm = 0;


void setup() {
Serial.begin (9600);
 //configure pin modes
 pinMode (pinA1, OUTPUT);
 pinMode (pinA2, OUTPUT);
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,0);
 //turnMotorCCW();
}

void loop() { 
   while (Serial.available() && toggleComplete == false) {
    char inChar = (char)Serial.read();
    if(inChar == 'E'){ // end character for toggle LED
     toggleComplete = true;
    }
    else{
      inputString += inChar; 
    }
  }
  // Toggle LED 13
  if(!Serial.available() && toggleComplete == true)
  {
    // convert String to int. 
    int recievedVal = stringToInt();
    digitalWrite(ledPin,recievedVal); 
    toggleComplete = false;
  }

  
    // Potmeter
     sensorValue = analogRead(analogInPin);   
    // read the analog in value:
    if(prevValue != sensorValue){
      Serial.print("B"); // begin character 
      Serial.print(sensorValue);  
      Serial.print("E"); // end character
      prevValue = sensorValue;
    }  
    if(sensorValue >=100){
      pwm = (count - 100);
      if(pwm>=50){
        pwm=50;
      }
      turnMotorCW();
    }
    else if(count <= -90)
    {
      pwm = abs(count) - 90;
      if(pwm >=50){
        pwm=50;
      }
       turnMotorCCW();
    }
    else {
      offMotor();
      pwm = 0;
    }

 }


 void turnMotorCW(){
     analogWrite (pinA1, 100); //not sure if A1 high + A2 low is CW or CCW
     analogWrite (pinA2, 10);
   return;
 }

void turnMotorCCW(){
     analogWrite (pinA1, 10); //not sure if A1 high + A2 low is CW or CCW
     analogWrite (pinA2, 100);
   return;
}

 void offMotor(){
     digitalWrite (pinA1, LOW); //not sure if A1 high + A2 low is CW or CCW
     digitalWrite (pinA2, LOW);
    return;
 }

 int stringToInt()
{
    char charHolder[inputString.length()+1];
    inputString.toCharArray(charHolder,inputString.length()+1);
    inputString = "";
    int _recievedVal = atoi(charHolder);
    return _recievedVal;
}
