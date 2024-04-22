#ifndef __INTER_INTEGRATED_CIRCUITS_H
#define __INTER_INTEGRATED_CIRCUITS_H

#include <stm32g4xx.h>

typedef enum
{
    AcknowledgeNotRequired      = 0x00,
    AcknowledgeRequired         = 0x01,
    
} AcknowledgeRequirements;

void InitInterIntegratedCircuits(void);

void IssueStartSignal(void);
void IssueStopSignal(void);

void WriteByte(uint8_t byte, AcknowledgeRequirements pollForAcknowledgement);
uint8_t ReadByte(AcknowledgeRequirements acknowledgeTransmitterNeeded);

#endif  /*  #ifndef __INTER_INTEGRATED_CIRCUITS_H   */