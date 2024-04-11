#include "./ControllerAreaNetwork/Include/ControllerAreaNetwork.h"

#define FDCAN                                                           FDCAN1

#define RCC_FDCAN_CLOCK_ENABLE()                                        do                                      \
                                                                        {                                       \
                                                                            __HAL_RCC_FDCAN_CLK_ENABLE();       \
                                                                        }                                       \
                                                                        while   (0)                             \

/*  #define FDCAN_OPERATING_MODE                                            FDCAN_MODE_NORMAL   */
#define FDCAN_OPERATING_MODE                                            FDCAN_MODE_INTERNAL_LOOPBACK

#define FDCAN_FRAME_FORMAT                                              FDCAN_FRAME_CLASSIC
#define STANDARD_FILTERS_NUMBER                                         28
#define EXTENDED_FILTERS_NUMBER                                         8

#define FDCAN_FILTER_TYPE_FILTER_TYPE                                   FDCAN_FILTER_MASK
#define FDCAN_FILTER_TYPE_FILTER_CONFIG                                 FDCAN_FILTER_TO_RXFIFO0

#define FDCAN_FILTER_TYPE_FILTER_ID1                                    0x0000
#define FDCAN_FILTER_TYPE_FILTER_ID2                                    0x0000

#define NON_MATCHING_STANDARD_OR_EXTENED_REJECT                         FDCAN_REJECT
#define REJECT_REMOTE_STANDARD_OR_EXTENED                               FDCAN_FILTER_REMOTE

#define NOTIFICATION_ACTIVE_INTERRUPTS                                  FDCAN_IT_RX_FIFO0_NEW_MESSAGE
#define NOTIFICATION_BUFFER_INDEXES                                     0

#define CAN_RX_GPIO_PORT                                                GPIOA

#define RCC_CAN_RX_GPIO_PORT_ENABLE()                                   do                                      \
                                                                        {                                       \
                                                                            __HAL_RCC_GPIOA_CLK_ENABLE();       \
                                                                        }                                       \
                                                                        while   (0)                             \
                                                                            
#define CAN_RX_GPIO_PIN                                                 GPIO_PIN_11
#define CAN_RX_GPIO_PIN_ALTERNATE_FUNCTION                              GPIO_AF9_FDCAN1

#define CAN_TX_GPIO_PORT                                                GPIOA

#define RCC_CAN_TX_GPIO_PORT_ENABLE()                                   do                                      \
                                                                        {                                       \
                                                                            __HAL_RCC_GPIOA_CLK_ENABLE();       \
                                                                        }                                       \
                                                                        while   (0)                             \

#define CAN_TX_GPIO_PIN                                                 GPIO_PIN_12
#define CAN_TX_GPIO_PIN_ALTERNATE_FUNCTION                              GPIO_AF9_FDCAN1

#define FDCAN_PERIPH_CLOCK_SOURCE_SELECTION                             RCC_FDCANCLKSOURCE_PCLK1
                                                                        
#define FDCAN_IT_IRQn                                                   FDCAN1_IT0_IRQn
#define FDCAN_IT_IRQHANDLER                                             FDCAN1_IT0_IRQHandler                                                                        

#define FDCAN_RX_INTERRUPT_ENABLED                                      1
                                                                        
#define FDCAN_TX_HEADER_TYPE_IDENTIFER                                  0x00000001
#define FDCAN_TX_HEADER_TYPE_ID_TYPE                                    FDCAN_STANDARD_ID
#define FDCAN_TX_HEADER_TYPE_TX_FRAME_TYPE                              FDCAN_DATA_FRAME
#define FDCAN_TX_HEADER_TYPE_BIT_RATE_SWITCH                            FDCAN_BRS_OFF
#define FDCAN_TX_HEADER_TYPE_FD_FORMAT                                  FDCAN_CLASSIC_CAN
#define FDCAN_TX_HEADER_TYPE_TX_EVENT_FIFO_CONTROL                      FDCAN_NO_TX_EVENTS

#define FDCAN_GET_MESSAGE_RX_LOCATION                                   FDCAN_RX_FIFO0

FDCAN_HandleTypeDef FDCAN_HandleType                    = { 0 };

extern void Error_Handler(void);

static void MspInitCallback(FDCAN_HandleTypeDef *hfdcan);

