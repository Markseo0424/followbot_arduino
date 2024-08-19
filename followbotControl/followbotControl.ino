#include <ros.h>
#include <geometry_msgs/Twist.h>

#define EN1 6
#define IN1 8
#define IN2 9
#define IN3 3
#define IN4 4
#define EN2 5

ros::NodeHandle n;

unsigned long lastPressed;

void MoveMotor(float rSpeed, float lSpeed){
  if(rSpeed > 1) rSpeed = 1;
  if(rSpeed < -1) rSpeed = -1;
  if(lSpeed > 1) lSpeed = 1;
  if(lSpeed < -1) lSpeed = -1;
  
  if(rSpeed > 0){
    analogWrite(EN1, int(rSpeed * 255));
    
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
  else if(rSpeed < 0){
    analogWrite(EN1, int(-rSpeed * 255));
    
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
  else {
    digitalWrite(EN1, LOW);
  }

  if(lSpeed > 0){
    analogWrite(EN2, int(lSpeed * 255));
    
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  else if(lSpeed < 0){
    analogWrite(EN2, int(-lSpeed * 255));
    
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  else {
    digitalWrite(EN2, LOW);
  }
  
}

void Callback(const geometry_msgs::Twist& cmd_vel){
  MoveMotor(cmd_vel.linear.x * 2, cmd_vel.linear.y * 2);
  lastPressed = millis();
  
//  if(cmd_vel.angular.z > 0){
//    MoveMotor(1, -1);
//  }
//  
//  else if(cmd_vel.angular.z < 0) {
//    MoveMotor(-1, 1);
//  }
//
//  else if(cmd_vel.linear.x > 0) {
//    MoveMotor(1, 1);
//  }
//
//  else if(cmd_vel.linear.x < 0) {
//    MoveMotor(-1, -1);
//  }
//
//  else {
//    MoveMotor(0,0);
//  }
}

ros::Subscriber<geometry_msgs::Twist> sub("followbot/cv_cmd", Callback);

void setup() {
  n.initNode();
  n.subscribe(sub);

  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  n.spinOnce();
  delay(1);

  if(millis() - lastPressed > 100){
    MoveMotor(0, 0);
  }
}
