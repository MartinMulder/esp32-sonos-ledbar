/*
   Created by Martin Mulder - deFEEST, April 2021
   Released into the public domain
   */

#ifndef SonosLEDBar_h
#define SonosLEDBar_h

#ifndef SONOSLEDBAR_NUMLEDS
#define SONOSLEDBAR_NUMLEDS 4
#endif

// How long should the volume been displayd
#ifndef SONOSLEDBAR_VOLUMEDISPLAYTIME
#define SONOSLEDBAR_VOLUMEDISPLAYTIME 5000
#endif

#include "Arduino.h"

class SonosLEDBar
{
	public:
		struct led {
			int pin_r, pin_g, pin_b;
		}; 

		typedef enum capabilities {
			VOLUME,
			ROOMS
		};
		capabilities currentMode = ROOMS;
		SonosLEDBar(bool debug = false);
		int setup();
		void handle();
		void addLed(int r, int g, int b);
		void setVolume(uint8_t volume);

	private:
		unsigned long timer_now = 0;
		bool _debug;
		uint8_t _registeredLeds = 0;
		SonosLEDBar::led leds[];
		SonosLEDBar::capabilities _currentState;
		uint8_t _lastKnownVolume = 100;
		unsigned long _lastVolumeSet;
		void showVolumeOnLeds();
		void showRed(uint8_t r_channel);
		void showOrange(uint8_t r_channel);
		void showYellow(uint8_t r_channel);
		void showNone(uint8_t r_channel);
};	

#endif
