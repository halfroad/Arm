#ifndef WINDOW_WATCH_DOG
#define WINDOW_WATCH_DOG

#include <stm32h7xx.h>

void InitWindowWatchDog(uint8_t prescaler, uint8_t counter, uint8_t window);
void FeedWindowWatchDog(void);

#endif