void InitControllerAreaNetwork(uint32_t operatingMode, uint32_t nominalPrescaler, uint32_t nominalSyncJumpWidth, uint32_t nominalTimeSegment1, uint32_t nominalTimeSegment2,
    uint32_t dataPrescaler, uint32_t dataSyncJumpWidth, uint32_t dataTimeSegment1, uint32_t dataTimeSegment2)
{
    
    
    FDCAN_HandleType.Instance                           = FDCAN;
    
    FDCAN_HandleType.Init.ClockDivider                  = FDCAN_CLOCK_DIV1;
    FDCAN_HandleType.Init.FrameFormat                   = FDCAN_FRAME_FORMAT;
    /*  FDCAN_HandleType.Init.Mode                          = FDCAN_OPERATING_MODE; */
    FDCAN_HandleType.Init.Mode                          = operatingMode;
    FDCAN_HandleType.Init.AutoRetransmission            = DISABLE;
    FDCAN_HandleType.Init.TransmitPause                 = DISABLE;
    FDCAN_HandleType.Init.ProtocolException             = DISABLE;
    
    /*  Arbitration Segment.    */
    FDCAN_HandleType.Init.NominalPrescaler              = nominalPrescaler;
    FDCAN_HandleType.Init.NominalSyncJumpWidth          = nominalSyncJumpWidth;
    FDCAN_HandleType.Init.NominalTimeSeg1               = nominalTimeSegment1;
    FDCAN_HandleType.Init.NominalTimeSeg2               = nominalTimeSegment2;
    
    /*  Data Segment.   */
    FDCAN_HandleType.Init.DataPrescaler                 = dataPrescaler;
    FDCAN_HandleType.Init.DataSyncJumpWidth             = dataSyncJumpWidth;
    FDCAN_HandleType.Init.DataTimeSeg1                  = dataTimeSegment1;
    FDCAN_HandleType.Init.DataTimeSeg2                  = dataTimeSegment2;
    
    /*  Filter settings */
    FDCAN_HandleType.Init.StdFiltersNbr                 = STANDARD_FILTERS_NUMBER;
    FDCAN_HandleType.Init.ExtFiltersNbr                 = EXTENDED_FILTERS_NUMBER;
    
    FDCAN_HandleType.Init.TxFifoQueueMode               = FDCAN_TX_FIFO_OPERATION;
    
    FDCAN_HandleType.MspInitCallback                    = MspInitCallback;
    
    if (HAL_OK != HAL_FDCAN_Init(&FDCAN_HandleType))
        Error_Handler();
    
    /*
    
        Configure the filters.
    
    */
    FDCAN_FilterTypeDef FDCAN_FilterType                = { 0 };
    
    FDCAN_FilterType.IdType                             = FDCAN_STANDARD_ID;
    FDCAN_FilterType.FilterIndex                        = 0;
    /*
    
    #define IS_FDCAN_STD_FILTER_TYPE(TYPE) (((TYPE) == FDCAN_FILTER_RANGE) || \
                                        ((TYPE) == FDCAN_FILTER_DUAL ) || \
                                        ((TYPE) == FDCAN_FILTER_MASK
    */
    FDCAN_FilterType.FilterType                         = FDCAN_FILTER_TYPE_FILTER_TYPE;
    /*
    
    #define IS_FDCAN_FILTER_CFG(CONFIG) (((CONFIG) == FDCAN_FILTER_DISABLE      ) || \
                                     ((CONFIG) == FDCAN_FILTER_TO_RXFIFO0   ) || \
                                     ((CONFIG) == FDCAN_FILTER_TO_RXFIFO1   ) || \
                                     ((CONFIG) == FDCAN_FILTER_REJECT       ) || \
                                     ((CONFIG) == FDCAN_FILTER_HP           ) || \
                                     ((CONFIG) == FDCAN_FILTER_TO_RXFIFO0_HP) || \
                                     ((CONFIG) == FDCAN_FILTER_TO_RXFIFO1_HP))
    
    */
    FDCAN_FilterType.FilterConfig                       = FDCAN_FILTER_TYPE_FILTER_CONFIG;
    FDCAN_FilterType.FilterID1                          = FDCAN_FILTER_TYPE_FILTER_ID1;
    FDCAN_FilterType.FilterID2                          = FDCAN_FILTER_TYPE_FILTER_ID2;
        
    if (HAL_OK != HAL_FDCAN_ConfigFilter(&FDCAN_HandleType, &FDCAN_FilterType))
        Error_Handler();
    
    /*
    
        Configure the global filter to reject the standard and extened frames that don't match the filters.
    
    */
    if (HAL_OK != HAL_FDCAN_ConfigGlobalFilter(&FDCAN_HandleType, NON_MATCHING_STANDARD_OR_EXTENED_REJECT, NON_MATCHING_STANDARD_OR_EXTENED_REJECT, REJECT_REMOTE_STANDARD_OR_EXTENED, REJECT_REMOTE_STANDARD_OR_EXTENED))
        Error_Handler();
    
    /*
    
        Start the periph of CAN device.
    
    */
    if (HAL_OK != HAL_FDCAN_Start(&FDCAN_HandleType))
        Error_Handler();
    
    if (HAL_OK != HAL_FDCAN_ActivateNotification(&FDCAN_HandleType, NOTIFICATION_ACTIVE_INTERRUPTS, NOTIFICATION_BUFFER_INDEXES))
        Error_Handler();
}

