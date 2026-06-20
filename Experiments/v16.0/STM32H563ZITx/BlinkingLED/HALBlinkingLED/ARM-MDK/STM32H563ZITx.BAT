SET PATH=d:\Program Files\Arm\Keil_v5\ARM\ARMCLANG\Bin;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\WINDOWS\System32\OpenSSH\;C:\Users\???\AppData\Local\Microsoft\WindowsApps
SET ARMCC6_ASMOPT=--diag_suppress=9931
SET ARMCC6_CCOPT=--diag_suppress=9931
SET ARMCC6_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=STM32H563ZITx
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=STM32H563ZITx
SET CPU_CLOCK=0x00B71B00
"d:\Program Files\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\outputs\objects\system_stm32h5xx.__i"
"d:\Program Files\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\outputs\objects\startup_stm32h563xx._ac"
"d:\Program Files\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\outputs\objects\stm32h5xx_hal.__i"
"d:\Program Files\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\outputs\objects\stm32h5xx_hal_cortex.__i"
"d:\Program Files\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\outputs\objects\stm32h5xx_hal_gpio.__i"
"d:\Program Files\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\outputs\objects\stm32h5xx_hal_pwr.__i"
"d:\Program Files\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\outputs\objects\stm32h5xx_hal_pwr_ex.__i"
"d:\Program Files\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\outputs\objects\stm32h5xx_hal_rcc.__i"
"d:\Program Files\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\outputs\objects\stm32h5xx_hal_rcc_ex.__i"
"d:\Program Files\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\outputs\objects\main.__i"
"d:\Program Files\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmLink" --Via ".\Outputs\Objects\BlinkingLED.lnp"
"d:\Program Files\Arm\Keil_v5\ARM\ARMCLANG\Bin\fromelf.exe" ".\Outputs\Objects\BlinkingLED.axf" --i32combined --output ".\Outputs\Objects\BlinkingLED.hex"
