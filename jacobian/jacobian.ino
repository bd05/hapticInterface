#include <math.h> 


//Define Constant
#define THETAL 55 // TODO: Didn't we re-measure these to be 45??  
#define THETAR 50
#define ARM 12.6
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
float ql,qr,x,y;
float plx;
float ply;
float prx;
float pry;
float last_time;

float last_ql;
float last_qr;
float p_i_cumulative_R;
float p_i_cumulative_L;

char beginR = 'B';
char endR = 'E';
char beginL = 'C';
char endL = 'E';



volatile int pwm_left; //for the haptic logic, being changed in check_walls()
volatile int pwm_right;


//Varialbes
//presribed_path
int q1_done;
int q2_done;
int prescribe_path = 1;

//LED status pins
const int led13 = 13;
const int ledRed = A4;
const int ledGreen = A5;
String inputString = "";         // a string to hold incoming data
boolean toggleComplete = false;  // whether the string is complete
int fig8Count = 0;

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
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  pinMode(A4,OUTPUT);
  digitalWrite(A4,LOW);
  pinMode(A5,OUTPUT);
  analogWrite(A5,LOW);


/*
  
  setPwmFrequency(6, 256);
  setPwmFrequency(5, 256);
  */

  setPwmFrequency(6, 256);
  setPwmFrequency(5, 256);
  setPwmFrequency(10, 8);
  setPwmFrequency(11, 8);
  

 /*   setPwmFrequency(6, 8);
  setPwmFrequency(5, 8);
  setPwmFrequency(10, 8);
  setPwmFrequency(11, 8);*/
}


void loop(){
//===============================================UI=====================================
  // Recieve data from Node and write it to a String    
  while (Serial.available() && toggleComplete == false) {
    char inChar = (char)Serial.read();
    if(inChar == 'E'){ // end character for toggle LED
     toggleComplete = true;
    }
    else{
      inputString += inChar; 
    }
  }
  if(!Serial.available() && toggleComplete == true)
  {
    int receivedVal = stringToInt(); // convert String to int. 
    if(receivedVal == 7){
       digitalWrite(ledGreen, HIGH);
      while (digitalRead(7))
      {
        draw_spiral_haptic();
      }
      inputString = "";
      toggleComplete = false;
      off_motor_left();
      off_motor_right();
        digitalWrite(ledGreen, LOW);
    }
    else if(receivedVal == 4){
       digitalWrite(ledGreen, HIGH);
      while (digitalRead(7))
      {
        draw_flower_haptic();
      }
      inputString = "";
      toggleComplete = false;
      off_motor_left();
      off_motor_right();
       digitalWrite(ledGreen, LOW);
    }
    else if(receivedVal == 6){
      digitalWrite(ledGreen, HIGH);
      while (digitalRead(7))
      {
        draw_figure8_haptic();
      }
      inputString = "";
      toggleComplete = false;
      off_motor_left();
      off_motor_right();
      digitalWrite(ledGreen, LOW);
    }  
     else if(receivedVal == 5){
      digitalWrite(ledGreen, HIGH);
      while (digitalRead(7))
      {
        draw_circle_haptic();
      }
      inputString = "";
      toggleComplete = false;
      off_motor_left();
      off_motor_right();
      digitalWrite(ledGreen, LOW);
    }
  // TODO: Add haptic modes
    /* 
     else if(receivedVal == 3){
      digitalWrite(ledGreen, HIGH);
      while (digitalRead(7))
      {
        doElasticHaptic();
      }
      inputString = "";
      toggleComplete = false;
      off_motor_left();
      off_motor_right();
      digitalWrite(ledGreen, LOW);
    } 
     else if(receivedVal == 2){
      digitalWrite(ledGreen, HIGH);
      while (digitalRead(7))
      {
        doCanoeingHaptic();
      }
      inputString = "";
      toggleComplete = false;
      off_motor_left();
      off_motor_right();
      digitalWrite(ledGreen, LOW);
    } 
     else if(receivedVal == 1){
      digitalWrite(ledGreen, HIGH);
      while (digitalRead(7))
      {
        doRhombusHaptic();
      }
      inputString = "";
      toggleComplete = false;
      off_motor_left();
      off_motor_right();
      digitalWrite(ledGreen, LOW);
    } */
     else if(receivedVal == 0){
      digitalWrite(ledGreen, HIGH);
      while (digitalRead(7))
      {
        doWallHaptic();
      }
      inputString = "";
      toggleComplete = false;
      off_motor_left();
      off_motor_right();
      digitalWrite(ledGreen, LOW);
    }
  }
      

  //draw_flower_haptic();
 //draw_figure8_haptic();
 //draw_spiral_haptic();
  //doWallHaptic();
}
//===================================================DIRECT KINEMATICS MATH=============================================================================

