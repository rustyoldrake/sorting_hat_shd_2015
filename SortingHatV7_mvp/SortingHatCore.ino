#include "Mouth.h"
Mouth mouth;

#include "Servo.h"

//Eyebrow Variables
Servo leftEyebrow; // lower=?, middle=?, upper=?
Servo rightEyebrow; // lower=?, middle=?, upper=?
Servo middleEyebrow; // lower=?, middle=?, upper=?
unsigned long leftEyebrowTimeout = 0;
unsigned long rightEyebrowTimeout = 0;
unsigned long middleEyebrowTimeout = 0;
unsigned long leftEyebrowStartTimeout = 0;
unsigned long rightEyebrowStartTimeout = 0;
unsigned long middleEyebrowStartTimeout = 0;
int leftEyebrowTarget = 90;
int rightEyebrowTarget = 90;
int middleEyebrowTarget = 90;
int leftEyebrowDefault = 90;
int rightEyebrowDefault = 90;
int middleEyebrowDefault = 90;
bool leftEyebrowDone = true;
bool rightEyebrowDone = true;
bool middleEyebrowDone = true;

//Eyes Variables
int redPin;
int greenPin;
int yellowPin;
int whitePin;

unsigned long redEyeTimeout = 0;
unsigned long greenEyeTimeout = 0;
unsigned long yellowEyeTimeout = 0;
unsigned long whiteEyeTimeout = 0;
unsigned long redEyeStartTimeout = 0;
unsigned long greenEyeStartTimeout = 0;
unsigned long yellowEyeStartTimeout = 0;
unsigned long whiteEyeStartTimeout = 0;

bool redEyeDone = true;
bool greenEyeDone = true;
bool yellowEyeDone = true;
bool whiteEyeDone = true;

unsigned long lastBlinkUpdate = 0;
bool greenEyesBlinking = false;
bool whiteEyesBlinking = false;

//Core Logic
void sort_setup() {
  Serial.begin(38400);
  mouth.setup();
  eyes_setup(11, 4, 10, 12);
  eyebrows_setup(A0, A1, A2);
  eyebrows_setLeftDefault(110);
  eyebrows_setRightDefault(130);
}
 
void sort_loop() {
  mouth.update();
  eyebrows_update();
  eyes_update();
}

void question(){
  mouth.talk(11300);
  eye_whiteFor(11300);
  eyebrows_setLeftFor(80, 4000);
  eyebrows_setRightFor(160, 4000);
  eyebrows_setLeftForAfter(120, 4000, 5000);
  eyebrows_setRightForAfter(110, 4500, 5500);
}

void thinking(){
  eyebrows_setLeftFor(140, 2000);
  eyebrows_setRightFor(100, 4000);
  eyebrows_setLeftForAfter(80, 3000, 3000);
  eyebrows_setRightForAfter(150, 1500, 4500);
  eyeBlink_greenFor(5000);
  eye_whiteFor(7000);
}

void gryffindor(){
  mouth.talk(6400);
  eyebrows_setLeftFor(120, 5000);
  eyebrows_setRightFor(110, 6000);
  eye_greenFor(6400);
}

void slytherin(){
  mouth.talk(7500);
  eyebrows_setLeftFor(80, 5000);
  eyebrows_setRightFor(160, 5000);
  eye_redFor(7500);
}

void ravenclaw(){
  mouth.talk(3600);
  eyebrows_setLeftFor(140, 3600);
  eyebrows_setRightFor(100, 3600);
  eye_whiteFor(3600);
}

void hufflepuff(){
  mouth.talk(6600);
  eyebrows_setLeftFor(140, 6600);
  eyebrows_setRightFor(100, 6600);
  eye_greenFor(6600);
  eyeBlink_whiteFor(6600);
}

//void stopExpressions(){
//  mouth.stopTalking();
//}

//Eyebrow Functions
void eyebrows_setup(uint8_t leftBrowPin, uint8_t rightBrowPin, uint8_t middleBrowPin) {
  leftEyebrow.attach(leftBrowPin);
  rightEyebrow.attach(rightBrowPin);
  middleEyebrow.attach(middleBrowPin);
}

