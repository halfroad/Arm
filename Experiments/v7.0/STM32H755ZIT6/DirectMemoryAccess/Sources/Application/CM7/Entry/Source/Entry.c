#include <stdio.h>
#include <string.h>

#include "../../Initializers//Include/SystemInitializer.h"
#include "../../Peripherals/Include/Delay.h"
#include "../../Peripherals/Include/LED.h"
#include "../../Peripherals/Include/OLED.h"
#include "../../Peripherals/Include/GPIO.h"
#include "../../Peripherals/Include/USART.h"
#include "../../Peripherals/Include/DirectMemoryAccess.h"

/*

Table 125. DMAMUX1: assignment of multiplexer inputs to resource.
    
    DMA request MUX input       Resource
    45                          usart3_rx_dma
    46                          usart3_tx_dma
*/
#define DMA_REQUEST_MUX_INPUT                           46
#define TRANSMISSION_BUFFER_LENGTH                      7800

uint8_t transmissionBuffer[TRANSMISSION_BUFFER_LENGTH] __attribute__((section(".ARM.__at_0x24070000")));

const uint8_t TEXT_TO_SEND[] = { "Hello, this is a message dilivered via USART." };

void onReceived(uint8_t *buffer, uint8_t length);

int main(void)
{
    /*
    
    User LD1: a green user LED is connected to the STM32H7 I/O PB0 (SB65 OFF and SB54
    ON) or PA5 (SB65 ON and SB54 OFF) corresponding to the ST Zio D13.
    
    */

    /*
    
    B1 user (Blue button): the user button is connected to the I/O PC13 by default (Tamper
    support, SB82 ON and SB81 OFF) or PA0 (Wakeup support, SB81 ON and SB82 OFF) of
    the STM32H7 microcontroller.
    
    https://community.st.com/t5/stm32-mcus/dma-is-not-working-on-stm32h7-devices/ta-p/49498
    
    */
    
#ifdef USE_HSE
    InitSystem(1, 120, 2, 2, 2);
#else
    InitSystem(4, 60, 2, 2, 2);
#endif
    InitDelay(240);
    InitGPIOs();
    InitLEDs();
    OLED_Init();
    
    InitUSART(60, 115200, onReceived);
    InitDirectMemoryAccess((uint32_t)transmissionBuffer, (uint32_t)&USART -> TDR, DMA_REQUEST_MUX_INPUT, sizeof transmissionBuffer);

    uint8_t t = 0;
    uint8_t j = sizeof(TEXT_TO_SEND), mask = 0;
    
    memset(transmissionBuffer, 0, TRANSMISSION_BUFFER_LENGTH);
    
    for (uint16_t i = 0; i < TRANSMISSION_BUFFER_LENGTH; i ++)
    {
        if (t >= j)
        {
            if (mask)
            {
                transmissionBuffer [i] = 0x0a;
                
                t = 0;
            }
            else 
            {
                transmissionBuffer [i] = 0x0d;
                
                mask ++;
            }    
        }
        else
        {
            mask = 0;
            
            transmissionBuffer [i] = TEXT_TO_SEND [t];
            
            t++;
        }          
    }
        
    j = 0;
    
    while (1)
    {
        /*
        PC13 Blue Push Button.
        */
        if (0x2000 == (GPIOC -> IDR & 0x2000))
        {
            DelayMilliseconds(10);
            
            if (0x2000 == (GPIOC -> IDR & 0x2000))
            {
                while (0x2000 == (GPIOC -> IDR & 0x2000))
                    ;
                
                OLED_Clear();
                
                OLED_ShowString(8, 5, (uint8_t *)"Start Transmission:\n", 8, 1);
                
                OLED_Refresh();
                /*
                
                Bit 7 DMAT: DMA enable transmitter
                This bit is set/reset by software
                    1: DMA mode is enabled for transmission
                    0: DMA mode is disabled for transmission
                
                */
                                
                StartConveyItems(DMA_STREAM_TYPE, sizeof transmissionBuffer);
            }
        }
        
        j ++;
        
        DelayMilliseconds(10);
        
        if (j == 25)
        {
             /*
        
            Bits 31:16 BR[15:0]: Port x reset I/O pin y (y = 15 to 0)
            These bits are write-only. A read to these bits returns the value 0x0000.
            0: No action on the corresponding ODRx bit
            1: Resets the corresponding ODRx bit
            Note: If both BSx and BRx are set, BSx has priority.
            
            Bits 15:0 BS[15:0]: Port x set I/O pin y (y = 15 to 0)
            These bits are write-only. A read to these bits returns the value 0x0000.
            0: No action on the corresponding ODRx bit
            1: Sets the corresponding ODRx bit
            
            */
            
            static uint8_t on = 0;
            
            on = on == 0? 1: 0;
            
            GPIOB -> ODR &= ~(0x01 << 0);
            GPIOB -> ODR |= on << 0;
            
            j = 0;
        }   
    }
}