float calculate_ql(int L_reading)
{

  float ql;

  if(L_reading <= 855)
  {
    ql = 3 - (((float)855-L_reading)/ 52);
  }
  else if(L_reading <= 861)
  {
    ql = 3.3 - (((float)861-L_reading)/ 24);
  }
  else if(L_reading <= 871)
  {
    ql = 3.5 - (((float)871-L_reading)/ 40);
  }
  else if(L_reading <= 881)
  {
    ql = 3.7 - (((float)881-L_reading)/ 40);
  }
  else if(L_reading <= 890)
  {
    ql = 4 - (((float)890-L_reading)/ 36);
  }
  else if(L_reading <= 900)
  {
    ql = 4.2 - (((float)900-L_reading)/ 40);
  }
  else if(L_reading <= 908)
  {
    ql = 4.5 - (((float)908-L_reading)/ 32);
  }
  else if(L_reading <= 916)
  {
    ql =  4.8 - (((float)916-L_reading)/ 32);
  }
  else if(L_reading <= 926)
  {
    ql =  5 - (((float)926-L_reading)/ 40);
  }
  else if(L_reading <= 936)
  {
    ql =  5.2 - (((float)936-L_reading)/ 40);
  }
   else if(L_reading <= 944)
  {
    ql =  5.4 - (((float)944-L_reading)/ 32);
  } 
  else if(L_reading <= 953)
  {
    ql =  5.8 - (((float)953-L_reading)/ 36);
  } 
  else if(L_reading <= 965)
  {
    ql =  6.1 - (((float)965-L_reading)/ 48);
  }
  else if(L_reading <= 970)
  {
    ql = 6.2  - (((float)970-L_reading)/ 20);
  } 
  else if(L_reading <= 979)
  {
    ql =  6.4 - (((float)979-L_reading)/ 36);
  } 
  else if(L_reading <= 988)
  {
    ql =  6.8 - (((float)988-L_reading)/ 36);
  } 
  else if(L_reading <= 996)
  {
    ql =  7 - (((float)996-L_reading)/ 32);
  }
  else if(L_reading <= 1006)
  {
    ql =  7.2 - (((float)1006-L_reading)/ 40);
  } 
  else if(L_reading <= 1015)
  {
    ql =  7.4 - (((float)1015-L_reading)/ 36);
  }
   else 
  {
    ql =  8 - (((float)1023-L_reading)/ 32);
  }
  return ql;
 
}
float calculate_qr(int R_reading){
  float qr;

  if (R_reading <= 868)
  {
    qr = 3.9 - (((float)868-R_reading)/44);
  }
  else if (R_reading <= 876)
  {
    qr = 4.1 - (((float)876-R_reading)/32);
  }
  else if (R_reading <= 887)
  {
    qr = 4.4 - (((float)887-R_reading)/44);
  }
   else if (R_reading <= 896)
  {
    qr = 4.5 - (((float)896-R_reading)/36);
  }
   else if (R_reading <= 906)
  {
    qr = 4.6 - (((float)906-R_reading)/40);
  }
   else if (R_reading <= 915)
  {
    qr =  4.8 - (((float)915-R_reading)/36);
  }
   else if (R_reading <= 925)
  {
    qr =  5.1 - (((float)925-R_reading)/40);
  }
  else if(R_reading <= 933){
    qr =  5.4 - (((float)933-R_reading)/32);
  }
  else if (R_reading <= 941)
  {
    qr =  5.5 - (((float)941-R_reading)/32);
  }
  else if (R_reading <= 951)
  {
    qr = 5.9 - (((float)951-R_reading)/40);
  }
  else if (R_reading <= 956)
  {
    qr =  6.1 - (((float)956 - R_reading)/20);
  }
    else if (R_reading <= 965)
  {
    qr =  6.3 - (((float)965 - R_reading)/36);
  }
    else if (R_reading <= 970)
  {
    qr =  6.4 - (((float)970 - R_reading)/20);
  }
    else if (R_reading <= 986)
  {
    qr =  6.8 - (((float)986 - R_reading)/64);
  }
    else if (R_reading <= 992)
  {
    qr =  6.9 - (((float)992 - R_reading)/24);
  }
    else if (R_reading <= 1002)
  {
    qr =  7.2 - (((float)1002 - R_reading)/40);
  }
    else if (R_reading <= 1011)
  {
    qr =  7.5 - (((float)1011 - R_reading)/36);
  }
    else if (R_reading <= 1020)
  {
    qr =  7.7 - (((float)1020 - R_reading)/36);
  }
  else
  {
    qr = 8 - (((float)1023 - R_reading)/12);
  }
  return qr;

}

