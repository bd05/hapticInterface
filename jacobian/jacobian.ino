#include <math.h> 
#include<stdlib.h>

//Define Constant
#define THETAL 55
#define THETAR 50
#define ARM 12
#define B 26

//Custom PWM stuff
//Define Pins

int L1_motor_pin = 5;
int L2_motor_pin = 6;
int R1_motor_pin = 10;
int R2_motor_pin = 11;
int MAX_PWM = 255;
int L_pot_pin = A1;
int R_pot_pin = A0;



//Variables
//haptic
int L_reading;
int R_reading;
float last_time;

float last_ql;
float last_qr;
float p_i_cumulative_R;
float p_i_cumulative_L;

char beginR = 'B';
char endR = 'E';
char beginL = 'C';
char endL = 'F';



volatile int pwm_left; //for the haptic logic, being changed in check_walls()
volatile int pwm_right;




//Varialbes
//presribed_path
int q1_done;
int q2_done;
int prescribe_path = 1;

void setup() {

  last_time = micros();
  last_ql = 0.0;
  last_qr = 0.0;
  p_i_cumulative_R = 0.0;
  Serial.begin (9600);

  //configure pin modes
  pinMode (L1_motor_pin, OUTPUT);
  pinMode (L2_motor_pin, OUTPUT);  
  pinMode (R1_motor_pin, OUTPUT);
  pinMode (R2_motor_pin, OUTPUT);
  pinMode(7, INPUT_PULLUP);

  //setPwmFrequency(6, 1024);
  //setPwmFrequency(5, 1024);
  setPwmFrequency(6, 256);
  setPwmFrequency(5, 256);
  setPwmFrequency(10, 64);
  setPwmFrequency(11, 64);
}


void loop(){
// prescribe_path = digitalRead(7);
 if(prescribe_path == 0)
  {
    //do_prescribe_path();
    delay(20000);
  }
  
  //print x and y to arduino serial monitor
  /*Serial.print("x is ");
  Serial.print(x, DEC);
  Serial.print("    ");
  Serial.print("y is ");
  Serial.println(y, DEC); */
  
  //print x and y for web client to see
 /* Serial.print("B"); // begin character 
  Serial.print(R_reading);  
  Serial.print("E"); // end character
  Serial.print("C"); // begin character 
  Serial.print(L_reading);  
  Serial.print("F"); // end character */
 // delay(50);

      

  
  //draw_figure8_haptic();
  doWallHaptic();
}
//===================================================DIRECT KINEMATICS MATH=============================================================================

float calculate_ql(int L_reading)
{

  float ql;

  if(L_reading <= 848)
  {
    ql = 2.9 - (((float)848-L_reading)/ 52);
  }
  else if(L_reading <= 867)
  {
    ql = 3.4 - (((float)867-L_reading)/ 38);
  }
  else if(L_reading <= 886)
  {
    ql = 4 - (((float)886-L_reading)/ 38);
  }
  else if(L_reading <= 905)
  {
    ql = 4.4 - (((float)905-L_reading)/ 38);
  }
  else if(L_reading <= 921)
  {
    ql = 4.9 - (((float)921-L_reading)/ 32);
  }
  else if(L_reading <= 939)
  {
    ql = 5.4 - (((float)939-L_reading)/ 36);
  }
  else if(L_reading <= 955)
  {
    ql = 5.8 - (((float)955-L_reading)/ 32);
  }
  else if(L_reading <= 973)
  {
    ql = 6.3 - (((float)955-L_reading)/ 36);
  }
  else if(L_reading <= 993)
  {
    ql =  7 - (((float)993-L_reading)/ 40);
  }
  else if(L_reading <= 1009)
  {
    ql =  7.4 - (((float)1009-L_reading)/ 32);
  }
   else 
  {
    ql =  8 - (((float)1023-L_reading)/ 28);
  }
  return ql;
 
}
float calculate_qr(int R_reading){
  float qr;

  if (R_reading <= 837)
  {
    qr = 3.1 - (((float)837-R_reading)/54);
  }
  else if (R_reading <= 859)
  {
    qr = 3.5 - (((float)859-R_reading)/44);
  }
  else if (R_reading <= 876)
  {
    qr = 3.9 - (((float)876-R_reading)/34);
  }
   else if (R_reading <= 899)
  {
    qr = 4.6 - (((float)899-R_reading)/46);
  }
   else if (R_reading <= 914)
  {
    qr = 5.1 - (((float)914-R_reading)/30);
  }
   else if (R_reading <= 930)
  {
    qr =  5.4 - (((float)930-R_reading)/32);
  }
   else if (R_reading <= 946)
  {
    qr =  6 - (((float)946-R_reading)/32);
  }
  else if(R_reading <= 968){
    qr =  6.6 - (((float)968-R_reading)/44);
  }
  else if (R_reading <= 985)
  {
    qr =  7 - (((float)985-R_reading)/34);
  }
  else if (R_reading <= 1006)
  {
    qr = 7.6 - (((float)1006-R_reading)/42);
  }
  else if (R_reading <= 1020)
  {
    qr =  8 - (((float)1020 - R_reading)/28);
  }
  else{
    qr = 8.3 - (((float)1023-R_reading)/30);
  }
  return qr;

}

