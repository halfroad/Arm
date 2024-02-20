#include <string.h>
#include <stdlib.h>

#include "../../../Application/User/Peripherals/Include/SerialCommunications.h"
#include "../../../Application/User/Utils/Include/CyclicRedundancyCheck.h"
#include "../Include/UpperHostCommunications.h"

#define COMMUNICATIONS_ENABLED                              1
#define CRC_ENABLED                                         1

#define MOTOR_CONTROLS_PROTOCOL_HEAD                        0xC5
#define MOTOR_CONTROLS_PROTOCOL_TAIL                        0x5C

#define MOTOR_CONTROLS_PROTOCOL_ERROR                       0xFE

#define MAXIMUM_RECEIVE_BUFFER_LENGTH                       17

typedef struct
{
    union
    {
        float outgoingPIDs[3];
        int8_t incomingPIDs[12];
        
    } PIDsUnion;
    
} InterrimPIDs;

typedef struct
{
    uint8_t state;
    int16_t velocity;
    uint64_t travelDistance;
    uint8_t motorCategory;
    float torque;
    float power;
    
    uint8_t hallSensorLocation;
    uint16_t encoderLocation;
    
    float voltage;
    float currents[3];
    float temperatures[2];
    float counterElectromotiveForces[3];
    
    InterrimPIDs PIDs[10];
    int16_t waves[16];
    
} MotorControlProtocol;

typedef struct
{
    uint8_t controlDirective;
    uint8_t modeDirective;
    
    float *velocity;
    float *torque;
    float PIDs[3];
    
} UpperHostDispatchedParameters;

typedef enum
{
    MotorCategoryDirectCurrentMotor                         = 0x10,
    MotorCategoryBrushedlessDirectCurrentMotor              = 0x11,
    MotorCategoryPermanentMagnetSynchronousMotor            = 0x12,
    MotorCategoryStepMotor                                  = 0x13,
    MotorCategoryServoMotor                                 = 0x14,
    MotorCategoryTriplePhaseAsynchronousMotor               = 0x15,
    MotorCategorySteeringEngineServoMotor                   = 0x16,
    
    
} MotorCategories;

typedef enum
{
    MotorStateIdle                                          = 0x00,
    MotorStateRun                                           = 0x01,
    MotorStateError                                         = 0x02,
    MotorStateRotorLockedOrTimeout                          = 0x03,
    MotorStateBraked                                        = 0x04,
    
} MotorStates;

typedef enum
{
    MotorDriveBoardReportTypeError                          = 0x0F,
    
    MotorDriveBoardReportTypeState                          = 0x10,
    MotorDriveBoardReportTypeVelocity                       = 0x11,
    MotorDriveBoardReportTypeHallSensoEncoderLocation       = 0x12,
    MotorDriveBoardReportTypeVoltageBus                     = 0x13,
    MotorDriveBoardReportTypeCurrents                       = 0x14,
    MotorDriveBoardReportTypeTemperatures                   = 0x15,
    MotorDriveBoardReportTypeTravelDistance                 = 0x16,
    MotorDriveBoardReportTypeCounterElectromotiveForces     = 0x17,
    MotorDriveBoardReportTypeMotorCategory                  = 0x18,
    MotorDriveBoardReportTypeMotorTorque                    = 0x19,
    MotorDriveBoardReportTypePower                          = 0x1A,
    
    MotorDriveBoardReportTypePID1                           = 0x20,
    MotorDriveBoardReportTypePID2                           = 0x21,
    MotorDriveBoardReportTypePID3                           = 0x22,
    MotorDriveBoardReportTypePID4                           = 0x23,
    MotorDriveBoardReportTypePID5                           = 0x24,
    MotorDriveBoardReportTypePID6                           = 0x25,
    MotorDriveBoardReportTypePID7                           = 0x26,
    MotorDriveBoardReportTypePID8                           = 0x27,
    MotorDriveBoardReportTypePID9                           = 0x28,
    MotorDriveBoardReportTypePID10                          = 0x29,
    
    MotorDriveBoardReportTypeWaves                          = 0x30,
    MotorDriveBoardReportTypeCustom                         = 0x31,
    
} MotorDriveBoardReportTypes;