void eyebrows_update() {
  unsigned long now = millis();

  if (leftEyebrowStartTimeout != 0 && now > leftEyebrowStartTimeout) {
    leftEyebrow.write(leftEyebrowTarget);
    leftEyebrowStartTimeout = 0;
  }
  if (leftEyebrowTimeout != 0 && now > leftEyebrowTimeout) {
    eyebrows_setLeft(leftEyebrowDefault);
    leftEyebrowTimeout = 0;
    leftEyebrowDone = true;
  }

  if (rightEyebrowStartTimeout != 0 && now > rightEyebrowStartTimeout) {
    rightEyebrow.write(rightEyebrowTarget);
    rightEyebrowStartTimeout = 0;
  }
  if (rightEyebrowTimeout != 0 && now > rightEyebrowTimeout) {
    eyebrows_setRight(rightEyebrowDefault);
    rightEyebrowTimeout = 0;
    rightEyebrowDone = true;
  }

  if (middleEyebrowStartTimeout != 0 && now > middleEyebrowStartTimeout) {
    middleEyebrow.write(middleEyebrowTarget);
    middleEyebrowStartTimeout = 0;
  }
  if (middleEyebrowTimeout != 0 && now > middleEyebrowTimeout) {
    eyebrows_setMiddle(middleEyebrowDefault);
    middleEyebrowTimeout = 0;
    middleEyebrowDone = true;
  }
}

void eyebrows_setLeft(int pos) { leftEyebrow.write(pos); }
void eyebrows_setLeftDefault(int pos) { leftEyebrowDefault = pos; }
void eyebrows_setLeftFor(int pos, unsigned long duration) {
  leftEyebrow.write(pos);
  leftEyebrowTimeout = millis() + duration;
  leftEyebrowDone = false;
}
void eyebrows_setLeftForAfter(int pos, unsigned long duration, unsigned long waitTime) {
  leftEyebrowStartTimeout = millis() + waitTime;
  leftEyebrowTimeout = leftEyebrowStartTimeout + duration;
  leftEyebrowTarget = pos;
  leftEyebrowDone = false;
}

void eyebrows_setRight(int pos) { rightEyebrow.write(pos); }
void eyebrows_setRightDefault(int pos) { rightEyebrowDefault = pos; }
void eyebrows_setRightFor(int pos, unsigned long duration) {
  rightEyebrow.write(pos);
  rightEyebrowTimeout = millis() + duration;
  rightEyebrowDone = false;
}
void eyebrows_setRightForAfter(int pos, unsigned long duration, unsigned long waitTime) {
  rightEyebrowStartTimeout = millis() + waitTime;
  rightEyebrowTimeout = rightEyebrowStartTimeout + duration;
  rightEyebrowTarget = pos;
  rightEyebrowDone = false;
}

void eyebrows_setMiddle(int pos) { middleEyebrow.write(pos); }
void eyebrows_setMiddleDefault(int pos) { middleEyebrowDefault = pos; }
void eyebrows_setMiddleFor(int pos, unsigned long duration) {
  middleEyebrow.write(pos);
  middleEyebrowTimeout = millis() + duration;
  middleEyebrowDone = false;
}
void eyebrows_setMiddleForAfter(int pos, unsigned long duration, unsigned long waitTime) {
  middleEyebrowStartTimeout = millis() + waitTime;
  middleEyebrowTimeout = middleEyebrowStartTimeout + duration;
  middleEyebrowTarget = pos;
  middleEyebrowDone = false;
}

//Eyes Functions
void eyes_setup(int red, int green, int yellow, int white) {
  redPin = red;
  pinMode(redPin, OUTPUT);
  greenPin = green;
  pinMode(greenPin, OUTPUT);
  yellowPin = yellow;
  pinMode(yellowPin, OUTPUT);
  whitePin = white;
  pinMode(whitePin, OUTPUT);
}

