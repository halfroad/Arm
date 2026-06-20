SET PATH=d:\Program Files\Arm\Keil_v5\ARM\ARMCLANG\Bin;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Users\???\AppData\Local\Microsoft\WindowsApps;
SET ARMCC6_ASMOPT=--diag_suppress=9931
SET ARMCC6_CCOPT=--diag_suppress=9931
SET ARMCC6_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=STM32H563ZITx
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=STM32H563ZIT6
SET CPU_CLOCK=0x00B71B00
"d:\Program Files\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\compilations\objects\startup_stm32h563xx._ac"
"d:\Program Files\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\compilations\objects\startup._ac"
"d:\Program Files\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\compilations\objects\main.__i"
"d:\Program Files\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmLink" --Via ".\Compilations\Objects\uVision.lnp"
"d:\Program Files\Arm\Keil_v5\ARM\ARMCLANG\Bin\fromelf.exe" ".\Compilations\Objects\uVision.axf" --i32combined --output ".\Compilations\Objects\uVision.hex"
