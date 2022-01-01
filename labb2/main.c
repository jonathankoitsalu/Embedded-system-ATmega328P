#include "bitshift.h"
#include "led.h"
#include "timer.h"

//Deluppgift 3: simpleRamp (fade). Led är kopplad till digital pin 6 (PD6).
//Beräkningar för periodtid 16 ms i Timer.c


void main (void) {
	timerInit();
	ledInit();
	fade();
}
