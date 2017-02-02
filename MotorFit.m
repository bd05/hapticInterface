Vel_Data = iddata(angularVelocity, time, 0.04);
Motor = tfest(Vel_Data, 2,0); %not including Hbridge
H_Bridge = 2.844;
%step(Motor);
rlocus(H_Bridge * Motor);