void InitDefaultDataTimeSegmentsControllerAreaNetwork(uint32_t operatingMode, uint32_t nominalPrescaler, uint32_t nominalSyncJumpWidth, uint32_t nominalTimeSegment1, uint32_t nominalTimeSegment2)
{
    InitControllerAreaNetwork(operatingMode, nominalPrescaler, nominalSyncJumpWidth, nominalTimeSegment1, nominalTimeSegment2,
    4, 16, 3, 1);
}

void InitControllerAreaNetworkOnInternalLoopbackMode(uint32_t nominalPrescaler, uint32_t nominalSyncJumpWidth, uint32_t nominalTimeSegment1, uint32_t nominalTimeSegment2)
{
    InitDefaultDataTimeSegmentsControllerAreaNetwork(FDCAN_MODE_INTERNAL_LOOPBACK, nominalPrescaler, nominalSyncJumpWidth, nominalTimeSegment1, nominalTimeSegment2);
}

void InitControllerAreaNetworkOnExternalLoopbackMode(uint32_t nominalPrescaler, uint32_t nominalSyncJumpWidth, uint32_t nominalTimeSegment1, uint32_t nominalTimeSegment2)
{
    InitDefaultDataTimeSegmentsControllerAreaNetwork(FDCAN_MODE_EXTERNAL_LOOPBACK, nominalPrescaler, nominalSyncJumpWidth, nominalTimeSegment1, nominalTimeSegment2);
}

void InitControllerAreaNetworkOnNormalMode(uint32_t nominalPrescaler, uint32_t nominalSyncJumpWidth, uint32_t nominalTimeSegment1, uint32_t nominalTimeSegment2)
{
    InitDefaultDataTimeSegmentsControllerAreaNetwork(FDCAN_MODE_NORMAL, nominalPrescaler, nominalSyncJumpWidth, nominalTimeSegment1, nominalTimeSegment2);
}

static void MspInitCallback(FDCAN_HandleTypeDef *hfdcan)
{
    if (FDCAN == hfdcan -> Instance)
    {
        RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitType  = { 0 };
        
        RCC_PeriphCLKInitType.PeriphClockSelection      = RCC_PERIPHCLK_FDCAN;
        RCC_PeriphCLKInitType.FdcanClockSelection       = FDCAN_PERIPH_CLOCK_SOURCE_SELECTION;
        
        HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitType);
        
        RCC_FDCAN_CLOCK_ENABLE();
        
        GPIO_InitTypeDef GPIO_InitType                  = { 0 };
        
        GPIO_InitType.Pin                               = CAN_RX_GPIO_PIN;
        GPIO_InitType.Mode                              = GPIO_MODE_AF_PP;
        GPIO_InitType.Pull                              = GPIO_PULLUP;
        GPIO_InitType.Speed                             = GPIO_SPEED_FAST;
        
        /*
        
        /STM32G474VET6/Product Specifications/stm32g474ve.pdf
        Table 13. Alternate function
        
        Port                                AF9
        
        PA11                                FDCAN1_RX
        PA12                                FDCAN1_TX
        
        */
        GPIO_InitType.Alternate                         = CAN_RX_GPIO_PIN_ALTERNATE_FUNCTION;
        
        
        RCC_CAN_RX_GPIO_PORT_ENABLE();
        HAL_GPIO_Init(CAN_RX_GPIO_PORT, &GPIO_InitType);
        
        GPIO_InitType.Pin                               = CAN_TX_GPIO_PIN;
        GPIO_InitType.Alternate                         = CAN_TX_GPIO_PIN_ALTERNATE_FUNCTION;
        
        RCC_CAN_TX_GPIO_PORT_ENABLE();
        HAL_GPIO_Init(CAN_TX_GPIO_PORT, &GPIO_InitType);
        
#if FDCAN_RX_INTERRUPT_ENABLED

        HAL_NVIC_SetPriority(FDCAN_IT_IRQn, 1, 2);
        HAL_NVIC_EnableIRQ(FDCAN_IT_IRQn);

#endif  /*  #if FDCAN_RX_INTERRUPT_ENABLED  */

    }
}

