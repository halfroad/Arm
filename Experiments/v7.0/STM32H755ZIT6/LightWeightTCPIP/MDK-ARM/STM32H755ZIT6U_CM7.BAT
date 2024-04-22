SET PATH=D:\Programs\Arm\Keil_v5\ARM\ARMCLANG\Bin;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Users\Àî½ø»Ô\AppData\Local\Microsoft\WindowsApps;
SET CPU_TYPE=STM32H755ZITx:CM7
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=STM32H755ZIT6U_CM7
SET CPU_CLOCK=0x00B71B00
"D:\Programs\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\compilations\cm7\objects\entry_1.__i"
"D:\Programs\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\compilations\cm7\objects\systeminitializer_1.__i"
"D:\Programs\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\compilations\cm7\objects\systeminterrutps.__i"
"D:\Programs\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\compilations\cm7\objects\delay.__i"
"D:\Programs\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\compilations\cm7\objects\gpio.__i"
"D:\Programs\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\compilations\cm7\objects\led.__i"
"D:\Programs\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\compilations\cm7\objects\oled.__i"
"D:\Programs\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\compilations\cm7\objects\system_stm32h7xx_dualcore_boot_cm4_cm7.__i"
"D:\Programs\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\compilations\cm7\objects\startup_stm32h755xx_cm7._ac"
"D:\Programs\Arm\Keil_v5\ARM\ARMCLANG\Bin\ArmLink" --Via ".\Compilations\CM7\Objects\FileSystems_CM7.lnp"
"D:\Programs\Arm\Keil_v5\ARM\ARMCLANG\Bin\fromelf.exe" ".\Compilations\CM7\Objects\FileSystems_CM7.axf" --i32combined --output ".\Compilations\CM7\Objects\FileSystems_CM7.hex"