typedef enum
{
    UpperHostDispatchMotorCommandAcquireAllParameters       = 0x19,
    UpperHostDispatchMotorCommandControlDirective           = 0x21,
    UpperHostDispatchMotorCommandModeDirective              = 0x22,
    UpperHostDispatchMotorCommandSetVelocity                = 0x23,
    UpperHostDispatchMotorCommandSetTorque                  = 0x24,
    
    UpperHostDispatchMotorCommandSetPID1                    = 0x31,
    UpperHostDispatchMotorCommandSetPID2                    = 0x32,
    UpperHostDispatchMotorCommandSetPID3                    = 0x33,
    UpperHostDispatchMotorCommandSetPID4                    = 0x34,
    UpperHostDispatchMotorCommandSetPID5                    = 0x35,
    UpperHostDispatchMotorCommandSetPID6                    = 0x36,
    UpperHostDispatchMotorCommandSetPID7                    = 0x37,
    UpperHostDispatchMotorCommandSetPID8                    = 0x38,
    UpperHostDispatchMotorCommandSetPID9                    = 0x39,
    UpperHostDispatchMotorCommandSetPID10                   = 0x4A,
    
} UpperHostDispatchMotorCommands;

__IO uint8_t receivedBytes[MAXIMUM_RECEIVE_BUFFER_LENGTH];
__IO uint8_t receivedBytesAddressOffset = 0;

MotorControlProtocol motorControlProtocol = { 0 };
UpperHostDispatchedParameters upperHostDispatchedParameters = { 0 };

static void InitMotorControlProtocol(MotorControlProtocol *motorControlProtocol);
static void HandleUpperHostPayload(uint8_t *payload);
static HAL_StatusTypeDef Report(MotorControlProtocol *protocol, MotorDriveBoardReportTypes motorDriveBoardReportType);

void InitUpperHostCommunications(void)
{
    InitSerialCommunications(115200, HandleUpperHostPayload);
    InitMotorControlProtocol(&motorControlProtocol);
}

static void InitMotorControlProtocol(MotorControlProtocol *motorControlProtocol)
{
    size_t size = sizeof(MotorControlProtocol);
    
    memset(motorControlProtocol, 0, size);
}

static void HandleUpperHostPayload(uint8_t *payload)
{
    uint8_t bytes[MAXIMUM_RECEIVE_BUFFER_LENGTH];
    
    if (receivedBytesAddressOffset >= MAXIMUM_RECEIVE_BUFFER_LENGTH)
        receivedBytesAddressOffset = 0;
    
    receivedBytes[receivedBytesAddressOffset] = *payload;
    
    if (*payload == MOTOR_CONTROLS_PROTOCOL_TAIL)
    {
        if (receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 4) % MAXIMUM_RECEIVE_BUFFER_LENGTH] == MOTOR_CONTROLS_PROTOCOL_HEAD)
        {
            for (uint8_t i = 0; i < 2; i ++)
                bytes[i] = receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 4 + i) % MAXIMUM_RECEIVE_BUFFER_LENGTH];
            
#ifdef CRC_ENABLED
            
            if (CRC16Validate(bytes, 2) == (receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 4 + 2) % MAXIMUM_RECEIVE_BUFFER_LENGTH] << 8 |
                receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 4 + 3) % MAXIMUM_RECEIVE_BUFFER_LENGTH]))
#endif
            {
                if (receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 4 + 1) % MAXIMUM_RECEIVE_BUFFER_LENGTH] == UpperHostDispatchMotorCommandAcquireAllParameters)
                {
                    Report(&motorControlProtocol, MotorDriveBoardReportTypeState);
                    Report(&motorControlProtocol, MotorDriveBoardReportTypeVelocity);
                    Report(&motorControlProtocol, MotorDriveBoardReportTypeHallSensoEncoderLocation);
                    Report(&motorControlProtocol, MotorDriveBoardReportTypeVoltageBus);
                    Report(&motorControlProtocol, MotorDriveBoardReportTypeCurrents);
                    Report(&motorControlProtocol, MotorDriveBoardReportTypeTemperatures);
                    Report(&motorControlProtocol, MotorDriveBoardReportTypeTravelDistance);
                    Report(&motorControlProtocol, MotorDriveBoardReportTypeCounterElectromotiveForces);
                    Report(&motorControlProtocol, MotorDriveBoardReportTypeMotorCategory);
                    
                    for (uint8_t i = MotorDriveBoardReportTypePID1; i < MotorDriveBoardReportTypePID10; i ++)
                        Report(&motorControlProtocol, i);
                }
            }
        }
        else if (receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 5) % MAXIMUM_RECEIVE_BUFFER_LENGTH] == MOTOR_CONTROLS_PROTOCOL_HEAD)
        {
            for (uint8_t i = 0; i < 3; i ++)
                bytes[i] = receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 5 + i) % MAXIMUM_RECEIVE_BUFFER_LENGTH];
            
