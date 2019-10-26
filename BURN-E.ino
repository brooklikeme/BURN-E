#include <Servo.h>

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

#define STOP      0
#define FORWARD   1
#define BACKWARD  2
#define TURNLEFT  3
#define TURNRIGHT 4
#define SPINLEFT  5
#define SPINRIGHT 6

#define HEAD_SERVO_PIN 2
#define LEFT_ARM_SERVO_PIN 3
#define RIGHT_ARM_SERVO_PIN 4
#define WAIST_SERVO_PIN 5
#define LEFT_WHEEL_SERVO_PIN 6
#define RIGHT_WHEEL_SERVO_PIN 7

#define TOP_LEFT_EYE_LED_PIN A6
#define TOP_RIGHT_EYE_LED_PIN A4
#define BOTTOM_LEFT_EYE_LED_PIN A5 
#define BOTTOM_RIGHT_EYE_LED_PIN A7

#define BUZZER_PIN 12

#define FRONT_LEFT_IR_PIN 8
#define FRONT_RIGHT_IR_PIN 9
#define LEFT_IR_PIN 10
#define RIGHT_IR_PIN 11

Servo headServo;
Servo leftArmServo;
Servo rightArmServo;
Servo waistServo;
Servo leftWheelServo;
Servo rightWheelServo;

// notes in the melody:
int melody[] = {
  NOTE_A7, NOTE_B7, NOTE_C7, NOTE_D7
};

int noteDurations[] = {
  200, 200, 200, 200
};

bool frontLeftIR = false;
bool frontRightIR = false;
bool leftIR = false;
bool rightIR = false;

float leftScale = 1.0;
float rightScale = 0.9;
float turnScale = 0.1;

int trackingStatus = 0; // 0: none, 1: tracking ok, 2: find 
int motionStatus = 0; 

void setup() {
    //串口初始化
  Serial.begin(9600); 
  
  pinMode(FRONT_LEFT_IR_PIN, INPUT);
  pinMode(FRONT_RIGHT_IR_PIN, INPUT);
  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT); 
  pinMode(BUZZER_PIN, OUTPUT);
  // init servos
  headServo.attach(HEAD_SERVO_PIN);
  leftArmServo.attach(LEFT_ARM_SERVO_PIN);
  rightArmServo.attach(RIGHT_ARM_SERVO_PIN);
  waistServo.attach(WAIST_SERVO_PIN);
  leftWheelServo.attach(LEFT_WHEEL_SERVO_PIN);
  rightWheelServo.attach(RIGHT_WHEEL_SERVO_PIN);

  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 4; thisNote++) {

    tone(BUZZER_PIN, melody[thisNote], noteDurations[thisNote]);
    delay(noteDurations[thisNote] * 1.30);
    noTone(BUZZER_PIN);
  }
}

void speak(int what) {
  return;
}

void action(int what) {
  
}

void getIRStatus() {
  frontLeftIR = digitalRead(FRONT_LEFT_IR_PIN);
  frontRightIR = digitalRead(FRONT_RIGHT_IR_PIN);
  leftIR = digitalRead(LEFT_IR_PIN);
  rightIR = digitalRead(RIGHT_IR_PIN); 
}

void servoRun(int cmd,int speedValue) // speed from 0 - 100
{
  int realSpeed = 90;
  switch(cmd){
    case FORWARD:
      Serial.println("FORWARD"); //输出状态
      realSpeed = map(speedValue * leftScale, 0, 100, 91, 180);
      leftWheelServo.write(realSpeed);
      realSpeed = map(speedValue * rightScale, 0, 100, 90, 0);
      rightWheelServo.write(realSpeed);
      delay(10);
      break;
     case BACKWARD:
      Serial.println("BACKWARD"); //输出状态
      realSpeed = map(speedValue * leftScale, 0, 100, 90, 0);
      leftWheelServo.write(realSpeed);
      realSpeed = map(speedValue * rightScale, 0, 100, 91, 180);
      rightWheelServo.write(realSpeed);
      delay(10);
      break;
     case TURNLEFT:
      Serial.println("TURN  LEFT"); //输出状态
      realSpeed = map(speedValue * leftScale * turnScale, 0, 100, 91, 180);      
      leftWheelServo.write(realSpeed);
      realSpeed = map(speedValue * rightScale, 0, 100, 90, 0);   
      rightWheelServo.write(realSpeed);
      //delay(15);
      break;
     case TURNRIGHT:
      Serial.println("TURN  RIGHT"); //输出状态
      realSpeed = map(speedValue * leftScale, 0, 100, 91, 180);
      leftWheelServo.write(realSpeed);
      realSpeed = map(speedValue * rightScale * turnScale, 0, 100, 90, 0);
      rightWheelServo.write(realSpeed);
      delay(10);
      break;      
     case SPINLEFT:
      Serial.println("SPIN  LEFT"); //输出状态
      realSpeed = map(speedValue * leftScale, 0, 100, 90, 0);      
      leftWheelServo.write(realSpeed);
      realSpeed = map(speedValue * rightScale, 0, 100, 90, 0);   
      rightWheelServo.write(realSpeed);
      delay(10);
      break;
     case SPINRIGHT:
      Serial.println("SPIN  RIGHT"); //输出状态
      realSpeed = map(speedValue * leftScale, 0, 100, 91, 180);
      leftWheelServo.write(realSpeed);
      realSpeed = map(speedValue * rightScale, 0, 100, 91, 180);
      rightWheelServo.write(realSpeed);
      delay(10);
      break;
     default:
      Serial.println("STOP"); //输出状态
      leftWheelServo.write(90);
      rightWheelServo.write(90);
      delay(10);
  }
}

void tracingLine() {
  getIRStatus();

  if(!frontLeftIR && !frontRightIR)  
  {
    motionStatus = FORWARD;
    servoRun(FORWARD, 40);
  } else if (!frontLeftIR && frontRightIR) {
    motionStatus = TURNRIGHT;
    servoRun(TURNRIGHT, 20);
  } else if (frontLeftIR && !frontRightIR) {
    motionStatus = TURNLEFT;
    servoRun(TURNLEFT, 20);
  } else if (frontLeftIR && frontRightIR) {
    // check motion status
    if (motionStatus == TURNLEFT || motionStatus == TURNRIGHT) {
      ;
    } else {
      motionStatus = STOP;
      servoRun(STOP, 20);
    }
  }
  
  Serial.print(frontLeftIR);
  Serial.print("---");
  Serial.print(frontRightIR);
  Serial.print("---");
  Serial.print(leftIR);
  Serial.print("---");
  Serial.print(rightIR);
  Serial.print("---");
  Serial.println(motionStatus);
}


void loop() {
  // put your main code here, to run repeatedly:
  tracingLine();

  delay(50);

}
