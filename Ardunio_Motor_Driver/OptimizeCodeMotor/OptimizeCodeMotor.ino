#include <AFMotor.h>
#include <NewPing.h>
#include <Servo.h> 


// Direction
#define LEFT 0x4
#define RIGHT 0x8

// WHEEL Position 
#define WHEEL_FRONT_RIGHT 0x1
#define WHEEL_FRONT_LEFT 0x2
#define WHEEL_BACK_LEFT 0x4
#define WHEEL_BACK_RIGHT 0x8
#define WHEELS (WHEEL_FRONT_RIGHT | WHEEL_FRONT_LEFT | WHEEL_BACK_LEFT | WHEEL_BACK_RIGHT)

// Servo Settings
#define SERVO_PIN 10
#define SERVO_STRAIGHT_ANGLE 90
#define SERVO_LEFT_ANGLE 50
#define SERVO_RIGHT_ANGLE 130
#define SERVO_NEAK_DELAY 500

// Motor Variables
#define FRONT_LEFT_MOTOR 4
#define FRONT_RIGHT_MOTOR 3
#define BACK_LEFT_MOTOR 1
#define BACK_RIGHT_MOTOR 2
#define MAX_MOTOR_SPEED 200

//altraSonic Sensor
#define TRIGGER_PIN  A4  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     A5  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define DISTANCE_LIMIT 30 // Max distance in cm
#define MAX_DISTANCE 200

//
#define LOOP_DELAY 500

// Globals
AF_DCMotor frontLeft(FRONT_LEFT_MOTOR);
AF_DCMotor frontRight(FRONT_RIGHT_MOTOR);
AF_DCMotor backLeft(BACK_LEFT_MOTOR);
AF_DCMotor backRight(BACK_RIGHT_MOTOR);

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
Servo servo;
int LAST_PATH = 0;
int repeat_counter = 0;


void setup() {
  Serial.begin(9600);
  servo.attach (SERVO_PIN);
  set_motor_speed(MAX_MOTOR_SPEED, WHEELS);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  int path = search_path();
  Serial.println(path);
  if (LAST_PATH != path){
    repeat_counter = 0;
    if (path == FORWARD){
      set_motor_action(WHEELS, FORWARD);
      //LAST_PATH = FORWARD;
    } else if (path == LEFT){
      turn_vehicle(LEFT);
      //set_motor_action(WHEELS, FORWARD);  
    } else if(path == RIGHT){
      turn_vehicle(RIGHT);
      //set_motor_action(WHEELS, FORWARD); 
    } 
  } else {
    repeat_counter++;
  }

  if (repeat_counter == 3){
    if (path != FORWARD){
      Serial.println("Stuck in a loop:");
      Serial.print(path);
      set_motor_action(WHEELS, BACKWARD);
    }
  }

  LAST_PATH = path;
  
  //int distance = get_distance(RIGHT);
  //Serial.println(distance);
  delay(LOOP_DELAY);
}

/*------------------ Setup Motor Speed ----------------//

------------------------------------------------------*/
void set_motor_speed(int speed, int wheels){

  if (wheels & WHEEL_FRONT_RIGHT){
    frontRight.setSpeed(speed);
  } else if (wheels & WHEEL_FRONT_LEFT)
  {
    frontLeft.setSpeed(speed);
  } else if (wheels & WHEEL_BACK_LEFT)
  {
    backLeft.setSpeed(speed);
  } else if (wheels & WHEEL_BACK_RIGHT)
  {
    backRight.setSpeed(speed);
  }
}

/*-------------------- Motor Action -------------- //
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4
--------------------------------------------------*/
void set_motor_action(int wheels, int action){
  if (wheels & WHEEL_FRONT_RIGHT){
    frontRight.run(action);
  } else if (wheels & WHEEL_FRONT_LEFT)
  {
    frontLeft.run(action);
  } else if (wheels & WHEEL_BACK_LEFT)
  {
    backLeft.run(action);
  } else if (wheels & WHEEL_BACK_RIGHT)
  {
    backRight.run(action);
  }
}

/* -------------------- Turn Vehicle ---------------- /

---------------------------------------------------- */
void turn_vehicle(int direction){
  set_motor_action(WHEELS, RELEASE);
  if (direction == LEFT){
    set_motor_action(WHEEL_FRONT_LEFT | WHEEL_BACK_LEFT,BACKWARD);        
    set_motor_action(WHEEL_FRONT_RIGHT | WHEEL_BACK_RIGHT,FORWARD);
  } else if (direction == RIGHT)
  {
    set_motor_action(WHEEL_FRONT_LEFT | WHEEL_BACK_LEFT, FORWARD);        
    set_motor_action(WHEEL_FRONT_RIGHT | WHEEL_BACK_RIGHT, BACKWARD);
  }
  delay(400);
  set_motor_action(WHEELS, RELEASE);
}

/*---------------------- Search Path --------------------//

-------------------------------------------------------- */
int search_path(){

  int distance = get_distance(FORWARD);
  Serial.println("Forward Distance");
  Serial.print(distance);

  if (distance > DISTANCE_LIMIT){
    Serial.println("Return FORWARD");
    return FORWARD;
  }

  int left_distance = get_distance(LEFT);
  int right_distance = get_distance(RIGHT);

  Serial.print("LEFT Distance:");
  Serial.println(left_distance);
  Serial.print("RIGHT Distance");
  Serial.println(right_distance);

  if ((left_distance < DISTANCE_LIMIT) & (right_distance < DISTANCE_LIMIT) ){
    int back_distance = get_distance(BACKWARD);
    if (back_distance > DISTANCE_LIMIT){
      return FORWARD;
    } 
  } else if (left_distance > right_distance)
  {
    return RIGHT;
  } else if (left_distance < right_distance)
  {
    return LEFT;
  }
  
  set_motor_action(WHEELS, RELEASE);    
  
  return 0;
  
}

/*----------------------- get Distance in cm -------------- //

---------------------------------------------------------- */
int get_distance(int direction){
  
  if (direction == FORWARD){
    set_servo_angle(SERVO_STRAIGHT_ANGLE);
  } else if (direction == RIGHT)
  {
    set_servo_angle(SERVO_RIGHT_ANGLE);
  } else if (direction == LEFT)
  {
    set_servo_angle(SERVO_LEFT_ANGLE);
  } else if (direction == BACKWARD)
  {
    turn_vehicle(LEFT);
    turn_vehicle(LEFT); 
    set_servo_angle(SERVO_STRAIGHT_ANGLE);
  }

  int distance = get_sonar_distance();
  Serial.print("Sonar distance");
  Serial.println(distance);
  return distance;
}

/*------------------------ Rotate Servo -----------------------\\

---------------------------------------------------------------- */
void set_servo_angle(int angle){
  servo.write(angle);
  //delay(SERVO_NEAK_DELAY);
}

/*------------------------- Get Sonar Distance ---------------- \\

-------------------------------------------------------------- */
int get_sonar_distance(){
  //delay(400);
  int distance = sonar.ping_cm();
  if (distance == 0){
    distance = MAX_DISTANCE; 
   }
  //delay(400);
  Serial.println(distance);
  return distance;
}
