//Keypad Activated Door
//by Ryan Payne
//Version 1.1
//Added multiple settings for unlocking system
//A indeffinite unlock
//B Unlock for 1 second
//C Unlock for 10 seconds
//* Final key

#include <Keypad.h>
#include <Stepper.h>

#define STEPS 32

const int oneRev = 2048;
const int lockturn = 512;
const int unlockturn = -512;

int buttonState  = 0;

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

char keyPressed;
char code[] = {'1', '1', '2', '3', '5', '8'};

char code_buff1[sizeof(code)];
char code_buff2[sizeof(code)];

boolean doorLocked = false;

short a, i, s, j = 0;

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

const byte led = 0;
const byte orswitch = 1;

Stepper stepper(STEPS, 10, 12, 11, 13);

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup() {
  pinMode(orswitch, INPUT);
  pinMode(led, OUTPUT);

  stepper.setSpeed(500);

  stepper.step(lockturn);
  stepper.step(unlockturn);

  delay(1000);
  blinkled();
  dropPins();
}

void loop() {
  keyPressed = customKeypad.getKey();
  buttonState = digitalRead(orswitch);
  
  if (keyPressed == 'A') {
    GetCode();
    if (a == sizeof(code)) {
      OpenDoor();
    }
    delay(2000);
  } if (keyPressed == 'B') {
    GetCode();
    if (a == sizeof(code)) {
      OpenDoor();
      delay(1000);
      CloseDoor();
    }
    delay(2000);
  } if (keyPressed == 'C') {
    GetCode();
    if (a == sizeof(code)) {
      OpenDoor();
      delay(10000);
      CloseDoor();
    }
    delay(2000);
  } else if (!doorLocked && keyPressed == '#') {
    CloseDoor();
  }

  if (buttonState == HIGH) {
    overRide();
  }
}

void GetCode() {
  i = 0;
  a = 0;

  while (keyPressed != '*') {
    keyPressed = customKeypad.getKey();
    if (keyPressed != NO_KEY && keyPressed != '*') {
      //Serial.println('*');
      if (keyPressed == code[i] && i < sizeof(code)) {
        a++;
        i++;
      } else {
        a--;
      }
    }
  }
  keyPressed = NO_KEY;
}

void OpenDoor() {
  //Serial.println("Welcome");
  if (doorLocked) {
    doorLocked = false;
    stepper.step(unlockturn);
    digitalWrite(led, LOW);
    dropPins();
  }
}

void CloseDoor() {
  if (!doorLocked) {
    //Serial.println("Door is Locked");
    doorLocked = true;
    stepper.step(lockturn);
    digitalWrite(led, HIGH);
    dropPins();
  }
}

void overRide() {
  while (buttonState == HIGH) {
    CloseDoor();
    buttonState = digitalRead(orswitch);
  }
  OpenDoor();
}

void dropPins() {
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
}

void blinkled() {
  digitalWrite(led, HIGH);
  delay(250);
  digitalWrite(led, LOW);
  delay(250);
  digitalWrite(led, HIGH);
  delay(250);
  digitalWrite(led, LOW);
  delay(250);
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
}