float calculate_plx(float ql){
  float plx = ql*(0.7071); //cos(thetaL)
  return plx;
}

float calculate_ply(float ql){
  float ply = ql*(0.7071);//sin(thetaL)
  return ply;
}

float calculate_prx(float qr){
  float prx = (float)B - (qr*(0.7071));//cos(thetaR)
  //Serial.print("prx is ");
  //Serial.println(prx, DEC);
  return prx;
}

float calculate_pry(float qr){
  float pry = qr*(0.7071);//sin(thetaR)
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

  float pry = calculate_pry(qr);
  float prx = calculate_prx(qr);

  float ply = calculate_ply(ql);
  float plx = calculate_plx(ql);
  
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
  float omega = 0.0000060;
  float period = (2*PI)/omega;

  float desiredX = (15-13.5)*sin(micros() * omega) + 13;
  float desiredY = (15-13.5)*sin(micros() * omega)*cos(micros() * omega) + 12;

//========draw actual position on GUI======================
  if(micros()%300 == 0){
      L_reading = analogRead(L_pot_pin); //try  moving this out of the if statement if problems occur
      R_reading = analogRead(R_pot_pin);
      ql = abs(calculate_ql(L_reading));
      qr = abs(calculate_qr(R_reading));
      float currentY = direct_kin_y(ql, qr);
      float currentX = direct_kin_x(ql, qr);
      write_to_serial(currentX,currentY);
  }
//=========================================================
  
 // float desiredX = 13; 
  //float desiredY = 12;
  
  //desiredqr = 4.5;
  //desiredql = 3.5;

  float pScale_L = 350;
  float dScale_L = 26000;
  float iScale_L = 0.05;
  
  float iScale_R = 0.05;
  float pScale_R = 450;
  float dScale_R = 21000;

  do_PID(pScale_L, dScale_L, iScale_L, pScale_R, dScale_R, iScale_R, desiredX, desiredY);
  
  /*fig8Count++;
  if(fig8Count > 5000){
    toggleComplete = false;
    analogWrite(ledGreen,0);
    return;
  }*/
}

void draw_flower_haptic() //need to add d-control
{ 
  //recalc position

  float omega = 0.0000010;
  float period = (2*PI)/omega;

 
 float desiredX = (2*(sin(4*(micros() * omega))*cos(micros()*omega)))+13;
  float desiredY = (2*(sin(4*(micros() * omega))*sin(micros()*omega)))+12;

   /* if(micros()%650 == 0){ 
      L_reading = analogRead(L_pot_pin); //try  moving this out of the if statement if problems occur
      R_reading = analogRead(R_pot_pin);
      ql = abs(calculate_ql(L_reading));
      qr = abs(calculate_qr(R_reading));
      float currentY = direct_kin_y(ql, qr);
      float currentX = direct_kin_x(ql, qr);
      write_to_serial(currentX,currentY);*/
  if(micros()%300 == 0){
      write_to_serial(desiredX,desiredY);
  }
  
 // float desiredX = 13; 
  //float desiredY = 12;
  
  //desiredqr = 4.5;
  //desiredql = 3.5;

  float pScale_L = 350;
  float dScale_L = 26000;
  float iScale_L = 0.05;
  
  float iScale_R = 0.05;
  float pScale_R = 450;
  float dScale_R = 21000;

  do_PID(pScale_L, dScale_L, iScale_L, pScale_R, dScale_R, iScale_R, desiredX, desiredY);
  
  return;
}



