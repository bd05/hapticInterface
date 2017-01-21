 //interrupt for encoder to tell how much motor has turned
//Define Pins
 //Motor A
int enableA = 10;
int pinA1 = 5;
int pinA2 = 6;
int encoderPinA1 = 3; //interrupt pin
int encoderPinA2 = 4;

//global variables
volatile int distance = 0; //track how far the user has moved. gave it a random value for now to avoid neg numbers while testing
// Keep track of last rotary value
int lastCount = 0;


//interrupt service routines

int isr_encoder(){
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  
  // If interrupts come faster than 5ms, assume it's a bounce and ignore
  if (interruptTime - lastInterruptTime > 5) {
      if (digitalRead(encoderPinA1) == digitalRead(encoderPinA2)) { //if pulses are both high,motor is moving CW
        distance++;
      } else { //if one pulse is high and one pulse is low, motor is moving CCW
        distance--;
      }
  }

  //return encoder0Pos;
  Serial.println (distance, DEC);

}

void setup() {
Serial.begin (9600);
 //configure pin modes
 pinMode (enableA, OUTPUT);
 pinMode (pinA1, OUTPUT);
 pinMode (pinA2, OUTPUT);
 pinMode (encoderPinA1, INPUT);
 pinMode (encoderPinA2, INPUT);
 attachInterrupt(digitalPinToInterrupt(encoderPinA1), isr_encoder, CHANGE);
}
void loop() { 
    // If the current rotary switch position has changed then update everything
    if (distance != lastCount) {
      // Keep track of this new value
     // Serial.println (distance, DEC);
      lastCount = distance ;

      if(distance > 20 ){
        turnMotor();
      }
    }
 }

 void turnMotor(){
   //enabling motor A
   Serial.println ("Enabling Motor A");
   digitalWrite (enableA, HIGH);
  //do something
   //forward
   Serial.println ("Forward");
   digitalWrite (pinA1, HIGH);
   digitalWrite (pinA2, LOW);
   delay (1000);
   //stop
   Serial.println ("Stopping Motor A");
   digitalWrite (enableA, LOW);
   delay (1000);
   return;
 }

