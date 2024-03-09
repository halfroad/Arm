#ifndef __INTER_INTEGRATED_CIRCUIT_H
#define __INTER_INTEGRATED_CIRCUIT_H

#include <stm32h7xx.h>

#include "../Include/Delay.h"

void InitInterIntegratedCircuit(void);

/*

Issue I2C Start Signal circumstances,

1. SLC keeps high for a stamina t(SU: STA).
2. SDA jumps from high to low, and keep the low state for the stamina t(HD: STA)

*/
void IssueStartSignal (void);

/*

Issue I2C Start Signal circumstances,

1. SLC keeps high.
2.1. SDA jumps from low to high,
2.2. and keep the high state for the stamina t(SU: STA).

*/
void IssueStopSignal(void);

/*

Bits giver waits for the acknowledgement from Slave (Peer of bits receiver), circumstances,

*/
uint8_t PollAcknowledgement(void);

/*

Issue I2C Acknowledgement Signal circumstances,

1. Post 8 consecutive bits received by the peer, the peer acknowledges the bits giver.
2.1. The peer sends back with low level to the giver, means the peer acknowledges that the bits are well received. High level means transmission is done.
2.2. the bits giver might determine the next action depends on the acknowledgement.
2.2. When the peer is no acknowledged, means the bits giver is out of services.

*/

void IssueAcknowledgementSignal(uint8_t acknowledgementRequired);

void TransmitByte(uint8_t byte);

uint8_t ReadByte(uint8_t acknowledgementRequired);

#endif  /*  #ifndef __INTER_INTEGRATED_CIRCUIT_H    */