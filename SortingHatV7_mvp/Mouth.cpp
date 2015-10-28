#include "Mouth.h"

static int AIA = 5;
static int AIB = 6;

static int jawSpeed = 150;  // change this (0-255) to control the speed of the motors 
static int jawDelay = 340;

Mouth::Mouth() {
	_lastUpdated = 0;
	isTalking = false;
	_mouthMovementState = MOUTH_STOPPED;
	//TODO: ensure mouth is closed
}

void Mouth::setup(){
  pinMode(AIA, OUTPUT); // set pins to output
  pinMode(AIB, OUTPUT);
}

void Mouth::talk(int duration){
	_duration = duration;
	_startTime = millis();

	this->open();

	isTalking = true;
	_lastUpdated = millis();
}

// void Mouth::stopTalking(){
// 	this->close();
// 	isTalking = false;
// 	_lastUpdated = 0;
// }

void Mouth::update(){
        //TODO: make mouth close after its done talking, currently it may stop open or closed
	if(!isTalking){return;}
	if(millis() - _startTime > _duration){
		isTalking = false;
                //if open, then close, and delay 340, then stop
		this->stop();
		_startTime = 0;
		_duration = 0;
		_lastUpdated = 0;
                Serial.println('done talking');
		return;
	}
	if(millis() - _lastUpdated > jawDelay){
		if(_mouthMovementState == MOUTH_CLOSING){
			this->open();
		}else if(_mouthMovementState == MOUTH_OPENING){
			this->close();
		}
	}
}

void Mouth::open(){
    analogWrite(AIA, jawSpeed);
    analogWrite(AIB, 0);
    _mouthMovementState = MOUTH_OPENING;
    _lastUpdated = millis();
    Serial.println('opening mouth');
}

void Mouth::close(){
    analogWrite(AIA, 0);
    analogWrite(AIB, jawSpeed);
    _mouthMovementState = MOUTH_CLOSING;
    _lastUpdated = millis();
    Serial.println('closing mouth');
}

void Mouth::stop(){
    analogWrite(AIA, 0);
    analogWrite(AIB, 0);
    _mouthMovementState = MOUTH_STOPPED;
    _lastUpdated = millis();
    Serial.println('stopped mouth');
}