#ifdef CRC_ENABLED
            
            if (CRC16Validate(bytes, 3) == (receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 5 + 3) % MAXIMUM_RECEIVE_BUFFER_LENGTH] << 8 |
                receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 5 + 4) % MAXIMUM_RECEIVE_BUFFER_LENGTH]))
#endif
            {
                switch (receivedBytes[(receivedBytesAddressOffset - 5 + 1) % MAXIMUM_RECEIVE_BUFFER_LENGTH])
                {
                    case UpperHostDispatchMotorCommandControlDirective:
                        upperHostDispatchedParameters.controlDirective = receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 5 + 2) % MAXIMUM_RECEIVE_BUFFER_LENGTH];
                        break;
                    
                    case UpperHostDispatchMotorCommandModeDirective:
                        upperHostDispatchedParameters.modeDirective = receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 5 + 2) % MAXIMUM_RECEIVE_BUFFER_LENGTH];
                        break;
                }
            }
        }
        else if (receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 6) % MAXIMUM_RECEIVE_BUFFER_LENGTH] == MOTOR_CONTROLS_PROTOCOL_HEAD)
        {
            for (uint8_t i = 0; i < 4; i ++)
                bytes[i] = receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 6 + i) % MAXIMUM_RECEIVE_BUFFER_LENGTH];
            
#ifdef CRC_ENABLED
            
            if (CRC16Validate(bytes, 4) == (receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 6 + 4) % MAXIMUM_RECEIVE_BUFFER_LENGTH] << 8 |
                receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 6 + 5) % MAXIMUM_RECEIVE_BUFFER_LENGTH]))
#endif
            {
                switch (receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 6 + 1) % MAXIMUM_RECEIVE_BUFFER_LENGTH])
                {
                    case UpperHostDispatchMotorCommandSetVelocity:
                        *(upperHostDispatchedParameters.velocity) = receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 6 + 2) % MAXIMUM_RECEIVE_BUFFER_LENGTH] << 8 | receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 6 + 3) % MAXIMUM_RECEIVE_BUFFER_LENGTH];
                        break;
                    
                    case UpperHostDispatchMotorCommandSetTorque:
                        *(upperHostDispatchedParameters.torque) = receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 6 + 2) % MAXIMUM_RECEIVE_BUFFER_LENGTH] << 8 | receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 6 + 3) % MAXIMUM_RECEIVE_BUFFER_LENGTH];
                        break;
                }
            }
        }
        else if (receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 16) % MAXIMUM_RECEIVE_BUFFER_LENGTH] == MOTOR_CONTROLS_PROTOCOL_HEAD)
        {
            for (uint8_t i = 0; i < 14; i ++)
                bytes[i] = receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 16 + i) % MAXIMUM_RECEIVE_BUFFER_LENGTH];
            
