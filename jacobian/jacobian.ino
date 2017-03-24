#include <math.h> 

//Define Pins
int L1_motor_pin = 5;
int L2_motor_pin = 6;
int R1_motor_pin = 10;
int R2_motor_pin = 11;
int L_pot_pin = A0;
int R_pot_pin = A1;

//Define Constant
float L   = 8; //length of arm

//Variables
//haptic
int L_reading;
int R_reading;
float q1_a, q2_a, x_a, y_a;
volatile int pwm_left; //for the haptic logic, being changed in check_walls()
volatile int pwm_right;


//Varialbes
//presribed_path
int q1_done;
int q2_done;
int prescribe_path = 1;

void setup() {
  Serial.begin (9600);
  
  //configure pin modes
  pinMode (L1_motor_pin, OUTPUT);
  pinMode (L2_motor_pin, OUTPUT);  
  pinMode (R1_motor_pin, OUTPUT);
  pinMode (R2_motor_pin, OUTPUT);

  pinMode(7, INPUT_PULLUP);

}

void loop(){

// prescribe_path = digitalRead(7);

 
 if(prescribe_path == 0)
  {
    do_prescribe_path();
    delay(20000);
  }
  
  L_reading = analogRead(A0);
  R_reading = analogRead(A1);
  q1_a = calculate_q1(L_reading);
  q2_a = calculate_q2(R_reading);
  x_a = direct_kin_x(q1_a, q2_a);
  y_a = direct_kin_y(q1_a, q2_a);
    
  Serial.print("x_a is ");
  Serial.print(x_a, DEC);
  Serial.print("y_a is ");
  Serial.println(y_a, DEC);

  check_walls();
}

float calculate_q1(int L_reading)
{
  float q1 =  -(((867 - ((float)L_reading)) / 34) + 3);

  return q1;
}

float calculate_q2(int R_reading)
{
  float q2 = ((875 - ((float)R_reading)) / 32) + 3;

  return q2;
}

float direct_kin_x (float q1, float q2){
    float x = 0;
    x = (q1 + q2) / 2;
    return x;    
 }

float direct_kin_y (float q1, float q2){
    float y = 0, delta_q = 0;
    delta_q = (abs(q1 - q2) / 2);
    y = sqrt(square(L) - square(delta_q));

    return y;
 }

 
float inverse_kin_q1 (float x, float y){
    float q1 = 0, delta_q = 0;
    
    delta_q = sqrt(square(L) - square(y));
    q1 = x - delta_q;

    return q1;
 }

float inverse_kin_q2 (float x, float y){
    float q2 = 0, delta_q = 0;
    
    delta_q = sqrt(square(L) - square(y));
    q2 = x + delta_q;

    return q2;
 }
 


