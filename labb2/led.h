#ifndef _LED_H_
#define _LED_H_

#include "stdint.h"

void ledInit();
void fade();
uint8_t simpleRamp(uint8_t *pwm, uint8_t *direction);

#endif // _LED_H_

