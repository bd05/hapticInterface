int enableA = 10;

int pinA1 = 5;

int pinA2 = 6;

int encoderPinA1 = 2; //interrupt pin

int encoderPinA2 = 3; //interrupt pin





//global variables

volatile int count = 0; //track how far the user has moved. gave it a random value for now to avoid neg numbers while testing

volatile float angularDist = 0;

volatile int pwm = 0;





//interrupt service routines



int isr_encoderA1(){

  unsigned long interruptTime = millis();



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







void setup() {

Serial.begin (9600);

 //configure pin modes

 pinMode (enableA, OUTPUT);

 pinMode (pinA1, OUTPUT);

 pinMode (pinA2, OUTPUT);

 pinMode (encoderPinA1, INPUT);

 pinMode (encoderPinA2, INPUT);

 attachInterrupt(digitalPinToInterrupt(encoderPinA1), isr_encoderA1, CHANGE);

 pinMode(LED_BUILTIN, OUTPUT);

 //turnMotorCCW();

}

void loop() { 

     //Serial.println (count, DEC); //print millis() in adjacent column as well so you can copy and paste time in excel to graph

     //encoder is 120 cycles/ revolution. Therefore count/4 = # cycles (if you're checking both encoder pins), #cycles/120=#revolutions, #revolutions * 360 = total angular count

      Serial.print(count,DEC);
      Serial.print("   "); 
      Serial.print(pwm,DEC);

      Serial.println("\t");

      Serial.print("");   

    if(count >=100){
      pwm = (count - 100);
      if(pwm>=50){
        pwm=50;
      }
      turnMotorCCW();
    }
    else if(count <= -90)
    {
      pwm = abs(count) - 90;
      if(pwm >=50){
        pwm=50;
      }
       turnMotorCW();
    }
    else {
      offMotor();
      pwm = 0;
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



 void offMotor(){

    digitalWrite (enableA, LOW);

    return;

 }
