// LED vars 
const int ledPin = 13;
 
// LED read vars
String inputString = "";         // a string to hold incoming data
boolean toggleComplete = false;  // whether the string is complete
boolean pwmComplete = false;  
 
// Potmeter vars
const int analogInPin = A0;
int sensorValue = 0;        // value read from the potmeter
int prevValue = 0;          // previous value from the potmeter
 
void setup() {
  // initialize serial:
  Serial.begin(9600);
  // init LED
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,0);
}
 
void loop() {
   // Recieve data from Node and write it to a String
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
/*
    // Potmeter
     sensorValue = analogRead(analogInPin);   
    // read the analog in value:
    if(prevValue != sensorValue){
      Serial.print("B"); // begin character 
      Serial.print(sensorValue);  
      Serial.print("E"); // end character
      prevValue = sensorValue;
    }  
  delay(50); // give the Arduino some breathing room. */
}
 
int stringToInt()
{
    char charHolder[inputString.length()+1];
    inputString.toCharArray(charHolder,inputString.length()+1);
    inputString = "";
    int _recievedVal = atoi(charHolder);
    return _recievedVal;
}
