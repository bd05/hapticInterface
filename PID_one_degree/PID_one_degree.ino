

int enableA = 10;
int pinA1 = 5;
int pinA2 = 6;
int encoderPinA1 = 2; //interrupt pin
int encoderPinA2 = 3; //interrupt pin

//global variables
volatile int count = 0; //track how far the user has moved. gave it a random value for now to avoid neg numbers while testing
volatile int pwm = 0;
volatile int initial_flag = 1;

//PID variables
volatile long my_SetPoint = 0;
long num;
int startFlag = 0;

int isr_encoderA1(){

  unsigned long interruptTime = millis();

  initial_flag = 0; //once hit ISR, turn off initial flag

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
        if (count >= my_SetPoint)
        {
          pwm = 0; //turnoff motor immediately after reaching SetPoint
        }
        else
        {
          pwm = (pwm <= 25) ? 25 : (((int)my_SetPoint - count));
          //25 is to make sure pwm is always bigger than 25, because anything lower motor won't move, count won't increment, pwm won't increase, everything stuck
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

Serial.println("enter threshold: ");
/* while(Serial.available() <=0){
     entered_SetPoint = Serial.read();
 }

Serial.println(entered_SetPoint);*/


}

void loop() { 
  if( startFlag == 0){
    while(Serial.available()>0){ 
      num= Serial.parseInt();   
      Serial.print(num);  
      startFlag = 1;
    }
  }

  if( startFlag != 0){
      turnMotorCCW();
      if (my_SetPoint <= num ) // 100 is the utimate SetPoint for now, make is a variable later on
      {
        my_SetPoint = (my_SetPoint + ((num - my_SetPoint)/ 20));
        //the divide by 20 thing is a PD tuning, the bigger the gap, the more SetPoint is incremented, the higher pwm
      }

      if (initial_flag == 1)
      {
        pwm = 20; //in the beginning before ISR is reached for first time, want to make sure motor moves
      }

        
      Serial.print(count,DEC);
      Serial.print("   "); 
     Serial.print(pwm,DEC);
      Serial.print("   "); 
      Serial.print(my_SetPoint, DEC);
      Serial.println("\t");
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

