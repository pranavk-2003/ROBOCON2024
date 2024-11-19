#include <Servo.h>
int updown;
int leftright;
int clkaclk;
int servo;
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
int pwmvalclkaclk = 150;
int maxval = 300;
int minval = 1;
float factorac = 0.6;
float factorb = 0.7;
int pwmshoot1 = 100;
int pwmshoot2 = 255;
bool disable1 = false;
bool Larm = true;  // Initial state is Lup for left arm
bool Rarm = true;  // Initial state is Rup for right arm
bool a = true;
bool b = false;
bool c = true;
bool d = false;
bool LarmConditionExecuted = false;  // Flag for left arm condition
bool RarmConditionExecuted = false;  // Flag for right arm condition




int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue) {
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}
bool readSwitch(byte channelInput, bool defaultValue) {
  int intDefaultValue = (defaultValue) ? 100 : 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

#define ch1 15
#define ch2 14
#define ch4 16
#define ch5 17
#define ch6 36
#define ch7 34
#define ch8 42
#define ch9 44
const int dirA = 22;
const int pwmA = 6;
const int dirB = 23;
const int pwmB = 7;
const int dirC = 25;
const int pwmC = 8;
const int dirD = 27;
const int pwmD = 9;
const int dirs = 28;
const int pwms = 10;
const int Lswitch = 51;
const int servop = 29;
  const int servoGripperL = 46;
const int servoGripperR = 45;
const int dirArmL =2;
const int dirArmR =4;
const int pwmArmL = 3;
const int pwmArmR = 5;


Servo Servo1;
Servo Servo2;
Servo Servo3;

void setup() {
  Serial.begin(9600);
  pinMode(dirA, OUTPUT);
  pinMode(pwmA, OUTPUT);
  pinMode(ch1, INPUT);
  pinMode(ch2, INPUT);
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
  Servo1.attach(servop);
  Servo2.attach(servoGripperR);
  Servo3.attach(servoGripperL);
  Servo1.write(75);
  // Servo2.write(180);
  // Servo3.write(180);
}

void loop() {
  if (digitalRead(Lswitch) == 0) {
    digitalWrite(dirs, LOW);
    analogWrite(pwms, 60);
  } else {
    analogWrite(pwms, 0);
  }


  updown = pulseIn(ch1, HIGH);
  leftright = pulseIn(ch2, HIGH);
  clkaclk = readChannel(ch4, -255, 255, 0);
  int ch5Value = readChannel(ch5, -100, 100, 0);
  int ch6Value = readChannel(ch6, -100, 100, 100);
  int ch7Value = readChannel(ch7, -100, 100, 100);
  bool ch8Value = readSwitch(ch9, false);
  bool ch9Value = readSwitch(ch8, false);
  pwmvalAupdown = 0;
  pwmvalAleftright = 0;
  pwmvalBupdown = 0;
  pwmvalBleftright = 0;
  pwmvalCupdown = 0;
  pwmvalCleftright = 0;
  pwmvalDupdown = 0;
  pwmvalDleftright = 0;
  pwmvalclkaclk = 0;
  // Serial.println(updown);
  // Serial.println(leftright);
  Serial.println(clkaclk);

  if (updown > 1500) {  //UP
    pwmvalAupdown = map(updown, 1490, 1920, -minval * factorac, -maxval * factorac);
    pwmvalBupdown = map(updown, 1490, 1920, minval * factorac, maxval * factorac);
    pwmvalCupdown = map(updown, 1490, 1920, minval * factorac, maxval * factorac);
    pwmvalDupdown = map(updown, 1490, 1920, minval * factorac, maxval * factorac);
  }
  if (updown < 1450) {  //DOWN
    pwmvalAupdown = map(updown, 1470, 1920, -minval * factorac, -maxval * factorac);
    pwmvalBupdown = map(updown, 1470, 1920, minval * factorac, maxval * factorac);
    pwmvalCupdown = map(updown, 1490, 1920, minval * factorac, maxval * factorac);
    pwmvalDupdown = map(updown, 1490, 1920, minval * factorac, maxval * factorac);
  }
  if (leftright < 1450) {  // Left
    pwmvalAleftright = map(leftright, 1470, 1920, -minval * factorac, -maxval * factorac);
    pwmvalBleftright = map(leftright, 1470, 1920, -minval * factorb, -maxval * factorb);
    pwmvalCleftright = map(leftright, 1470, 1920, -minval * factorac, -maxval * factorac);
    pwmvalDleftright = map(leftright, 1470, 1920, minval * factorb, maxval * factorb);
  }
  if (leftright > 1500) {  // Right
    pwmvalAleftright = map(leftright, 1490, 1920, -minval * factorac, -maxval * factorac);
    pwmvalBleftright = map(leftright, 1490, 1920, -minval * factorb, -maxval * factorb);
    pwmvalCleftright = map(leftright, 1490, 1920, -minval * factorac, -maxval * factorac);
    pwmvalDleftright = map(leftright, 1490, 1920, minval * factorb, maxval * factorb);
  }
  pwmvalAabs = sqrt((sq(pwmvalAupdown)) + (sq(pwmvalAleftright)));
  pwmvalBabs = sqrt((sq(pwmvalBupdown)) + (sq(pwmvalBleftright)));
  pwmvalCabs = sqrt((sq(pwmvalCupdown)) + (sq(pwmvalCleftright)));
  pwmvalDabs = sqrt((sq(pwmvalDupdown)) + (sq(pwmvalDleftright)));
  pwmvalA = pwmvalAupdown + pwmvalAleftright;
  pwmvalB = pwmvalBupdown + pwmvalBleftright;
  pwmvalC = pwmvalCupdown + pwmvalCleftright;
  pwmvalD = pwmvalDupdown + pwmvalDleftright;
  if (ch8Value == 1) {
    Larm = !Larm;                   // Toggle the state for left arm
    LarmConditionExecuted = false;  // Reset the flag for left arm condition
    delay(500);
  }

  if (ch9Value == 1) {
    Rarm = !Rarm;                   // Toggle the state for right arm
    RarmConditionExecuted = false;  // Reset the flag for right arm condition
    delay(500);
  }

  if (Larm && !LarmConditionExecuted) {
    Serial.println("Lup");
    digitalWrite(dirArmL, HIGH);
    analogWrite(pwmArmL, 255);
    delay(2100);
    analogWrite(pwmArmL, 0);
    b = true;
    a = false;
    LarmConditionExecuted = true;  // Set the flag for left arm condition
  } else if (!Larm && !LarmConditionExecuted) {
    Serial.println("Ldown");
    digitalWrite(dirArmL, LOW);
    analogWrite(pwmArmL, 255);
    delay(2100);
    analogWrite(pwmArmL, 0);
    a = true;
    b = false;
    LarmConditionExecuted = true;  // Set the flag for left arm condition
  }
// Serial.println(ch6Value);
  if (Rarm && !RarmConditionExecuted) {
    Serial.println("Rup");
    digitalWrite(dirArmR, HIGH);
    analogWrite(pwmArmR, 170);
    delay(900);
    analogWrite(pwmArmR, 0);
    d = true;
    c = false;
    RarmConditionExecuted = true;  // Set the flag for right arm condition
  } else if (!Rarm && !RarmConditionExecuted) {
    Serial.println("Rdown");
    digitalWrite(dirArmR, LOW);
    analogWrite(pwmArmR, 170);
    delay(900);
    analogWrite(pwmArmR, 0);
    c = true;
    d = false;
    RarmConditionExecuted = true;  // Set the flag for right arm condition
  }

  if (ch7Value > 0) {
    Servo2.write(0);
  }
  if (ch7Value < 0) {
    Servo2.write(180);
  }
  if (ch6Value > 0) {
    Servo3.write(180);
  }
  if (ch6Value < 0) {
    Servo3.write(0);
  }
  if (ch5Value > 0) {
    Servo1.write(75);
  }
  if (ch8Value == 1) {
    Rarm = !Rarm;
    delay(100);
  }
  if (ch9Value == 1) {
    Larm = !Larm;
    delay(100);
  }

  if (ch5Value < 0 && ch5Value > -10) {
    Servo1.write(0);
    disable1 = true;
  }
  if (disable1) {
    if (ch5Value < -100) {
      digitalWrite(dirs, HIGH);
      analogWrite(pwms, 255);
      delay(750);
      analogWrite(pwms, 0);
      disable1 = false;
    }
  }

  if (pwmvalA > 20) {
    digitalWrite(dirA, HIGH);
    analogWrite(pwmA, pwmvalAabs);
  }
  if (pwmvalA < 20) {
    digitalWrite(dirA, LOW);
    analogWrite(pwmA, pwmvalAabs);
  }
  if (pwmvalA == 0) {
    digitalWrite(dirA, LOW);
    analogWrite(pwmA, 0);
  }

  if (pwmvalB > 20) {
    digitalWrite(dirB, HIGH);
    analogWrite(pwmB, pwmvalBabs);
  }
  if (pwmvalB < 20) {
    digitalWrite(dirB, LOW);
    analogWrite(pwmB, pwmvalBabs);
  }
  if (pwmvalB == 0) {
    digitalWrite(dirB, LOW);
    analogWrite(pwmB, 0);
  }

  if (pwmvalC > 20) {
    digitalWrite(dirC, HIGH);
    analogWrite(pwmC, pwmvalCabs);
  }
  if (pwmvalC < 20) {
    digitalWrite(dirC, LOW);
    analogWrite(pwmC, pwmvalCabs);
  }
  if (pwmvalC == 0) {
    digitalWrite(dirC, LOW);
    analogWrite(pwmC, 0);
  }
  if (pwmvalD > 20) {
    digitalWrite(dirD, HIGH);
    analogWrite(pwmD, pwmvalDabs);
  }
  if (pwmvalD < 20) {
    digitalWrite(dirD, LOW);
    analogWrite(pwmD, pwmvalDabs);
  }
  if (pwmvalD == 0) {
    digitalWrite(dirD, LOW);
    analogWrite(pwmD, 0);
  }
  if (clkaclk > 50 && clkaclk < 255) {
    digitalWrite(dirA, LOW);
    digitalWrite(dirB, LOW);
    digitalWrite(dirC, HIGH);
    digitalWrite(dirD, LOW);
    analogWrite(pwmA, clkaclk);
    analogWrite(pwmB, clkaclk);
    analogWrite(pwmC, clkaclk);
    analogWrite(pwmD, clkaclk);
  }
  if (clkaclk < -50 && clkaclk > -255) {
    digitalWrite(dirA, HIGH);
    digitalWrite(dirB, HIGH);
    digitalWrite(dirC, LOW);
    digitalWrite(dirD, HIGH);
    analogWrite(pwmA, -clkaclk);
    analogWrite(pwmB, -clkaclk);
    analogWrite(pwmC, -clkaclk);
    analogWrite(pwmD, -clkaclk);
  }
}