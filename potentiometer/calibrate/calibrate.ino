 //Testing the DC Motors
 int potPinR = A0;
 int potPinL = A1;
 int pinA1 = 10;
 int pinA2 = 11;
 int pinB1 = 5;
 int pinB2 = 6;
int pwm = 2;

int current_time;

void setup() {
 Serial.begin (9600);
 pinMode (pinA1, OUTPUT);
 pinMode (pinA2, OUTPUT);
 delay(3000);
}
void loop() {
 current_time = millis();
 int readingR  = analogRead(potPinR);
 int readingL  = analogRead(potPinL);
 digitalWrite (pinA1, LOW);
 analogWrite (pinA2, pwm);
 digitalWrite (pinB1, LOW);
 analogWrite (pinB2, pwm);
 //print values for left pot
 Serial.print(readingL, DEC);
 Serial.print(" ");
 Serial.println(current_time, DEC);
 }