void eyes_update() {
  unsigned long now = millis();

  if(greenEyesBlinking){
    if(now > lastBlinkUpdate + 500){
      digitalWrite(greenPin, !digitalRead(greenPin));
      lastBlinkUpdate = now;
    }
  }

  if(whiteEyesBlinking){
    if(now > lastBlinkUpdate + 500){
      digitalWrite(whitePin, !digitalRead(whitePin));
      lastBlinkUpdate = now;
    }
  }

  if (redEyeStartTimeout != 0 && now > redEyeStartTimeout) {
    digitalWrite(redPin, HIGH);
    redEyeStartTimeout = 0;
  }
  if (redEyeTimeout != 0 && now > redEyeTimeout) {
    digitalWrite(redPin, LOW);
    redEyeTimeout = 0;
    redEyeDone = true;
  }

  if (greenEyeStartTimeout != 0 && now > greenEyeStartTimeout) {
    digitalWrite(greenPin, HIGH);
    greenEyeStartTimeout = 0;
  }
  if (greenEyeTimeout != 0 && now > greenEyeTimeout) {
    digitalWrite(greenPin, LOW);
    greenEyeTimeout = 0;
    greenEyeDone = true;
    greenEyesBlinking = false;
  }

  if (yellowEyeStartTimeout != 0 && now > yellowEyeStartTimeout) {
    digitalWrite(yellowPin, HIGH);
    yellowEyeStartTimeout = 0;
  }
  if (yellowEyeTimeout != 0 && now > yellowEyeTimeout) {
    digitalWrite(yellowPin, LOW);
    yellowEyeTimeout = 0;
    yellowEyeDone = true;
  }

  if (whiteEyeStartTimeout != 0 && now > whiteEyeStartTimeout) {
    digitalWrite(whitePin, HIGH);
    whiteEyeStartTimeout = 0;
  }
  if (whiteEyeTimeout != 0 && now > whiteEyeTimeout) {
    digitalWrite(whitePin, LOW);
    whiteEyeTimeout = 0;
    whiteEyeDone = true;
    whiteEyesBlinking = false;
  }
}

void eye_redFor(unsigned long duration) {
  digitalWrite(redPin, HIGH);
  redEyeTimeout = millis() + duration;
  redEyeDone = false;
}
void eye_redForAfter(unsigned long duration, unsigned long waitTime) {
  redEyeStartTimeout = millis() + waitTime;
  redEyeTimeout = redEyeStartTimeout + duration;
  redEyeDone = false;
}

void eye_greenFor(unsigned long duration) {
  digitalWrite(greenPin, HIGH);
  greenEyeTimeout = millis() + duration;
  greenEyeDone = false;
}
void eye_greenForAfter(unsigned long duration, unsigned long waitTime) {
  greenEyeStartTimeout = millis() + waitTime;
  greenEyeTimeout = greenEyeStartTimeout + duration;
  greenEyeDone = false;
}

void eye_yellowFor(unsigned long duration) {
  digitalWrite(yellowPin, HIGH);
  yellowEyeTimeout = millis() + duration;
  yellowEyeDone = false;
}
void eye_yellowForAfter(unsigned long duration, unsigned long waitTime) {
  yellowEyeStartTimeout = millis() + waitTime;
  yellowEyeTimeout = yellowEyeStartTimeout + duration;
  yellowEyeDone = false;
}

void eye_whiteFor(unsigned long duration) {
  digitalWrite(whitePin, HIGH);
  whiteEyeTimeout = millis() + duration;
  whiteEyeDone = false;
}
void eye_whiteForAfter(unsigned long duration, unsigned long waitTime) {
  whiteEyeStartTimeout = millis() + waitTime;
  whiteEyeTimeout = whiteEyeStartTimeout + duration;
  whiteEyeDone = false;
}

void eyeBlink_greenFor(unsigned long duration) {
  digitalWrite(greenPin, HIGH);
  greenEyeTimeout = millis() + duration;
  lastBlinkUpdate = millis();
  greenEyesBlinking = true;
}

void eyeBlink_whiteFor(unsigned long duration) {
  digitalWrite(whitePin, HIGH);
  whiteEyeTimeout = millis() + duration;
  lastBlinkUpdate = millis();
  whiteEyesBlinking = true;
}