void DMA_STREAM_IRQHANDLER(void)
{
    char buffer[30];
    
    /*
    
    DMA high interrupt status register (DMA_HISR)
    
    Bits 27, 21, 11, 5 TCIF[7:4]: stream x transfer complete interrupt flag (x = 7 to 4)
    This bit is set by hardware. It is cleared by software writing 1 to the corresponding bit in the DMA_HIFCR register.
        0: no transfer complete event on stream x
        1: a transfer complete event occurred on stream x
    
    */
    if (DMA -> HISR & (0x01 << 22))
    {
         /*
        
        Bits 22, 16, 6, 0 FEIF[7:4]: stream x FIFO error interrupt flag (x = 7 to 4)
        This bit is set by hardware. It is cleared by software writing 1 to the corresponding bit in the
        DMA_HIFCR register.
            0: no FIFO error event on stream x
            1: a FIFO error event occurred on stream x
        
        */
        
        DMA -> HIFCR |= 0x01 << 22;
        
        sprintf(buffer, "FIFO error occured.");
        
        OLED_ShowString(8, 20, (uint8_t *)buffer, 8, 1);
    }
    if (DMA -> HISR & (0x01 << 24))
    {
        /*
        
        Bits 24, 18, 8, 2 DMEIF[7:4]: stream x direct mode error interrupt flag (x = 7 to 4)
        This bit is set by hardware. It is cleared by software writing 1 to the corresponding bit in the
        DMA_HIFCR register.
            0: no direct mode error on stream x
            1: a direct mode error occurred on stream x
        
        */
        
        DMA -> HIFCR |= 0x01 << 24;
        
        sprintf(buffer, "Direct mode error occured.");
        
        OLED_ShowString(8, 20, (uint8_t *)buffer, 8, 1);
    }
    else if (DMA -> HISR & (0x01 << 25))
    {
        /*
        
        Bits 25, 19, 9, 3 TEIF[7:4]: stream x transfer error interrupt flag (x = 7 to 4)
        This bit is set by hardware. It is cleared by software writing 1 to the corresponding bit in the
        DMA_HIFCR register.
            0: no transfer error on stream x
            1: a transfer error occurred on stream x
        
        */
        
        DMA -> HIFCR |= 0x01 << 25;
        
        sprintf(buffer, "Transfer error occured.");
        
        OLED_ShowString(8, 20, (uint8_t *)buffer, 8, 1);
    }
    else if (DMA -> HISR & (0x01 << 26) || DMA -> HISR & (0x01 << 27))
    {
        /*
        
        Bits 27, 21, 11, 5 CTCIF[7:4]: stream x clear transfer complete interrupt flag (x = 7 to 4)
        Writing 1 to this bit clears the corresponding TCIFx flag in the DMA_HISR register.
        
        */
        DMA -> HIFCR |= 0x01 << 26;
        DMA -> HIFCR |= 0x01 << 27;
        
        float residualProportionate = DMA_STREAM_TYPE -> NDTR;
        
        residualProportionate = 1 - residualProportionate / sizeof transmissionBuffer;
        residualProportionate *= 100;
        
        sprintf(buffer, "Residual: %.1f%%", residualProportionate);
        
        /*  OLED_ShowNum(8, 20, buffer, 8, 1);  */
        OLED_ShowString(8, 20, (uint8_t *)buffer, 8, 1);
    }
    
    OLED_Refresh();
}

void onReceived(uint8_t *buffer, uint8_t length)
{
    OLED_Clear();
    
    OLED_ShowString(8, 16, buffer, 8, 1);
    
    OLED_Refresh();
}