HAL_StatusTypeDef FDCANTransmitMessage(uint8_t *message, uint32_t length)
{
    FDCAN_TxHeaderTypeDef FDCAN_TxHeaderType            = { 0 };

    FDCAN_TxHeaderType.Identifier                       = FDCAN_TX_HEADER_TYPE_IDENTIFER;
    FDCAN_TxHeaderType.IdType                           = FDCAN_TX_HEADER_TYPE_ID_TYPE;
    FDCAN_TxHeaderType.TxFrameType                      = FDCAN_TX_HEADER_TYPE_TX_FRAME_TYPE;
    FDCAN_TxHeaderType.DataLength                       = length;
    FDCAN_TxHeaderType.ErrorStateIndicator              = FDCAN_ESI_ACTIVE;
    FDCAN_TxHeaderType.BitRateSwitch                    = FDCAN_TX_HEADER_TYPE_BIT_RATE_SWITCH;
    FDCAN_TxHeaderType.FDFormat                         = FDCAN_TX_HEADER_TYPE_FD_FORMAT;
    FDCAN_TxHeaderType.TxEventFifoControl               = FDCAN_TX_HEADER_TYPE_TX_EVENT_FIFO_CONTROL;
    
    /*!< Specifies the message marker to be copied into Tx Event FIFO
                                     element for identification of Tx message status.
                                     This parameter must be a number between 0 and 0xFF                */
    FDCAN_TxHeaderType.MessageMarker                    = 0;
    
    return HAL_FDCAN_AddMessageToTxFifoQ(&FDCAN_HandleType, &FDCAN_TxHeaderType, message);
}

HAL_StatusTypeDef FDCANTransmitFixedLengthMessage(uint8_t *message)
{
    FDCAN_TxHeaderTypeDef FDCAN_TxHeaderType            = { 0 };

    FDCAN_TxHeaderType.Identifier                       = FDCAN_TX_HEADER_TYPE_IDENTIFER;
    FDCAN_TxHeaderType.IdType                           = FDCAN_TX_HEADER_TYPE_ID_TYPE;
    FDCAN_TxHeaderType.TxFrameType                      = FDCAN_TX_HEADER_TYPE_TX_FRAME_TYPE;
    FDCAN_TxHeaderType.DataLength                       = FDCAN_DLC_BYTES_8;
    FDCAN_TxHeaderType.ErrorStateIndicator              = FDCAN_ESI_ACTIVE;
    FDCAN_TxHeaderType.BitRateSwitch                    = FDCAN_TX_HEADER_TYPE_BIT_RATE_SWITCH;
    FDCAN_TxHeaderType.FDFormat                         = FDCAN_TX_HEADER_TYPE_FD_FORMAT;
    FDCAN_TxHeaderType.TxEventFifoControl               = FDCAN_TX_HEADER_TYPE_TX_EVENT_FIFO_CONTROL;
    
    /*!< Specifies the message marker to be copied into Tx Event FIFO
                                     element for identification of Tx message status.
                                     This parameter must be a number between 0 and 0xFF                */
    FDCAN_TxHeaderType.MessageMarker                    = 0;
    
    return HAL_FDCAN_AddMessageToTxFifoQ(&FDCAN_HandleType, &FDCAN_TxHeaderType, message);
}

HAL_StatusTypeDef FDCANReceiveMessage(uint8_t *buffer, uint8_t *length)
{
    FDCAN_RxHeaderTypeDef FDCAN_RxHeaderType            = { 0 };
    
    HAL_StatusTypeDef statusType = HAL_FDCAN_GetRxMessage(&FDCAN_HandleType, FDCAN_GET_MESSAGE_RX_LOCATION, &FDCAN_RxHeaderType, buffer);
    
    *length = FDCAN_RxHeaderType.DataLength;
    
    return statusType;
}

void FDCAN_IT_IRQHANDLER(void)
{
    HAL_FDCAN_IRQHandler(&FDCAN_HandleType);
}