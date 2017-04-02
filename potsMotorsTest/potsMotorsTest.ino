 //Testing the DC Motors
//Define Pins
int potPinR = A0;
int potPinL = A1;
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
 pinMode (pinB1, OUTPUT);
 pinMode (pinB2, OUTPUT);
}
void loop() {
 int readingR  = analogRead(potPinR);
 int readingL  = analogRead(potPinL);
 Serial.print("right pot: ");
 Serial.print(readingR);
 Serial.print("\t");
 Serial.print("left pot: ");
 Serial.println(readingL);
 //motors
 digitalWrite (pinA1, HIGH);
 analogWrite (pinA2, pwm);
 digitalWrite (pinB1, HIGH);
 analogWrite (pinB2, pwm);
 delay(100);
 }
