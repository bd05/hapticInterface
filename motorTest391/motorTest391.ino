 //Testing the DC Motors
//Define Pins
 //Motor A
 int pinA1 = 5;
 int pinA2 = 6;
int pwm = 50;

void setup() {
Serial.begin (9600);
 //configure pin modes
 pinMode (pinA1, OUTPUT);
 pinMode (pinA2, OUTPUT);
}
void loop() {
 //enabling motor A
 Serial.println ("Forward");
 analogWrite (pinA1, 10);
 analogWrite (pinA2, pwm);
 }
