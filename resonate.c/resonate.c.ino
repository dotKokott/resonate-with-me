#include "pitches.h"
#include <stdlib.h>

const int PLAY_TONES = 1;

const int outs[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10 };
const int COUNT = 1;

const int duration = 200;

const int MIN_DURATION = 5;
const int MAX_DURATION = 1000;

const int a_PIN1 = 5;
const int a_PIN2 = 4;

const int WAIT_BUTTON_PIN = 11;

int waitButtonState = 0;

float map_range (float value, float low1, float high1, float low2, float high2) {
  return low2 + (high2 - low2) * (value - low1) / (high1 - low1);
}

unsigned long startTime = 0;
unsigned long elapsedTime = 0;
unsigned long oldTime = 0;
unsigned long newTime = 0;
unsigned long deltaTime = 0;

unsigned long switchTimer = 0;
unsigned long _switchTimer = 0;

int currentIdx = 0;
int switches = 0;

void playCurrentTone() {
  tone(outs[currentIdx], getFrequency());
}

void setup() {
  startTime = millis();
  pinMode(WAIT_BUTTON_PIN, INPUT_PULLUP);
  for (int i = 0; i < COUNT; i++) {
    pinMode(outs[i], OUTPUT);
  }
  Serial.begin(9600);

  playCurrentTone();
}


float getFrequency() {
  float freq = (float)analogRead(a_PIN2);
  return map_range(freq, 0, 1023, 31, 4978);
}

float getRandomFrequency() {
  float freq = (float)analogRead(3);
  return map_range(freq, 0, 1023, 31, 4978);
}

float getDuration() {
  float duration = (float)analogRead(a_PIN1);

  return map_range(duration, 0, 1023, MIN_DURATION, MAX_DURATION);
}

void calculateTimes() {
  newTime = millis();
  elapsedTime = newTime - startTime;
  deltaTime = newTime - oldTime;
  oldTime = newTime;
}

void newLoop() {
  waitButtonState = digitalRead(WAIT_BUTTON_PIN);
  
  switchTimer += deltaTime;
  if (switchTimer >= getDuration() && waitButtonState) {
    switchTimer = 0;
    switches++;
    
    int previousIdx = currentIdx;
    currentIdx++;
    if (currentIdx >= COUNT) currentIdx = 0;

    noTone(outs[previousIdx]);
    
    playCurrentTone();
  }  
  if (!waitButtonState) {    
    playCurrentTone();
  }
}

void loop() {
  calculateTimes();  
  

  if (!PLAY_TONES) {
    return;
  }

  newLoop();
}
