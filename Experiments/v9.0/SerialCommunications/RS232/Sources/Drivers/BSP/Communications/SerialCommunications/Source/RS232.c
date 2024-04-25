#include <string.h>

#include "../Include/SerialCommunications.h"

#include "../Include/RS232.h"

#define BAUD_RATE                   115200

void InitRS485(void onBytesReceivedHandler(void *protocol, uint8_t *bytes, uint16_t length))
{
    InitSerialCommunications(BAUD_RATE, NULL, onBytesReceivedHandler);
}

void SendRS485Message(char *message)
{
    TransmitDMA((uint8_t *)message, strlen(message));
}