void check_walls()
{
  if (x_a >= 0.8)
  {
    pwm_right = 50;
    in_motor_right(pwm_right);
    off_motor_left();
  }
  
  else if (x_a <= -0.8)
  {
    pwm_left = 50;
    in_motor_left(pwm_left);
     off_motor_right();    
  }
  else
  {
    off_motor_right();
    off_motor_left();
  }
  
  return;
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

void in_motor_left(int pwm_left)
{
  digitalWrite(L1_motor_pin, HIGH);
  analogWrite(L2_motor_pin, pwm_left);

  return;
}

void out_motor_left(int pwm_left)
{
  digitalWrite(L2_motor_pin, HIGH);
  analogWrite(L1_motor_pin, pwm_left);

  return;
}

void in_motor_right(int pwm_right)
{
  digitalWrite(R2_motor_pin, HIGH);
  analogWrite(R1_motor_pin, pwm_right);

  return;
}

void out_motor_right(int pwm_right)
{
  digitalWrite(R1_motor_pin, HIGH);
  analogWrite(R2_motor_pin, pwm_right);

  return;
}
 
void do_prescribe_path() {
  float x_curr, y_curr, q1_curr, q2_curr;
  float x_next, y_next, q1_next, q2_next;
  int pp_pwm_left = 0; 
  int pp_pwm_right = 0;

  q1_curr = calculate_q1(analogRead(L_pot_pin));
  q2_curr = calculate_q2(analogRead(R_pot_pin));

 /* x_next = 1;
  y_next = 6.8;
  q1_next = inverse_kin_q1(x_next, y_next);
  q2_next = inverse_kin_q2(x_next, y_next); */

  q1_next = -3;
  q2_next = 4.23;

  Serial.print("q1_curr is ");
  Serial.print(q1_curr, DEC);
  Serial.print("q2_curr is ");
  Serial.println(q2_curr, DEC);
  
  Serial.print("q1_next is ");
  Serial.print(q1_next, DEC);
  Serial.print("q2 next is ");
  Serial.println(q2_next, DEC);


  //clear flags before each move
  q1_done = 0;
  q2_done = 0;

  while(((q1_done == 0) && (abs(q1_next - q1_curr) >= 0.4)) ||
        ((q2_done == 0) && (abs(q2_next - q2_curr) >= 0.4)))//what if one matched, stopped, another contiuned and DRAGGED the first one
  //chances of inifinite loop?
  //PD control to lock it up; currently using flags to lock it up
  {
    if(abs(q2_next - q2_curr) >= 0.4)
    {
      if ((q2_next - q2_curr) >= 0.4)
      {
     
        out_motor_right(200);
      }
      else if((q2_curr - q2_next) >= 0.4)
      {
        in_motor_right(200);
      }
    }
    else if(abs(q1_next - q1_curr) >= 0.4)
    {
      if ((q1_next - q1_curr) >= 0.4)
      {
        in_motor_left(200);
      }
      else if((q1_curr - q1_next) >= 0.4)
      {
        out_motor_left(200);
      }
    }
    else
    {
      off_motor_right();
      off_motor_left();
    }
    
     q1_curr = calculate_q1(analogRead(L_pot_pin));
     q2_curr = calculate_q2(analogRead(R_pot_pin));

     //flags setting, if one of them is done, don't ever change it again
     q1_done = (abs(q1_curr - q1_next) < 0.4) ? 1 : 0;
     q2_done = (abs(q2_curr - q2_next) < 0.4) ? 1 : 0;
     
   
      
 
 
  }
    Serial.print("q1_curr is ");
  Serial.print(q1_curr, DEC);
  Serial.print("q2_curr is ");
  Serial.println(q2_curr, DEC);
  off_motor_right();
  off_motor_left();
  
  q1_done = 0;
  q2_done = 0;
  
  q1_curr = calculate_q1(analogRead(L_pot_pin));
  q2_curr = calculate_q2(analogRead(R_pot_pin));
  q1_next = -4.23;
  q2_next = q2_curr;

  while(((q1_done == 0) && (abs(q1_next - q1_curr) >= 0.4)) ||
        ((q2_done == 0) && (abs(q2_next - q2_curr) >= 0.4)))//what if one matched, stopped, another contiuned and DRAGGED the first one
  //chances of inifinite loop?
  //PD control to lock it up; currently using flags to lock it up
  {
    if(abs(q2_next - q2_curr) >= 0.4)
    {
      if ((q2_next - q2_curr) >= 0.4)
      {
        out_motor_right(200);
      }
      else if((q2_curr - q2_next) >= 0.4)
      {
        in_motor_right(200);
      }
    }
    else if(abs(q1_next - q1_curr) >= 0.4)
    {
      if ((q1_next - q1_curr) >= 0.4)
      {
        in_motor_left(200);
      }
      else if((q1_curr - q1_next) >= 0.4)
      {
        out_motor_left(200);
      }
    }
    else
    {
      off_motor_right();
      off_motor_left();
    }
    
 
  q1_curr = calculate_q1(analogRead(L_pot_pin));
  q2_curr = calculate_q2(analogRead(R_pot_pin));
     //flags setting, if one of them is done, don't ever change it again
     q1_done = (abs(q1_curr - q1_next) < 0.4) ? 1 : 0;
     q2_done = (abs(q2_curr - q2_next) < 0.4) ? 1 : 0;
     
   
      

 
  }

     Serial.print("q1_curr is ");
  Serial.print(q1_curr, DEC);
  Serial.print("q2_curr is ");
  Serial.println(q2_curr, DEC);
  return;
  
  /*Serial.print(millis(), DEC);
  Serial.print("  ");
  Serial.println(analogRead(potPin), DEC); //this means an enter is followed*/

 }


