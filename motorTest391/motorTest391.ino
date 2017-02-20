 //Testing the DC Motors
//Define Pins
 //Motor A
 int enableA = 10;
 int pinA1 = 5;
 int pinA2 = 6;

void setup() {
Serial.begin (9600);
 //configure pin modes
 pinMode (enableA, OUTPUT);
 pinMode (pinA1, OUTPUT);
 pinMode (pinA2, OUTPUT);
}
void loop() {
 //enabling motor A
 Serial.println ("Enabling Motor A");
 digitalWrite (enableA, 50);
//do something
 //forward
 Serial.println ("Forward");
 digitalWrite (pinA1, HIGH);
 digitalWrite (pinA2, LOW);
//5s forward
 delay (5000);
//reverse
 /*Serial.println ("Backward");
 digitalWrite (pinA1,LOW);
 digitalWrite (pinA2,HIGH);
//5s backwards
 delay (5000);
//stop
 Serial.println ("Stopping Motor A");
 digitalWrite (enableA, LOW);
 delay (5000);*/
 }