#ifdef CRC_ENABLED
            
            if (CRC16Validate(bytes, 14) == (receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 16 + 14) % MAXIMUM_RECEIVE_BUFFER_LENGTH] << 8 |
                receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 16 + 15) % MAXIMUM_RECEIVE_BUFFER_LENGTH]))
            {
                switch (receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 16 + 1) % MAXIMUM_RECEIVE_BUFFER_LENGTH])
                {
                    case UpperHostDispatchMotorCommandSetPID1:
                    case UpperHostDispatchMotorCommandSetPID2:
                    case UpperHostDispatchMotorCommandSetPID3:
                    case UpperHostDispatchMotorCommandSetPID4:
                    case UpperHostDispatchMotorCommandSetPID5:
                    case UpperHostDispatchMotorCommandSetPID6:
                    case UpperHostDispatchMotorCommandSetPID7:
                    case UpperHostDispatchMotorCommandSetPID8:
                    case UpperHostDispatchMotorCommandSetPID9:
                    case UpperHostDispatchMotorCommandSetPID10:
                    {
                        for (uint8_t i = 0; i < 12; i ++)
                            motorControlProtocol.PIDs[receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 16 + 1) % MAXIMUM_RECEIVE_BUFFER_LENGTH] - UpperHostDispatchMotorCommandSetPID1].PIDsUnion.incomingPIDs[i] = receivedBytes[(receivedBytesAddressOffset + MAXIMUM_RECEIVE_BUFFER_LENGTH - 16 + 2 + i) % MAXIMUM_RECEIVE_BUFFER_LENGTH];
                    }
                        break;
                }
            }
#endif
        }
    }
    
    receivedBytesAddressOffset ++;
}

