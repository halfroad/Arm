#ifndef REAL_TIME_CLOCK_H
#define REAL_TIME_CLOCK_H

#include <stm32h7xx.h>

uint8_t InitRealTimeClock(void);

uint8_t RealTimeClockSetTime(uint8_t hour, uint8_t minute, uint8_t second, uint8_t ampm);
uint8_t RealTimeClockSetDate(uint8_t year, uint8_t month, uint8_t day, uint8_t weekday);

void RealTimeClockGetTime(uint8_t *hour, uint8_t *minute, uint8_t *second, uint8_t *ampm);
void RealTimeClockGetDate(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *weekday);

void RealTimeClockSetAlarmA(uint8_t weekday, uint8_t hour, uint8_t minute, uint8_t second);
void RealTimeClockPeriodicWakeup(uint8_t weekdaySelection, uint16_t autoReload);

#endif