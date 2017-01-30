 //interrupt for encoder to tell how much motor has turned
//Define Pins
 //Motor A
int enableA = 10;
int pinA1 = 5;
int pinA2 = 6;
int encoderPinA1 = 2; //interrupt pin
int encoderPinA2 = 3; //interrupt pin


//global variables
volatile float count = 0; //track how far the user has moved. gave it a random value for now to avoid neg numbers while testing
volatile float angularDist = 0;
volatile int pwm = 50;


//interrupt service routines

int isr_encoderA1(){
        if (digitalRead(encoderPinA1) == HIGH) { //if signal A1 is on rising edge
            if (digitalRead(encoderPinA2) == LOW) { //and signal A2 is low when on rising edge of A1
              count++; 
            } else { //signal A2 is high on rising edge of A1
              count--; 
            }
        } else {
          if (digitalRead(encoderPinA2) == HIGH) { 
              count++; 
            } else { 
              count--; 
            }
        }
}

void isr_encoderA2(){
  // rising edge A2
  if (digitalRead(encoderPinA2) == HIGH) {   
   // check channel A to see which way encoder is turning
    if (digitalRead(encoderPinA1) == HIGH) {  
      count++;         // CW
    } 
    else {
      count--;         // CCW
    }
  }
  // falling edge A2
  else { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(encoderPinA1) == LOW) {   
      count++;          // CW
    } 
    else {
      count--;          // CCW
    }
  }
}

void setup() {
Serial.begin (9600);
 //configure pin modes
 pinMode (enableA, OUTPUT);
 pinMode (pinA1, OUTPUT);
 pinMode (pinA2, OUTPUT);
 pinMode (encoderPinA1, INPUT);
 pinMode (encoderPinA2, INPUT);
 attachInterrupt(digitalPinToInterrupt(encoderPinA1), isr_encoderA1, CHANGE);
 attachInterrupt(digitalPinToInterrupt(encoderPinA2), isr_encoderA2, CHANGE);
 pinMode(LED_BUILTIN, OUTPUT);
 turnMotorCCW();
}
void loop() { 
    if( angularDist < 50000){
        angularDist = (count/(4*120))*360;
       //encoder is 120 cycles/ revolution. Therefore count/4 = # cycles (if you're checking both encoder pins), #cycles/120=#revolutions, #revolutions * 360 = total angular count
        Serial.print(count,DEC);
        Serial.print("\t");
        Serial.print("");   
        Serial.print(angularDist, DEC);
        Serial.print("\t");
        Serial.print("");   
        Serial.print(millis());
        Serial.println("\t");
    }
    if( angularDist < 150000 && angularDist > 50000){
        pwm = pwm * 1.1;
        turnMotorCCW();
        Serial.println("pwm should be faster");
        angularDist = (count/(4*120))*360;
        Serial.print(count,DEC);
        Serial.print("\t");
        Serial.print("");   
        Serial.print(angularDist, DEC);
        Serial.print("\t");
        Serial.print("");   
        Serial.print(millis());
        Serial.println("\t");
    }
    if( angularDist > 150000){
        stopMotor();
        Serial.println("got to threshold");
    }
 }

 void turnMotorCW(){
     analogWrite (enableA, pwm);
     digitalWrite (pinA1, HIGH); //not sure if A1 high + A2 low is CW or CCW
     digitalWrite (pinA2, LOW);
   return;
 }

void turnMotorCCW(){
     analogWrite (enableA, pwm);
     digitalWrite (pinA1, LOW); //not sure if A1 high + A2 low is CW or CCW
     digitalWrite (pinA2, HIGH);
   return;
}

 void stopMotor(){
    digitalWrite (enableA, LOW);
    return;
 }

