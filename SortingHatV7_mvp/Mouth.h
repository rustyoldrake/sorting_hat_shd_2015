#ifndef MOUTH_h
#define MOUTH_h

#include <Arduino.h>

enum MouthMovementState {
	MOUTH_OPENING,
	MOUTH_CLOSING,
	MOUTH_STOPPED
};

class Mouth{
	public:
                bool isTalking;
                
		Mouth();

		void talk(int duration);

		// stopTalking();

		void update();

		void setup();

	protected:
		unsigned long _lastUpdated, _duration, _startTime;
		MouthMovementState _mouthMovementState;

		void open();
		void close();
		void stop();
};

#endif
