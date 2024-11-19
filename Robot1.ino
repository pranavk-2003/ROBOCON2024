#include <IBusBM.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <ezButton.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN 150   // Minimum pulse length count (out of 4096)
#define SERVOMAX 600   // Maximum pulse length count (out of 4096)
#define SERVO_FREQ 50  // Analog servos run at ~50 Hz updates

int angleToPulse(int ang) {
  int pulse = map(ang, 0, 180, SERVOMIN, SERVOMAX);
  return pulse;
}
int updown;
int leftright;
int clkaclk;
int shoot = 100;
int pwmvalA;
int pwmvalAabs;
long pwmvalAupdown;
long pwmvalAleftright;
int pwmvalB;
int pwmvalBabs;
long pwmvalBupdown;
long pwmvalBleftright;
int pwmvalC;
int pwmvalCabs;
long pwmvalCupdown;
long pwmvalCleftright;
int pwmvalD;
int pwmvalDabs;
long pwmvalDupdown;
long pwmvalDleftright;
bool LarmUp = true;  // Changed to true
bool RarmUp = true;  // Changed to true

bool LarmButtonPrevState = false;
bool RarmButtonPrevState = false;

bool leftServoOpen = false;
bool rightServoOpen = false;

const int limitSwitchLeftArm = 48;
const int limitSwitchRightArm = 44;

ezButton limitSwitchLeft(limitSwitchLeftArm);
ezButton limitSwitchRight(limitSwitchRightArm);
ezButton limitSwitch(45);

const int dirA = 22;
const int pwmA = 2;
const int dirB = 26;
const int pwmB = 3;
const int dirC = 24;
const int pwmC = 4;
const int dirD = 28;
const int pwmD = 5;
const int dirs = 32;
const int pwms = 7;
const int dirArmL = 25;
const int dirArmR = 23;
const int pwmArmL = 8;
const int pwmArmR = 9;

IBusBM ibusRc;

HardwareSerial& ibusRcSerial = Serial1;
HardwareSerial& debugSerial = Serial;

void setup() {
  debugSerial.begin(9600);
  ibusRc.begin(ibusRcSerial);

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);

  LarmUp = true;  // Changed to true
  RarmUp = true;  // Changed to true

  Serial.begin(9600);
  pinMode(dirA, OUTPUT);
  pinMode(pwmA, OUTPUT);
  pinMode(dirB, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(dirC, OUTPUT);
  pinMode(pwmC, OUTPUT);
  pinMode(dirD, OUTPUT);
  pinMode(pwmD, OUTPUT);
  pinMode(dirs, OUTPUT);
  pinMode(pwms, OUTPUT);
  pinMode(dirArmL, OUTPUT);
  pinMode(pwmArmL, OUTPUT);
  pinMode(dirArmR, OUTPUT);
  pinMode(pwmArmR, OUTPUT);
  pwm.setPWM(14, 0, angleToPulse(8));
  limitSwitch.setDebounceTime(50);
  limitSwitchLeft.setDebounceTime(50);
  limitSwitchRight.setDebounceTime(50);
}

