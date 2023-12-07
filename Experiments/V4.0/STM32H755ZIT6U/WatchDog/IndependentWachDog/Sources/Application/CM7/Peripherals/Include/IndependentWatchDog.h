#ifndef INDEPENDENT_WATCH_DOG_H
#define INDEPENDENT_WATCH_DOG_H

#include <stm32h7xx.h>

void InitIndependentWatchDog(uint8_t prescaler, uint16_t reload);
void FeedIndependentWatchDog(void);

#endif