void draw_circle_haptic() //need to add d-control
{ 
  //recalc position

  float omega = 0.0000080;
  float period = (2*PI)/omega;

   float desiredX = (15-13.5)*sin(micros() * omega) + 12;
  float desiredY = (15-13.5)*cos(micros() * omega) + 12;

    if(micros()%300 == 0){ 
      L_reading = analogRead(L_pot_pin); //try  moving this out of the if statement if problems occur
      R_reading = analogRead(R_pot_pin);
      ql = abs(calculate_ql(L_reading));
      qr = abs(calculate_qr(R_reading));
      float currentY = direct_kin_y(ql, qr);
      float currentX = direct_kin_x(ql, qr);
      write_to_serial(currentX,currentY);}
  /*if(micros()%300 == 0){
      write_to_serial(desiredX,desiredY);
  }*/
  
 // float desiredX = 13; 
  //float desiredY = 12;
  
  //desiredqr = 4.5;
  //desiredql = 3.5;

  float pScale_L = 350;
  float dScale_L = 26000;
  float iScale_L = 0.05;
  
  float iScale_R = 0.05;
  float pScale_R = 450;
  float dScale_R = 21000;

  do_PID(pScale_L, dScale_L, iScale_L, pScale_R, dScale_R, iScale_R, desiredX, desiredY);
  
  return;
}
void draw_spiral_haptic() //need to add d-control
{ 
  //recalc position
  
  float omega = 0.0000030;
  float period = (2*PI)/omega;

   int actual_period; 
   int modulated_period; 

   float amplitude;
   float desiredX;
   float desiredY;

   actual_period = micros()/period;
   modulated_period = actual_period % 8;

   amplitude = 1 * sin(micros() * (omega/5)) + 0.50;
   
   desiredX = amplitude *sin(micros() * omega) + 13;
   desiredY = amplitude *cos(micros() * omega) + 12;
   
   if(micros()%350 == 0){
      L_reading = analogRead(L_pot_pin); //try  moving this out of the if statement if problems occur
      R_reading = analogRead(R_pot_pin);
      ql = abs(calculate_ql(L_reading));
      qr = abs(calculate_qr(R_reading));
      float currentY = direct_kin_y(ql, qr);
      float currentX = direct_kin_x(ql, qr);
      write_to_serial(currentX,currentY);
      //write_to_serial(desiredX,desiredY);
  }

 // float desiredX = 13; 
  //float desiredY = 12;
  
  //desiredqr = 4.5;
  //desiredql = 3.5;

  float pScale_L = 350;
  float dScale_L = 26000;
  float iScale_L = 0.05;
  
  float iScale_R = 0.05;
  float pScale_R = 450;
  float dScale_R = 21000;

  do_PID(pScale_L, dScale_L, iScale_L, pScale_R, dScale_R, iScale_R, desiredX, desiredY);
  
  
  return;
}

void doWallHaptic(){
  float topWall = 13.5;
  float bottomWall = 12.5;

  float leftWall = 13;
  

  float pScale_L = 17505;
  float dScale_L = 155000;
  float iScale_L = 0;
  
  float iScale_R = 0;
  float pScale_R = 17505;
  float dScale_R = 155000;

  L_reading = analogRead(L_pot_pin);
  R_reading = analogRead(R_pot_pin);
  ql = abs(calculate_ql(L_reading));
  qr = abs(calculate_qr(R_reading));

  float currentY = direct_kin_y(ql, qr);
  float currentX = direct_kin_x(ql, qr);


  float desiredY;

  if(currentY > topWall){
    desiredY = topWall;
    do_PID(pScale_L, dScale_L, iScale_L, pScale_R, dScale_R, iScale_R, currentX, desiredY - 1.3);
  }
  else if(currentY < bottomWall){
    desiredY = bottomWall;
    do_PID(pScale_L, dScale_L, iScale_L, pScale_R, dScale_R, iScale_R, currentX - 0.2, desiredY + 0.8);
  }
  else{
    off_motor_right();
    off_motor_left();
  }

  
  
}

void do_PID(float pScale_L,float dScale_L,float iScale_L, float pScale_R,float dScale_R, float iScale_R, float desiredX, float desiredY){
  L_reading = analogRead(L_pot_pin);
  R_reading = analogRead(R_pot_pin);
  ql = abs(calculate_ql(L_reading));
  qr = abs(calculate_qr(R_reading));

  
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
      /*char xBuffer[2];
      dtostrf(x,2,3,xBuffer);
      char yBuffer[2];
      dtostrf(y,2,3,yBuffer);
      Serial.write('B');
      Serial.write(xBuffer[0]);
      Serial.write(xBuffer[1]);
      Serial.write('E');
      Serial.write('C');
      Serial.write(yBuffer[0]);
      Serial.write(yBuffer[1]);
      Serial.write('F');*/
  
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
//=============================================convert string to int for incoming data=================================
int stringToInt()
{
    char charHolder[inputString.length()+1];
    inputString.toCharArray(charHolder,inputString.length()+1);
    inputString = "";
    int _recievedVal = atoi(charHolder);
    return _recievedVal;
}
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




