?? 方法一：标准重定向（推荐，代码改动最小）
这是最常用、最灵活的方法，只需要 3 步就能让你的 printf 从 SWO 输出。

第 1 步：修改你的 USART.c 中的 fputc
找到你原来 USART.c 里的 fputc 函数，把里面的 HAL_USART_Transmit 注释掉，换成 ITM_SendChar(ch)。

修改后应该是这样：

c
// USART.c

// 在文件开头加上这个头文件 (core_cm7.h 或 core_cm4.h，STM32H5 是 M7 内核)
#include "core_cm7.h"   // 如果没有，可以试试 #include "stm32h5xx.h"，它通常包含了

int fputc(int ch, FILE *f)
{
    // 原来的 UART 发送 (可以注释掉保留，以后想用还能改回来)
    // HAL_USART_Transmit(&USART_HandleType, (uint8_t *)&ch, 1, RX_TX_DEFAULT_TIMEOUT);

    // 新的 SWV 发送
    ITM_SendChar(ch);
    
    return ch;
}
?? 说明：ITM_SendChar 是 ARM CMSIS 库自带的函数，声明在 core_cm7.h 中。你的 stm32h5xx.h 可能已经间接包含了它，如果编译报错找不到，再手动加上 #include "core_cm7.h" 即可。

第 2 步：配置 Keil 工程选项
关键步骤，不能跳过：

打开 Options for Target (右键项目 → Options for Target 或点击魔法棒图标)

进入 Debug 选项卡

点击你使用的调试器旁的 Settings 按钮

在弹窗中进入 Trace 选项卡，进行以下配置：

? 勾选 Enable (启用 Trace)

Core Clock：填入你 MCU 当前运行的 HCLK 频率 (例如 64MHz、120MHz、250MHz 等，与你的 SystemClock_Config 配置一致，这个值必须填对，否则 SWV 无法工作)

在 ITM Stimulus Ports 区域，? 勾选 Port 0 (因为 ITM_SendChar 默认使用端口 0)

Trace Port：选择 Serial Wire Viewer (SWV)

SWO 时钟：可以保持默认或设为 2-4 MHz

第 3 步：打开 Debug (printf) Viewer 并运行
按 Ctrl + F5 或点击 Start/Stop Debug Session 进入调试模式

在 Keil 菜单栏找到 View → Serial Windows → Debug (printf) Viewer，点击打开

全速运行程序 (按 F5)，你就会在 Debug (printf) Viewer 窗口中看到你的 printf 输出了！




? 方法二：使用 RTE 组件（最省事，自动配置）
如果不想手写 fputc，Keil 提供了一个现成的配置文件。

打开 Manage Run-Time Environment 窗口 (点击工具栏上那个像电路板的图标)

依次展开 Compiler → I/O → STDOUT

选择 ITM (而不是 EVR 或 User)

点击 OK 关闭窗口，RTE 会自动将 retarget_io.c 文件添加到你的工程中，并配置好 fputc

仍然需要按照方法一的第 2 步配置 Trace 选项，然后就可以正常使用了

?? 常见问题排查
如果看不到输出，按以下顺序检查：

SWO 线连接了吗？ — STM32H563ZI 的 SWO 引脚是 PB3，确保你的调试器 (ST-Link/J-Link) 连接了该引脚。板载 ST-Link 通常是自动连接的，检查跳线帽。

Core Clock 填对了吗？ — 这个最容易出错。填写的数值必须和代码中 SystemClock_Config 配置的 HCLK 完全一致。

ITM 端口 0 勾选了吗？ — 必须勾选 Port 0，因为 ITM_SendChar 默认使用端口 0。

调试器是否选中了 SWD 模式？ — 在 Debug → Settings → Debug 选项卡中，Port 应选择 SW，而不是 JTAG。如果选成 JTAG，SWO 功能不可用。

代码中 fputc 真的被调用了吗？ — 在 ITM_SendChar(ch); 前加一个断点，运行看是否会停在这里，确认重定向是否生效。