float calculate_plx(float ql){
  float plx = (ql)*(0.7071); //cos(thetaL)
  return plx;
}

float calculate_ply(float ql){
  float ply = (ql)*(0.7071);//sin(thetaL)
  return ply;
}

float calculate_prx(float qr){
  float prx = (float)B - ((qr)*(0.7071));//cos(thetaR)
  //Serial.print("prx is ");
  //Serial.println(prx, DEC);
  return prx;
}

float calculate_pry(float qr){
  float pry = (qr)*(0.7071);//sin(thetaR)
  return pry;
}

float direct_kin_x (float ql, float qr){

    
 /* plx = calculacalculate_prxte_plx(ql);
  ply = calculate_ply(ql);
  prx = (qr);
  pry = calculate_pry(qr);
  x = direct_kin_x(plx,ply,prx,pry);
  y = direct_kin_y(plx,ply,prx,pry);
  */

  float pry = calculate_pry(qr + 0.65);
  float prx = calculate_prx(qr+ 0.65);

  float ply = calculate_ply(ql+ 0.65);
  float plx = calculate_plx(ql+ 0.65);
  
  float p3x = prx - plx;
  float p3y = pry - ply;
  float mag = abs(sqrt(square(p3x) + square(p3y)));
  float ux = p3x/mag;
  float uy = p3y/mag;
  float u_tran_x = -uy;
  float u_tran_y = ux; 
  float half_basex = ux*(mag/2) + plx;
  float l = sqrt(square(ARM) - square(mag/2));
  float x = l*u_tran_x + half_basex;
  return x;
}

float direct_kin_y (float ql, float qr){


  float pry = calculate_pry(qr);
  float prx = calculate_prx(qr);
  float ply = calculate_ply(ql);
  float plx = calculate_plx(ql);
  
 /* Serial.print("plx: ");
  Serial.println(plx);
  Serial.print("ply: ");
  Serial.println(ply);
  Serial.print("prx: ");
  Serial.println(prx);
  Serial.print("pry: ");
  Serial.println(pry);*/
  
  
  float p3x = prx - plx;
  float p3y = pry - ply;
  
      
  /*Serial.print("p3x: ");
  Serial.println(p3x);
  Serial.print("p3y: ");
  Serial.println(p3y);*/
  
  float mag = abs(sqrt(square(p3x) + square(p3y)));
  
 /* Serial.print("mag: ");
  Serial.println(mag);*/
  
  float ux = p3x/mag;
  float uy = p3y/mag;
  
 /* Serial.print("ux: ");
  Serial.println(ux);
  Serial.print("uy: ");
  Serial.println(uy);*/
  
  //float u_tran_x = -uy;
  //float u_tran_y = ux; 
  
   float u_tran_x = -((p3y/2)/(mag/2));
   float u_tran_y = ((p3x/2)/(mag/2));
  /*Serial.print("u_tran_x: ");
  Serial.println(u_tran_x);
  Serial.print("u_tran_y: ");
  Serial.println(u_tran_y);*/
  
  float half_basey = uy*(mag/2) + ply;
  //float half_basey = (p3y/2) + ply;
  
  /*Serial.print("half_basey: ");
  Serial.println(half_basey);*/

  float l = sqrt(square(ARM) - square(mag/2));
  //float y = l*u_tran_y + half_basey;
  float y = l*u_tran_y + half_basey;
  return y;
  

 }

 float inverse_kin_ql(float xb,float yb){ //converts x,y in Bernadette's coordinate system to ql rotor length


	

	float yo = 18;
	float xo = 18;

  float xs = xb * 0.7071 + 0.7071 * yb ;
	float ys = -xb * 0.7071 + 0.7071 * yb + yo; 

	float h = abs(ys-yo);
	float g = abs(xs-xo);

	float f = sqrt((ARM * ARM) - (h*h));
	float t = sqrt((ARM * ARM) - (g*g));

	return (xs - f);



 }

 float inverse_kin_qr(float xb,float yb){ //converts x,y to qr rotor length
 
 float yo = 18;
 float xo = 18;

	float xs = xb * 0.7071 + 0.7071 * yb ;
	float ys = -xb * 0.7071 + 0.7071 * yb + yo;


	float h = abs(ys-yo);
	float g = abs(xs-xo);

	float f = sqrt((ARM * ARM) - (h*h));
	float t = sqrt((ARM * ARM) - (g*g));

	return (ys - t);
 }
