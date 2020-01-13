#include <TinyMPU6050.h>
#define TIME_1 50
MPU6050 mpu (Wire);
long angle_x, angle_y, angle_z, 
    offset_x, offset_y, offset_z;
unsigned long time_1 = 0;
int ledState = LOW, buzzerState = LOW, 
    redLed = 8, yellowLed = 9, buzzerPin = 10, greenLed=11;
void setup() {
  mpu.Initialize();
  //Initial values of Acc and Gyro(X,Y,Z)
  Serial.begin(9600);
  Serial.print("AccX Offset = ");
  Serial.println(mpu.GetAccXOffset());
  Serial.print("AccY Offset = ");
  Serial.println(mpu.GetAccYOffset());
  Serial.print("AccZ Offset = ");
  Serial.println(mpu.GetAccZOffset());
  Serial.print("GyroX Offset = ");
  Serial.println(mpu.GetGyroXOffset());
  Serial.print("GyroY Offset = ");
  Serial.println(mpu.GetGyroYOffset());
  Serial.print("GyroZ Offset = ");
  Serial.println(mpu.GetGyroZOffset());
  //Pin mode assignment
  pinMode(buzzerPin,OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  digitalWrite(redLed, ledState);
  digitalWrite(greenLed, ledState);
  digitalWrite(buzzerPin, ledState);
  digitalWrite(yellowLed, ledState);
  //Initial offset values assigned for future calculations
  for(int i=0; i<100;i++){
    mpu.Execute();
    offset_x = mpu.GetAngX();
    offset_y = mpu.GetAngY();
    offset_z = mpu.GetAngZ();
  }
  Serial.print("offset_x = ");
  Serial.print(offset_x);
  Serial.print("  /  offsetY = ");
  Serial.print(offset_y);
  Serial.print("  /  offsetZ = ");
  Serial.println(offset_z); 
}

void loop() {
  //Movement checking
  for(int i=0; i<100;i++){
    mpu.Execute();
    angle_x = mpu.GetAngX();
    angle_y = mpu.GetAngY();
    angle_z = mpu.GetAngZ();
  }
  //They prints every value that comes from MPU6050
  Serial.print("AngX = ");
  Serial.print(angle_x - offset_x);
  Serial.print("  /  AngY = ");
  Serial.print(angle_y - offset_y);
  Serial.print("  /  AngZ = ");
  Serial.println(angle_z - offset_z);
  //This condition checks movement then triggers LED and BUZZER
  if ( 5 < angle_x - offset_x || 
      -5 > angle_x - offset_x || 
      5 < angle_y - offset_y || 
      -5 > angle_y - offset_y ||
      5 < angle_z - offset_z || 
      -5 > angle_z - offset_z)
  { 
    if(millis() > time_1 + TIME_1){
      time_1 = millis();
      //There are three levels
      if (ledState == LOW) { ledState = HIGH; } else { ledState = LOW; }
      if (buzzerState == LOW) { buzzerState = HIGH; } else { buzzerState = LOW; }
	  //Red light and Buzzer
      if ( 30 < angle_x - offset_x || 
      -30 > angle_x - offset_x || 
      30 < angle_y - offset_y || 
      -30 > angle_y - offset_y ||
      30 < angle_z - offset_z || 
      -30 > angle_z - offset_z) { digitalWrite(redLed, ledState); digitalWrite(buzzerPin, buzzerState);}
      //Yellow light
      if ( 20 < angle_x - offset_x || 
      -20 > angle_x - offset_x || 
      20 < angle_y - offset_y || 
      -20 > angle_y - offset_y ||
      20 < angle_z - offset_z || 
      -20 > angle_z - offset_z) { digitalWrite(yellowLed, ledState); }
      //Green light
      if ( 5 < angle_x - offset_x || 
      -5 > angle_x - offset_x || 
      5 < angle_y - offset_y || 
      -5 > angle_y - offset_y ||
      5 < angle_z - offset_z || 
      -5 > angle_z - offset_z) { digitalWrite(greenLed, ledState); }
    }
	//After movement new offset values
    mpu.Execute();
    offset_x = mpu.GetAngX();
    offset_y = mpu.GetAngY();
    offset_z = mpu.GetAngZ();

  }
  //This 2 line of code checks LED or BUZZER stays HIGH or not, if it is HIGH then make it LOW
  //Sometimes LED or BUZZER can stay on HIGH after movement end
  if(ledState=HIGH){ ledState = LOW; digitalWrite(redLed, ledState); digitalWrite(yellowLed, ledState); digitalWrite(greenLed, ledState);}
  if(buzzerState=HIGH){ buzzerState = LOW; digitalWrite(buzzerPin, buzzerState); }
}
