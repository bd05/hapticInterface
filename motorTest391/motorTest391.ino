 //Testing the DC Motors
//Define Pins
 //Motor A
 int pinA1 = 10;
 int pinA2 = 11;
 int pinB1 = 5;
 int pinB2 = 6;
int pwm = 100;

void setup() {
Serial.begin (9600);
 //configure pin modes
 pinMode (pinA1, OUTPUT);
 pinMode (pinA2, OUTPUT);
}
void loop() {
 //enabling motor A
 Serial.println ("Forward");
 digitalWrite (pinA1, HIGH);
 analogWrite (pinA2, pwm);
 digitalWrite (pinB1, HIGH);
 analogWrite (pinB2, pwm);
 }
