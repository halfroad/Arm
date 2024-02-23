#ifndef __UPPER_HOST_COMMUNICATIONS_H
#define __UPPER_HOST_COMMUNICATIONS_H

#include <stm32g4xx.h>

#include "../../../Application/User/Peripherals/Include/PID.h"

#define UPPER_HOST_COMMUNICATIONS_ENABLED                   0

typedef enum
{
    MotorStateIdle                                          = 0x00,
    MotorStateRun                                           = 0x01,
    MotorStateError                                         = 0x02,
    MotorStateRotorLockedOrTimeout                          = 0x03,
    MotorStateBraked                                        = 0x04,
    /* MotorStateStopped                                       = 0x05,  */
    
} MotorStates;

typedef struct
{
    union
    {
        float FloatPIDs[3];
        int8_t IntegerPIDs[12];
        
    } PIDsUnion;
    
} InterrimPIDs;

typedef struct
{
    MotorStates state;
    int16_t velocity;
    int32_t pulseWidthModulation;
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

typedef enum
{
    UpperHostDispatchMotorControlCommandHalt                = 0x01,
    UpperHostDispatchMotorControlCommandRun                 = 0x02,
    UpperHostDispatchMotorControlCommandBrake               = 0x03,
    
} UpperHostDispatchMotorControlCommands;

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

void InitUpperHostCommunications(void);

void ReconcileInitialPIDs(MotorDriveBoardReportTypes motorDriveBoardReportType, float *targetValue, float proportionalFactor, float integralFactor, float derivativeFactor);
void ConfineTargets(float maximum, float minimum, float maximumMutation);
void AcquirePIDs(MotorDriveBoardReportTypes motorDriveBoardReportType, float *proportionalFactor, float *integralFactor, float *derivativeFactor);
void AcquireUpperHostDispatchMotorControlCommands();
uint8_t AcquireUpperHostDispatchedParameters(void);

void ReportCategory(MotorCategories motorCategory);
void ReportState(MotorStates motorState);
void ReportVelocity(float velocity);
void ReportWave(uint8_t channel, int16_t wave);

#endif  /* #ifndef __UPPER_HOST_COMMUNICATIONS_H */