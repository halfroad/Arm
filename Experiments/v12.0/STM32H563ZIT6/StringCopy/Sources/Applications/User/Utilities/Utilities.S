

            ;   String Copy

                            AREA    Utilities,  CODE,   READONLY

                            EXPORT  CopyString

CopyString

                            ;   R0 points towards the destionation address.
                            ;   R1 points towards the source address.

                            LDRB    R2, [R1],   #1
                            STRB    R2, [R0],   #1
                            
                            CMP     R2, #0
                            
                            BNE     CopyString
                            
                            MOV     PC, LR
                            
                            END