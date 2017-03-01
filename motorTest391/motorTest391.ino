 //Testing the DC Motors
//Define Pins
 //Motor A
 int enableA = 10;
 int pinA1 = 4;
 int pinA2 = 7;
int pwm = 50;

void setup() {
Serial.begin (9600);
 //configure pin modes
 pinMode (enableA, OUTPUT);
 pinMode (pinA1, OUTPUT);
 pinMode (pinA2, OUTPUT);
}
void loop() {
 //enabling motor A


//do something
 //forward
 Serial.println ("Forward");
 analogWrite (pinA1, 0);
 analogWrite (pinA2, 255);
//5s forward
 //delay (5000);
//reverse
 //Serial.println ("Backward");
 //digitalWrite (pinA1,LOW);
 //digitalWrite (pinA2,HIGH);
//5s backwards
 //delay (5000);
//stop
 //Serial.println ("Stopping Motor A");
 //digitalWrite (pinA1, LOW);
 //digitalWrite (pinA2, LOW);
 //delay (5000);
 }
