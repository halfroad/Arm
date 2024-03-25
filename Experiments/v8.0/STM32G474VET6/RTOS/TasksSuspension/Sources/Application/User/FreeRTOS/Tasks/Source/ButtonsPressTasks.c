#include "../../../Drivers/BSP/ATKDMG474/Include/PushButton.h"
#include "../../../User/FreeRTOS/Tasks/Include/FlashTasks.h"

#include "../Include/ButtonsPressTasks.h"

void ButtonPressTask(void *arguments)
{
    KeyPressStates keyPressState;
    
    while (1)
    {
        keyPressState = ScanButton();
        
        switch (keyPressState)
        {
            case KEY_0_PRESSED:
                
                SuspendFlashGreenLedTask();
            
                break;
            
            case KEY_1_PRESSED:
                
                ResumeFlashGreenLedTask();
            
                break;
            
            case KEY_2_PRESSED:
                break;
            
            default:
                break;
        }
    }
}