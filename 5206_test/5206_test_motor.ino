//Testing the DC Motors
//Define Pins
 //Motor A
 int enableA = 10;
 int pinA1 = 5;
 int pinA2 = 3;


void setup() {
Serial.begin (9600);
 //configure pin modes
 pinMode (enableA, OUTPUT);
 pinMode (pinA1, OUTPUT);
 pinMode (pinA2, OUTPUT);
 //pinMode (EncA, INPUT);
 //pinMode (EncB, INPUT);
}

void loop() {
 //enabling motor A
 //Serial.println ("Enabling Motor A");
 analogWrite (enableA, 120);
 //digitalWrite (enableA, HIGH);
//do something
 //forwar
 //Serial.println ("Forward");
digitalWrite (pinA2, HIGH);
analogWrite (pinA1, 10);
//int motorPinA = digitalRead (EncA);
//Serial.println("motorPinA: " + motorPinA);
//Serial.println(motorPinA, DEC);
//int motorPinB = digitalRead (EncB);
//Serial.println("motorPinB: " + motorPinB);
//Serial.println(motorPinB, DEC);
//5s forward
delay (5000);
////reverse
//Serial.println ("Backward");
 //digitalWrite (pinA1,LOW);
 //digitalWrite (pinA2,HIGH);
////5s backwards
// delay (5000);
////stop
// Serial.println ("Stopping Motor A");
//digitalWrite (pinA1, LOW);
//digitalWrite (pinA2, LOW);
//delay (5000);
 }