//================================================DRAW LINE=========================================================
void draw_figure8_haptic() //need to add d-control
{ 
  //recalc position
  


  float desiredX = (15-13.5)*sin(micros() * 0.000030) + 11.5;
  float desiredY = (15-13.5)*sin(micros() * 0.000030)*cos(micros() * 0.000030) + 10;

  //desiredqr = 4.5;
  //desiredql = 3.5;

 
  float pScale_L = 275;
  float dScale_L = 26000;
  float iScale_L = 0.5*0;
  
  float iScale_R = 0.5*0;
  float pScale_R = 400;
  float dScale_R = 25000;
  
  do_PID(pScale_L, dScale_L, iScale_L, pScale_R, dScale_R, iScale_R, desiredX, desiredY);
  
  
  return;
}

void doWallHaptic(){
  float topWall = 11;
  float bottomWall = 10;

  float pScale_L = 275;
  float dScale_L = 26000;
  float iScale_L = 0;
  
  float iScale_R = 0;
  float pScale_R = 400;
  float dScale_R = 26000;

  L_reading = analogRead(L_pot_pin);
  R_reading = analogRead(R_pot_pin);
  float ql = abs(calculate_ql(L_reading));
  float qr = abs(calculate_qr(R_reading));

  float currentY = direct_kin_y(ql, qr);
  float currentX = direct_kin_x(ql, qr);

  /*Serial.print("current Y: ");
  Serial.println(currentY, DEC);

  Serial.print("current X: ");
  Serial.println(currentX, DEC);

  Serial.print("ql: ");
  Serial.println(ql, DEC);

  Serial.print("qr: ");
  Serial.println(qr, DEC);*/
  //if(micros()%100 == 0){
   write_to_serial(currentX,currentY);
  //}

  float desiredY;

  if(currentY > topWall){
    desiredY = topWall;
  }
  else{
   desiredY = bottomWall;
  }
  do_PID(pScale_L, dScale_L, iScale_L, pScale_R, dScale_R, iScale_R, currentX, desiredY);
  
}

void do_PID(float pScale_L,float dScale_L,float iScale_L, float pScale_R,float dScale_R, float iScale_R, float desiredX, float desiredY){
  L_reading = analogRead(L_pot_pin);
  R_reading = analogRead(R_pot_pin);
  float ql = abs(calculate_ql(L_reading));
  float qr = abs(calculate_qr(R_reading));

  
  float desiredqr = inverse_kin_qr(desiredX,desiredY);
  float desiredql = inverse_kin_ql(desiredX,desiredY);

  float p_L = (desiredql - ql) * pScale_L;
  float p_R = (desiredqr - qr) * pScale_R;

  
  
  
  float deltaQ_L = -(last_ql - ql);
  float deltaQ_R = -(last_qr - qr);
  
  float deltaT = (float)(micros() - last_time);
  
  float V_L = deltaQ_L / deltaT;
  float V_R = deltaQ_R / deltaT;
 

  float d_L = -V_L * dScale_L; 
  float d_R = -V_R * dScale_R;

  p_i_cumulative_R += (desiredqr - qr);

  float i_R = p_i_cumulative_R * iScale_R;
  float i_L = p_i_cumulative_L * iScale_L;
  float pd_L = p_L + d_L + i_L;
  float pd_R = p_R + d_R + i_R;
  
  motor_right(round(pd_R)  );

  motor_left(round(pd_L));
  last_time = micros();
  last_ql = ql;
  last_qr = qr;
}
void off_motor_left()
{
  digitalWrite(L1_motor_pin, LOW);
  digitalWrite(L2_motor_pin, LOW);
  
  return;
}

void off_motor_right()
{
  digitalWrite(R1_motor_pin, LOW);
  digitalWrite(R2_motor_pin, LOW);
  
  
  return;
}

void motor_left(int pwm_left){

  int pwm_left_limited;

    
  if(abs(pwm_left) >= 255){

    if(pwm_left > 0){
      pwm_left_limited = 254;
    }

    else{
       pwm_left_limited = -254;
    }
  }
  else{
    pwm_left_limited = pwm_left;
  }
  if(pwm_left < 0){

  //  Serial.print("LEFT going IN with pwm: ");
 //  Serial.println(pwm_left_limited, DEC);
    
    in_motor_left(abs(pwm_left_limited));
  }
  else{
 //   Serial.print("LEFT going OUT with pwm: ");
  // Serial.println(pwm_left_limited, DEC);
    
    out_motor_left(abs(pwm_left_limited));
  }
}