static HAL_StatusTypeDef Report(MotorControlProtocol *protocol, MotorDriveBoardReportTypes motorDriveBoardReportType)
{
    uint8_t report[37];
    
    report[0] = MOTOR_CONTROLS_PROTOCOL_HEAD;
    report[1] = motorDriveBoardReportType;
    
    uint8_t cursor = 2;
    
    switch (motorDriveBoardReportType)
    {
        case MotorDriveBoardReportTypeState:
            
            report[cursor ++] = protocol -> state;
        
            break;
        
        case MotorDriveBoardReportTypeVelocity:
            
            report[cursor ++] = (protocol -> velocity >> 8) & 0xFF;
            report[cursor ++] = protocol -> velocity & 0xFF;
        
            break;
        
        case MotorDriveBoardReportTypeHallSensoEncoderLocation:
            
            report[cursor ++] = protocol -> hallSensorLocation & 0x08;
            report[cursor ++] = (protocol -> encoderLocation >> 8) & 0xFF;
            report[cursor ++] = protocol -> encoderLocation & 0xFF;
        
            break;
        
        case MotorDriveBoardReportTypeVoltageBus:
            
            report[cursor ++] = ((uint8_t)protocol -> voltage) % 101;
            report[cursor ++] = ((uint16_t)protocol -> voltage * 100) % 100;
        
            break;
        
        case MotorDriveBoardReportTypeCurrents:
            
            report[cursor ++] = (((int16_t)protocol -> currents[0] * 1000) >> 8) & 0xFF;
            report[cursor ++] = ((int16_t)protocol -> currents[0] * 1000) & 0xFF;
        
            report[cursor ++] = (((int16_t)protocol -> currents[1] * 1000) >> 8) & 0xFF;
            report[cursor ++] = ((int16_t)protocol -> currents[1] * 1000) & 0xFF;
        
            report[cursor ++] = (((int16_t)protocol -> currents[2] * 1000) >> 8) & 0xFF;
            report[cursor ++] = ((int16_t)protocol -> currents[2] * 1000) & 0xFF;
        
            break;
        
        case MotorDriveBoardReportTypeTemperatures:
            
            report[cursor ++] = (uint8_t)(protocol -> temperatures[0] + 50);
            report[cursor ++] = (uint8_t)(protocol -> temperatures[1] + 50);
        
            break;
        
        case MotorDriveBoardReportTypeTravelDistance:
            
            report[cursor ++] = (protocol -> travelDistance >> 56) & 0xFF;
            report[cursor ++] = (protocol -> travelDistance >> 48) & 0xFF;
            report[cursor ++] = (protocol -> travelDistance >> 40) & 0xFF;
            report[cursor ++] = (protocol -> travelDistance >> 32) & 0xFF;
            report[cursor ++] = (protocol -> travelDistance >> 24) & 0xFF;
            report[cursor ++] = (protocol -> travelDistance >> 16) & 0xFF;
            report[cursor ++] = (protocol -> travelDistance >> 8) & 0xFF;
            report[cursor ++] = (protocol -> travelDistance >> 0) & 0xFF;
        
            break;
        
        case MotorDriveBoardReportTypeCounterElectromotiveForces:
            
            report[cursor ++] = (int8_t)(protocol -> counterElectromotiveForces[0]);
            report[cursor ++] = (int16_t)(protocol -> counterElectromotiveForces[0] * 100) % 100;
        
            report[cursor ++] = (int8_t)(protocol -> counterElectromotiveForces[1]);
            report[cursor ++] = (int16_t)(protocol -> counterElectromotiveForces[1] * 100) % 100;
        
            report[cursor ++] = (int8_t)(protocol -> counterElectromotiveForces[2]);
            report[cursor ++] = (int16_t)(protocol -> counterElectromotiveForces[2] * 100) % 100;
        
            break;
        
        case MotorDriveBoardReportTypeMotorCategory:
            
            report[cursor ++] = protocol -> motorCategory;
        
            break;
        
        case MotorDriveBoardReportTypeMotorTorque:
            
            report[cursor ++] = (((int16_t)protocol -> torque * 1000) >> 8) & 0xFF;
            report[cursor ++] = ((int16_t)protocol -> torque * 1000) & 0xFF;
        
            break;
        
        case MotorDriveBoardReportTypePower:
            
            report[cursor ++] = (((int16_t)protocol -> power * 1000) >> 8) & 0xFF;
            report[cursor ++] = ((int16_t)protocol -> power * 1000) & 0xFF;
        
            break;
        
        case MotorDriveBoardReportTypePID1:
        case MotorDriveBoardReportTypePID2:
        case MotorDriveBoardReportTypePID3:
        case MotorDriveBoardReportTypePID4:
        case MotorDriveBoardReportTypePID5:
        case MotorDriveBoardReportTypePID6:
        case MotorDriveBoardReportTypePID7:
        case MotorDriveBoardReportTypePID8:
        case MotorDriveBoardReportTypePID9:
        case MotorDriveBoardReportTypePID10:
        {
            for (uint8_t i = 0; i < 3; i ++)
            {
                report[cursor ++] = protocol -> PIDs[motorDriveBoardReportType - MotorDriveBoardReportTypePID1].PIDsUnion.incomingPIDs[i * 4 + 0];
                report[cursor ++] = protocol -> PIDs[motorDriveBoardReportType - MotorDriveBoardReportTypePID1].PIDsUnion.incomingPIDs[i * 4 + 1];
                report[cursor ++] = protocol -> PIDs[motorDriveBoardReportType - MotorDriveBoardReportTypePID1].PIDsUnion.incomingPIDs[i * 4 + 2];
                report[cursor ++] = protocol -> PIDs[motorDriveBoardReportType - MotorDriveBoardReportTypePID1].PIDsUnion.incomingPIDs[i * 4 + 3];
            }
        }
            break;
        
        case MotorDriveBoardReportTypeWaves:
            
            for (uint8_t i = 0; i < 16; i ++)
            {
                report[cursor ++] = (protocol -> waves[i] >> 8) & 0xFF;
                report[cursor ++] = protocol -> waves[i] & 0xFF;
            }
        
            break;
        
        default:
            
            report[1] = MOTOR_CONTROLS_PROTOCOL_ERROR;
        
            break;
    }
    
    if (report[1] == MOTOR_CONTROLS_PROTOCOL_ERROR)
        
        return HAL_ERROR;
    
    else
    {
        uint16_t crc = CRC16Validate(&report[0], cursor);
        
        report[cursor ++] = (crc >> 8) & 0xFF;
        report[cursor ++] = crc & 0xFF;
        
        report[cursor ++] = MOTOR_CONTROLS_PROTOCOL_TAIL;
        
        HAL_StatusTypeDef HAL_StatusType = tranmist(report, cursor);
        
        return HAL_StatusType;
    }
}

void ConfineTargets(float maximum, float minimum, float maximumMutation)
{
    static float mutation = 0.0;
    
    if (abs((int32_t)(* upperHostDispatchedParameters.velocity - mutation)) > maximumMutation)
        *upperHostDispatchedParameters.velocity = mutation;
    
    mutation = *upperHostDispatchedParameters.velocity;
    
    if (*upperHostDispatchedParameters.velocity >= maximum)
        *upperHostDispatchedParameters.velocity = maximum;
    
    if (*upperHostDispatchedParameters.velocity <= minimum)
        *upperHostDispatchedParameters.velocity = minimum;
}

