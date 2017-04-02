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

//2nd potentiometer
const int analogInPin1 = A1;
int sensorValue1 = 0;        // value read from the potmeter
int prevValue1 = 0;          // previous value from the potmeter
 
void setup() {
  // initialize serial:
  Serial.begin(9600);
  // init LED
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,0);

  //initialize potentiometer for web client so D3 doesn't confused by NaN values
  sensorValue = analogRead(analogInPin); // Potmeter0
  sensorValue1 = analogRead(analogInPin1); //Potmeter1
 /* Serial.print("B"); // begin character 
  Serial.print(sensorValue);  
  Serial.print("E"); // end character
  Serial.print("C"); // begin character 
  Serial.print(sensorValue1);  
  Serial.print("F"); // end character     */

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

//===============================POTENTIOMETER READINGS SEND TO NODE.JS SERVER====================================
     sensorValue = analogRead(analogInPin); // Potmeter0
     sensorValue1 = analogRead(analogInPin1); //Potmeter1
    // read the analog in value:
    if(abs(sensorValue - prevValue) >= 3){
      /*Serial.print("B"); // begin character 
      Serial.print(sensorValue);  
      Serial.print("E"); // end character*/
      char beginR = 'B';
      char endR = 'E';
      char str[6];
      if( sensorValue < 1000){
        char sensorR[3];
        itoa(sensorValue,sensorR,10);
        sprintf(str, "%c%c%c%c%c", beginR, sensorR[0],sensorR[1],sensorR[2],endR);
      }
      else{
        char sensorR[4];
        itoa(sensorValue,sensorR,10);
        sprintf(str, "%c%c%c%c%c%c", beginR, sensorR[0],sensorR[1],sensorR[2],sensorR[3],endR);
      }
      Serial.write(str);
    
      prevValue = sensorValue;
    }  

    if(abs(sensorValue1 - prevValue1) >= 3){
      /*Serial.print("C"); // begin character 
      Serial.print(sensorValue1);  
      Serial.print("F"); // end character*/
      char beginL = 'C';
      char endL = 'F';
      char str[6];
      if( sensorValue < 1000){
        char sensorL[3];
        itoa(sensorValue1,sensorL,10);
        sprintf(str, "%c%c%c%c%c", beginL, sensorL[0],sensorL[1],sensorL[2],endL);
      }
      else{
        char sensorL[4];
        itoa(sensorValue1,sensorL,10);
        sprintf(str, "%c%c%c%c%c%c", beginL, sensorL[0],sensorL[1],sensorL[2],sensorL[3],endL);
      }
      Serial.write(str);
      
      prevValue1 = sensorValue1;
    }  
    
  /*  int test = 4567;
    char beginR = 'B';
    char endR = 'E';
    char str[6];
    if( test < 1000){
      char sensorR[3];
      itoa(test,sensorR,10);
      sprintf(str, "%c%c%c%c%c", beginR, sensorR[0],sensorR[1],sensorR[2],endR);
    }
    else{
      char sensorR[3];
      itoa(test,sensorR,10);
      sprintf(str, "%c%c%c%c%c%c", beginR, sensorR[0],sensorR[1],sensorR[2],sensorR[3],endR);
    }
    Serial.write(str);*/
    
    
  delay(50); // give the Arduino some breathing room. 
}
 
int stringToInt()
{
    char charHolder[inputString.length()+1];
    inputString.toCharArray(charHolder,inputString.length()+1);
    inputString = "";
    int _recievedVal = atoi(charHolder);
    return _recievedVal;
}
