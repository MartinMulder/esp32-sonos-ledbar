#include "Arduino.h"
#include "SonosLEDBar.h"

#define LED_FREQUENCY 5000
#define LED_RESOLUTION 8

SonosLEDBar::SonosLEDBar(bool debug)
{
	//Serial.begin(115200);
}

int SonosLEDBar::setup()
{
	Serial.println("SonosLEDBar setup");
	for(int i = 0; i < SONOSLEDBAR_NUMLEDS; i++)
	{
		Serial.print("Setting pin (R): ");
		Serial.print(leds[i].pin_r, DEC);
		Serial.println(" to OUTPUT");
		Serial.print("Setting pin (G): ");
		Serial.print(leds[i].pin_g, DEC);
		Serial.println(" to OUTPUT");
		Serial.print("Setting pin (B): ");
		Serial.print(leds[i].pin_b, DEC);
		Serial.println(" to OUTPUT");

		// Set the pin as an output pin
		pinMode(leds[i].pin_r, OUTPUT);
		pinMode(leds[i].pin_g, OUTPUT);
		pinMode(leds[i].pin_b, OUTPUT);


		// Configure a led channel per led
		ledcSetup(i*3, LED_FREQUENCY, LED_RESOLUTION); 
		ledcSetup(i*3+1, LED_FREQUENCY, LED_RESOLUTION); 
		ledcSetup(i*3+2, LED_FREQUENCY, LED_RESOLUTION); 

		// Add a led channel to the pin
		ledcAttachPin(leds[i].pin_r, i*3);
		ledcAttachPin(leds[i].pin_g, i*3+1);
		ledcAttachPin(leds[i].pin_b, i*3+2);

		// Turn off the led
		showNone(i*3);
	}
}
void SonosLEDBar::addLed(int r, int g, int b)
{
	leds[_registeredLeds] = {r, g, b};
	_registeredLeds++;
}

void SonosLEDBar::handle() 
{
	// Get a timer
	unsigned long currentMillis = millis();

	// Switching state machine
	switch(_currentState) 
	{
		// We are in ROOMS mode
		case ROOMS:
			//Serial.print(F("currentState: ROOMS"));
			showRed(0);
			showRed(3);
			showRed(6);
			showRed(9);
			delay(3000);
			showOrange(0);
			showOrange(3);
			showOrange(6);
			showOrange(9);
			delay(3000);
			showYellow(0);
			showYellow(3);
			showYellow(6);
			showYellow(9);
			delay(3000);


			Serial.println("Loop through volume");
			for( int j = 0; j <= 50; j++) {
    		setVolume(j*5);
				showVolumeOnLeds();
  	  	delay(2000);
  		}

			break;
		// We are in VOLUME MODE
		case VOLUME:
			// Check if we still need to display the volume
			if (currentMillis - _lastVolumeSet > SONOSLEDBAR_VOLUMEDISPLAYTIME) {
				Serial.println("Unsetting state, back to ROOMS");

				// Set the _lastVolumeSet to prevent multiple signals
				// But because of the state machine change this should not happen
				_lastVolumeSet = currentMillis;

				// Displaytime is over, back to ROOMS indicator
				_currentState = ROOMS;
			} else {
				showVolumeOnLeds();
			}
			//Serial.print(F("currentState: VOLUME"));
			
			break;
	}
}

void SonosLEDBar::setVolume(uint8_t volume)
{
	Serial.print("Setting volume to: ");
	Serial.println(volume);
	if (volume != _lastKnownVolume) {
		_currentState = VOLUME;
		Serial.print("Setting currentState to: " );
		Serial.println(_currentState);

		_lastVolumeSet = millis();	
		Serial.print("Setting lastVolumeSet to: " );
		Serial.println(_lastVolumeSet);
		_lastKnownVolume = volume;
		Serial.print("Setting lastKnownVolume to: " );
		Serial.println(_lastKnownVolume);
	} else {
		Serial.println("Volume not changed");
	}

}

void SonosLEDBar::showVolumeOnLeds()
{
	// Get the steps a led should display
	int volumeStepsPerLed = 255/SONOSLEDBAR_NUMLEDS;
	int fullRedLeds = _lastKnownVolume/volumeStepsPerLed;
	int moduloLastLed = _lastKnownVolume % volumeStepsPerLed;

	Serial.print("Steps per Led: " );
	Serial.println(volumeStepsPerLed);
	Serial.print("fullRedLeds: ");
	Serial.println(fullRedLeds);
	Serial.print("ModuloLastLed: " );
	Serial.println(moduloLastLed);

	// Loop through the leds
	for (int i = 0; i < SONOSLEDBAR_NUMLEDS; i++) {
					if (i+1 <= fullRedLeds) {
									Serial.print("Setting led: ");
									Serial.print(i+1);
									Serial.println(" to red");
									showRed(i*3);
					} else if (i+1 == fullRedLeds + 1 && moduloLastLed > 0) {
									int foo = volumeStepsPerLed/3;
									if (foo < moduloLastLed && foo*2 < moduloLastLed) {
													Serial.print("Setting led: ");
													Serial.print(i+1);
													Serial.println(" to orange");
													showOrange(i*3);
									} else if (foo < moduloLastLed) {
													Serial.print("Setting led: ");
													Serial.print(i+1);
													Serial.println(" to yellow");
													showYellow(i*3);
									} else {
													Serial.print("Setting led: ");
													Serial.print(i+1);
													Serial.println(" off");
													showNone(i*3);
									}
					} else {
									Serial.print("Turn off led: ");
									Serial.println(i+1);
									showNone(i*3);
					}
	}
}

// Only give the R Channel, G and B channels are a derivative
void SonosLEDBar::showRed(uint8_t r_channel)
{
  Serial.print("Setting red on r_channel: " );
	Serial.println(r_channel);

	ledcWrite(r_channel, 155);
	ledcWrite(r_channel+1, 0);
	ledcWrite(r_channel+2, 0);
}
													
// Only give the R Channel, G and B channels are a derivative
void SonosLEDBar::showOrange(uint8_t r_channel)
{
  Serial.print("Setting orange on r_channel: " );
	Serial.println(r_channel);

	ledcWrite(r_channel, 175);
	ledcWrite(r_channel+1, 50);
	ledcWrite(r_channel+2, 0);
}
// Only give the R Channel, G and B channels are a derivative
void SonosLEDBar::showYellow(uint8_t r_channel)
{
  Serial.print("Setting yellow on r_channel: " );
	Serial.println(r_channel);

	ledcWrite(r_channel, 140);
	ledcWrite(r_channel+1, 105);
	ledcWrite(r_channel+2, 0);
}
// Only give the R Channel, G and B channels are a derivative
void SonosLEDBar::showNone(uint8_t r_channel)
{
  Serial.print("Setting off on r_channel: " );
	Serial.println(r_channel);

	ledcWrite(r_channel, 0);
	ledcWrite(r_channel+1, 0);
	ledcWrite(r_channel+2, 0);
}