void ReconcileInitialPIDs(MotorDriveBoardReportTypes motorDriveBoardReportType, float *targetValue, float proportionalFactor, float integralFactor, float derivativeFactor)
{
    upperHostDispatchedParameters.velocity = (float *) targetValue;
    
    motorControlProtocol.PIDs[motorDriveBoardReportType - MotorDriveBoardReportTypePID1].PIDsUnion.outgoingPIDs[0] = proportionalFactor;
    motorControlProtocol.PIDs[motorDriveBoardReportType - MotorDriveBoardReportTypePID1].PIDsUnion.outgoingPIDs[1] = integralFactor;
    motorControlProtocol.PIDs[motorDriveBoardReportType - MotorDriveBoardReportTypePID1].PIDsUnion.outgoingPIDs[2] = derivativeFactor;
    
    Report(&motorControlProtocol, motorDriveBoardReportType);
}

void ReportCurrents(float phaseUCurrent, float phaseVCurrent, float phaseWCurrent)
{
    motorControlProtocol.currents[0] = phaseUCurrent;
    motorControlProtocol.currents[1] = phaseVCurrent;
    motorControlProtocol.currents[2] = phaseWCurrent;
    
    Report(&motorControlProtocol, MotorDriveBoardReportTypeCurrents);
}

void ReportVoltage(float voltage)
{
    motorControlProtocol.voltage = voltage;
    
    Report(&motorControlProtocol, MotorDriveBoardReportTypeVoltageBus);
}

void ReportPower(float power)
{
    motorControlProtocol.power = power;
    
    Report(&motorControlProtocol, MotorDriveBoardReportTypePower);
}

void ReportState(MotorStates motorState)
{
    motorControlProtocol.state = motorState;
    
    Report(&motorControlProtocol, MotorDriveBoardReportTypeState);
}

void ReportVolecity(float velocity)
{
    motorControlProtocol.velocity = velocity;
    
    Report(&motorControlProtocol, MotorDriveBoardReportTypeVelocity);
}

void ReportTravelDistance(float travelDistance)
{
    motorControlProtocol.travelDistance = travelDistance;
    
    Report(&motorControlProtocol, MotorDriveBoardReportTypeTravelDistance);
}

void ReportTemperatures(float motorTemperature, float boardTemperature)
{
    motorControlProtocol.temperatures[0] = motorTemperature;
    motorControlProtocol.temperatures[1] = boardTemperature;
    
    Report(&motorControlProtocol, MotorDriveBoardReportTypeTemperatures);
}

void ReportCategory(MotorCategories motorCategory)
{
    motorControlProtocol.motorCategory = motorCategory;
    
    Report(&motorControlProtocol, MotorDriveBoardReportTypeMotorCategory);
}

void ReportWaves(uint8_t channel, int16_t wave)
{
    motorControlProtocol.waves[channel - 1] = wave;
    
    Report(&motorControlProtocol, MotorDriveBoardReportTypeWaves);
}

void AcquirePIDs(MotorDriveBoardReportTypes motorDriveBoardReportType, float *proportionalFactor, float *integralFactor, float *derivativeFactor)
{
    *proportionalFactor = motorControlProtocol.PIDs[motorDriveBoardReportType - UpperHostDispatchMotorCommandSetPID1].PIDsUnion.incomingPIDs[0];
    *integralFactor = motorControlProtocol.PIDs[motorDriveBoardReportType - UpperHostDispatchMotorCommandSetPID1].PIDsUnion.incomingPIDs[1];
    *derivativeFactor = motorControlProtocol.PIDs[motorDriveBoardReportType - UpperHostDispatchMotorCommandSetPID1].PIDsUnion.incomingPIDs[2];
}

uint8_t AcquireUpperHostDispatchedParameters(void)
{
    static uint8_t receivedNumber = 0;
    
    if (upperHostDispatchedParameters.controlDirective >= 0x01 && upperHostDispatchedParameters.controlDirective <= 0x03)
    {
        receivedNumber ++;
        
        if (receivedNumber >= 2)
        {
            receivedNumber = 0;
            
            upperHostDispatchedParameters.controlDirective = 0;
        }
        
        return upperHostDispatchedParameters.controlDirective;
    }
    
    return 0;
}


