  
#include <Servo.h>

int photoSensor = 2;
int photoValue= 0;
Servo servoM1;
int servoPos = 0;
int srvAngle = 3;


void setup() {
  Serial.begin(9600);
  servoM1.attach(9);
  servoM1.write(servoPos);
}

void loop() {
  photoValue = analogRead(photoSensor);
  int stepperSensor = analogRead(A1);

  Serial.println(photoValue);
  if (photoValue > 375){
    Serial.println(photoValue);
    //delay(325);
    delay(460);
    for (servoPos = 0; servoPos <= srvAngle; servoPos += 1) { 
      servoM1.write(servoPos);              
      delay(1);                       
    }               
    
    for (servoPos = 90; servoPos >= 0; servoPos -= 1) {
      servoM1.write(servoPos);
      delay(1);
    }
   } 
}
