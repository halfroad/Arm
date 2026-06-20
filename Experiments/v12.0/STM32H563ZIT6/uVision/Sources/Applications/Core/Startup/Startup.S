;   https://www.youtube.com/watch?v=kaek8zF0ET0

                            AREA FUNCTIONS, CODE, READONLY

                            EXPORT  DisableExtrnalInterrupts
                            EXPORT  EnableExternalInterrupts

                            EXPORT  WriteRegisters
                            EXPORT  AddRegisters

                            IMPORT  counter
                            IMPORT  addOperands

        ;                   ENTRY
                            
DisableExtrnalInterrupts

                            MRS R0, xPSR
                            ORR R0, R0, #0x08
                            MSR xPSR,   R0

EnableExternalInterrupts

                            MRS R0, xPSR
                            BIC R0, R0, #0x80           ;   Clear the bit #7 to enable the IRQ
                            MSR xPSR, R0

WriteRegisters

                            LDR R1, = counter
                            LDR R0, [R1]
                            
                            ADD R0, R0, #2
                            STR R0, [R1]
                            
                            MOV PC, LR

AddRegisters

                    
                            STR LR, [SP,    #-4]!
                            
                            MOV R0, #1                  ;   Parameter #0 passed by register R0.
                            MOV R1, #2                  ;   Parameter #1 passed by register R1.
                            MOV R2, #3                  ;   Parameter #2 passed by register R2.
                            MOV R3, #4                  ;   Parameter #3 passed by register R3.
                            
                            MOV R4, #5                  ;   Parameter #3 passed by stack.
                            STR R4, [SP,    #-4]!
                            
                            BL  addOperands
                            
                            ADD SP, SP,     #4
                            LDR PC, [SP],   #4

                            END