int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue) {
  uint16_t ch = ibusRc.readChannel(channelInput);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

bool redSwitch(byte channelInput, bool defaultValue) {
  int intDefaultValue = (defaultValue) ? 100 : 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

void loop() {
  limitSwitch.loop();
  limitSwitchLeft.loop();
  limitSwitchRight.loop();
  int state = limitSwitch.getState();
  if (state == LOW) {
    digitalWrite(dirs, HIGH);
    analogWrite(pwms, 45);
  } else {
    analogWrite(pwms, 0);
  }

  int updown = readChannel(1, -100, 100, 0);
  int leftright = readChannel(0, -100, 100, 0);
  clkaclk = readChannel(3, -255, 255, 0);
  int Lservo = readChannel(5, -100, 100, 0);
  int Rservo = readChannel(6, -100, 100, 0);
  int shoot = readChannel(4, -100, 100, 0);
  int Larm = readChannel(7, -100, 100, 0);
  int Rarm = readChannel(8, -100, 100, 0);
  pwmvalAupdown = 0;
  pwmvalAleftright = 0;
  pwmvalBupdown = 0;
  pwmvalBleftright = 0;
  pwmvalCupdown = 0;
  pwmvalCleftright = 0;
  pwmvalDupdown = 0;
  pwmvalDleftright = 0;

  if (updown > 0) {  //UP
    pwmvalAupdown = map(updown, 0, 100, 0, 255);
    pwmvalBupdown = map(updown, 0, 100, 0, 254);
    pwmvalCupdown = map(updown, 0, 100, 0, 255);
    pwmvalDupdown = map(updown, 0, 100, 0, 255);
  }
  if (updown < 0) {  //DOWN
    pwmvalAupdown = map(updown, 0, -100, 0, -255);
    pwmvalBupdown = map(updown, 0, -100, 0, -255);
    pwmvalCupdown = map(updown, 0, -100, 0, -255);
    pwmvalDupdown = map(updown, 0, -100, 0, -255);
  }
  if (leftright < 0) {  // Left
    pwmvalAleftright = map(leftright, 0, 100, 0, 255);
    pwmvalBleftright = map(leftright, 0, 100, 0, -255);
    pwmvalCleftright = map(leftright, 0, 100, 0, -255);
    pwmvalDleftright = map(leftright, 0, 100, 0, 255);
  }
  if (leftright > 0) {  // Right
    pwmvalAleftright = map(leftright, 0, -100, 0, -255);
    pwmvalBleftright = map(leftright, 0, -100, 0, 255);
    pwmvalCleftright = map(leftright, 0, -100, 0, 255);
    pwmvalDleftright = map(leftright, 0, -100, 0, -255);
  }
  pwmvalAabs = sqrt((sq(pwmvalAupdown)) + (sq(pwmvalAleftright)));
  pwmvalBabs = sqrt((sq(pwmvalBupdown)) + (sq(pwmvalBleftright)));
  pwmvalCabs = sqrt((sq(pwmvalCupdown)) + (sq(pwmvalCleftright)));
  pwmvalDabs = sqrt((sq(pwmvalDupdown)) + (sq(pwmvalDleftright)));
  pwmvalA = pwmvalAupdown + pwmvalAleftright;
  pwmvalB = pwmvalBupdown + pwmvalBleftright;
  pwmvalC = pwmvalCupdown + pwmvalCleftright;
  pwmvalD = pwmvalDupdown + pwmvalDleftright;
  if (pwmvalA > 0) {
    digitalWrite(dirA, LOW);
    analogWrite(pwmA, pwmvalAabs);
  }
  if (pwmvalA < 0) {
    digitalWrite(dirA, HIGH);
    analogWrite(pwmA, pwmvalAabs);
  }
  if (pwmvalA == 0) {
    digitalWrite(dirA, LOW);
    analogWrite(pwmA, 0);
  }

  if (pwmvalB > 0) {
    digitalWrite(dirB, HIGH);
    analogWrite(pwmB, pwmvalBabs);
  }
  if (pwmvalB < 0) {
    digitalWrite(dirB, LOW);
    analogWrite(pwmB, pwmvalBabs);
  }
  if (pwmvalB == 0) {
    digitalWrite(dirB, LOW);
    analogWrite(pwmB, 0);
  }

  if (pwmvalC > 0) {
    digitalWrite(dirC, HIGH);
    analogWrite(pwmC, pwmvalCabs);
  }
  if (pwmvalC < 0) {
    digitalWrite(dirC, LOW);
    analogWrite(pwmC, pwmvalCabs);
  }
  if (pwmvalC == 0) {
    digitalWrite(dirC, LOW);
    analogWrite(pwmC, 0);
  }
  if (pwmvalD > 0) {
    digitalWrite(dirD, HIGH);
    analogWrite(pwmD, pwmvalDabs);
  }
  if (pwmvalD < 0) {
    digitalWrite(dirD, LOW);
    analogWrite(pwmD, pwmvalDabs);
  }
  if (pwmvalD == 0) {
    digitalWrite(dirD, LOW);
    analogWrite(pwmD, 0);
  }

  if (clkaclk > 0) {
    digitalWrite(dirA, LOW);
    digitalWrite(dirB, HIGH);
    digitalWrite(dirC, LOW);
    digitalWrite(dirD, LOW);
    analogWrite(pwmA, clkaclk);
    analogWrite(pwmB, clkaclk);
    analogWrite(pwmC, clkaclk);
    analogWrite(pwmD, clkaclk);
  }
  if (clkaclk < 0) {
    digitalWrite(dirA, HIGH);
    digitalWrite(dirB, LOW);
    digitalWrite(dirC, HIGH);
    digitalWrite(dirD, HIGH);
    analogWrite(pwmA, -clkaclk);
    analogWrite(pwmB, -clkaclk);
    analogWrite(pwmC, -clkaclk);
    analogWrite(pwmD, -clkaclk);
  }

  if (Lservo > 0) {
    pwm.setPWM(4, 0, angleToPulse(40));
    pwm.setPWM(5, 0, angleToPulse(40));
    leftServoOpen = false;
  }

  if (Lservo < 0) {
    pwm.setPWM(4, 0, angleToPulse(0));
    pwm.setPWM(5, 0, angleToPulse(0));
    leftServoOpen = true;
  }
  if (Rservo < 0) {
    pwm.setPWM(1, 0, angleToPulse(40));
    pwm.setPWM(2, 0, angleToPulse(40));
    rightServoOpen = false;
  }

  if (Rservo > 0) {
    pwm.setPWM(1, 0, angleToPulse(0));
    pwm.setPWM(2, 0, angleToPulse(0));
    rightServoOpen = true;
  }
  if (shoot > 0) {
    pwm.setPWM(14, 0, angleToPulse(8));
  }
  if (shoot == 0) {
    pwm.setPWM(14, 0, angleToPulse(100));
  }
  if (shoot < 0) {
    digitalWrite(dirs, LOW);
    analogWrite(pwms, 255);
    delay(400);
    analogWrite(pwms, 0);
    delay(800);
  }
  bool LarmButtonCurrentState = (Larm > 0);
  if (LarmButtonCurrentState && !LarmButtonPrevState) {
    if (LarmUp) {
      moveArmDown(true);
      LarmUp = false;
    } else {
      moveArmUp(true);
      LarmUp = true;
    }
  }
  LarmButtonPrevState = LarmButtonCurrentState;

  bool RarmButtonCurrentState = (Rarm > 0);
  if (RarmButtonCurrentState && !RarmButtonPrevState) {
    if (RarmUp) {
      moveArmDown(false);
      RarmUp = false;
    } else {
      moveArmUp(false);
      RarmUp = true;
    }
  }
  RarmButtonPrevState = RarmButtonCurrentState;
}

void moveArmUp(bool isLeftArm) {
  if (isLeftArm) {
    digitalWrite(dirArmL, HIGH);
    debugSerial.println("Lup");
    analogWrite(pwmArmL, 255);
    delay(1500);
    analogWrite(pwmArmL, 0);
  } else {
    digitalWrite(dirArmR, HIGH);
    debugSerial.println("Rup");
    analogWrite(pwmArmR, 255);
    delay(1000);
    analogWrite(pwmArmR, 0);
  }
}

void moveArmDown(bool isLeftArm) {
  if (isLeftArm) {
    digitalWrite(dirArmL, LOW);
    debugSerial.println("Ldown");
    analogWrite(pwmArmL, 255);
    if (leftServoOpen) {
      while (limitSwitchLeft.getState() == HIGH) {
        limitSwitchLeft.loop();
        delay(10);
      }
    } else {
      delay(2000);
    }
    analogWrite(pwmArmL, 0);
  } else {
    digitalWrite(dirArmR, LOW);
    debugSerial.println("Rdown");
    analogWrite(pwmArmR, 255);
    if (rightServoOpen) {
      while (limitSwitchRight.getState() == HIGH) {
        limitSwitchRight.loop();
        delay(10);
      }
    } else {
      delay(500);
    }
    analogWrite(pwmArmR, 0);
  }
}