void motor_right(int pwm_right){
  int pwm_right_limited;

    
  if(abs(pwm_right) >= 255){

    if(pwm_right > 0){
      pwm_right_limited = 254;
    }

    else{
       pwm_right_limited = -254;
    }
  }
  else{
    pwm_right_limited = pwm_right;
  }
  if(pwm_right < 0){

  //  Serial.print("LEFT going IN with pwm: ");
 //  Serial.println(pwm_left_limited, DEC);
    
    in_motor_right(abs(pwm_right_limited));
  }
  else{
 //   Serial.print("LEFT going OUT with pwm: ");
  // Serial.println(pwm_left_limited, DEC);
    
    out_motor_right(abs(pwm_right_limited));
  }
}

void in_motor_left(int pwm_left)
{
  digitalWrite(L2_motor_pin, LOW);
  analogWrite(L1_motor_pin, pwm_left);

  return;
}

void out_motor_left(int pwm_left)
{
  digitalWrite(L1_motor_pin, LOW);
  analogWrite(L2_motor_pin, pwm_left);
 // Serial.print("TEST  ");
  return;
}

void in_motor_right(int pwm_right)
{
  digitalWrite(R2_motor_pin, LOW);
  analogWrite(R1_motor_pin, pwm_right);

  return;
}


void out_motor_right(int pwm_right)
{
  digitalWrite(R1_motor_pin, LOW);
  analogWrite(R2_motor_pin, pwm_right);

  return;
}

//================================================WRITE TO SERIAL MONITOR======================================
void write_to_serial(float x, float y){
    /*float test = 12.12345;
    char buffer[8];
    dtostrf(test,2,3,buffer);
    Serial.write(buffer); */
  
      char xStr[6];
      if( x < 10){
        char xBuffer[1];
        dtostrf(x,2,3,xBuffer);
        sprintf(xStr, "%c%c%c%c%c", beginR, xBuffer[0],xBuffer[1],xBuffer[2],endR);
      }
      else{
        char xBuffer[2];
        dtostrf(x,2,3,xBuffer);
        sprintf(xStr, "%c%c%c%c%c%c", beginR, xBuffer[0],xBuffer[1],xBuffer[2],xBuffer[3],endR);
      }
      Serial.write(xStr);

      char yStr[6];
      if( y < 10){
        char yBuffer[1];
        dtostrf(y,2,3,yBuffer);
        sprintf(yStr, "%c%c%c%c%c", beginL, yBuffer[0],yBuffer[1],yBuffer[2],endL);
      }
      else{
        char yBuffer[2];
        dtostrf(y,2,3,yBuffer);
        sprintf(yStr, "%c%c%c%c%c%c", beginL, yBuffer[0],yBuffer[1],yBuffer[2],yBuffer[3],endL); 
      }
      Serial.write(yStr);

}
//===================================CUSTOM PWM FREQUENCY====================================================
 /**
 * Divides a given PWM pin frequency by a divisor.
 * 
 * The resulting frequency is equal to the base frequency divided by
 * the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
 *        256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
 *        128, 256, and 1024.
 * 
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired on timer0
 *   - Pins 9 and 10 are paired on timer1
 *   - Pins 3 and 11 are paired on timer2
 * 
 * Note that this function will have side effects on anything else
 * that uses timers:
 *   - Changes on pins 3, 5, 6, or 11 may cause the delay() and
 *     millis() functions to stop working. Other timing-related
 *     functions may also be affected.
 *   - Changes on pins 9 or 10 will cause the Servo library to function
 *     incorrectly.
 * 
 * Thanks to macegr of the Arduino forums for his documentation of the
 * PWM frequency divisors. His post can be viewed at:
 *   http://forum.arduino.cc/index.php?topic=16612#msg121031
 */
/**
 * Divides a given PWM pin frequency by a divisor.
 * 
 * The resulting frequency is equal to the base frequency divided by
 * the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
 *        256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
 *        128, 256, and 1024.
 * 
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired on timer0
 *   - Pins 9 and 10 are paired on timer1
 *   - Pins 3 and 11 are paired on timer2
 * 
 * Note that this function will have side effects on anything else
 * that uses timers:
 *   - Changes on pins 3, 5, 6, or 11 may cause the delay() and
 *     millis() functions to stop working. Other timing-related
 *     functions may also be affected.
 *   - Changes on pins 9 or 10 will cause the Servo library to function
 *     incorrectly.
 * 
 * Thanks to macegr of the Arduino forums for his documentation of the
 * PWM frequency divisors. His post can be viewed at:
 *   http://forum.arduino.cc/index.php?topic=16612#msg121031
 */
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}



