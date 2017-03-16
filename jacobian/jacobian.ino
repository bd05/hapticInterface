#include <math.h> 

 int pinA1 = 5;
 int pinA2 = 3;
float L   = 8; //length of arm


void setup() {
Serial.begin (9600);
 //configure pin modes

 pinMode (pinA1, OUTPUT);
 pinMode (pinA2, OUTPUT);

}
  
void loop() {
  float x = 0, y = 0, q1 = 0, q2 = 0;


  digitalWrite (pinA2, HIGH);
  analogWrite (pinA1, 60);
  q1 = 3;
  q2 = 4;
  x = Direct_Kin_x(q1, q2);
  y = Direct_Kin_y(q1, q2);
  q1 = Inverse_Kin_q1(x, y);
  q2 = Inverse_Kin_q2(x, y);
  Serial.print(x, DEC);
  Serial.println("\t");
  Serial.print(y, DEC); 
  Serial.println("\t");
  Serial.print(q1, DEC); 
  Serial.println("\t");
  Serial.print(q2, DEC); 
  Serial.println("\t");
  

delay (5000);

 }

float Direct_Kin_x (float q1, float q2){
    float x = 0;

    x = (q1 + q2) / 2;
    return x;    
 }

float Direct_Kin_y (float q1, float q2){
    float y = 0, delta_q = 0;
    delta_q = (abs(q1 - q2) / 2);
    y = sqrt(square(L) - square(delta_q));

    return y;
 }

 
float Inverse_Kin_q1 (float x, float y){
    float q1 = 0, delta_q = 0;
    
    delta_q = sqrt(square(L) - square(y));
    q1 = x - delta_q;

    return q1;
 }

float Inverse_Kin_q2 (float x, float y){
    float q2 = 0, delta_q = 0;
    
    delta_q = sqrt(square(L) - square(y));
    q2 = x + delta_q;

    return q2;
 }
 


