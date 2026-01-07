#include "delay.h"
#include "core_cm3.h"  // 或 core_cm4.h，根据内核选择

// 注意：STM32F1 使用 Cortex-M3，头文件为 core_cm3.h

static uint32_t fac_us = 0;  // 微秒因子
static uint32_t fac_ms = 0;  // 毫秒因子（可选）

/**
 * @brief 初始化 delay 模块
 *        必须在 SystemClock_Config() 之后调用！
 */
void delay_init(void)
{
    // 启用 DWT 寄存器访问（必须！）
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    
    // 重置 DWT 计数器
    DWT->CYCCNT = 0;
    
    // 启用 DWT 周期计数器
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    
    // 计算微秒因子：SystemCoreClock / 1000000
    fac_us = SystemCoreClock / 1000000U;
    fac_ms = SystemCoreClock / 1000U;   // 可用于 ms 计算（但通常直接用 us*1000）
}

/**
 * @brief 微秒级延时（精确到 1us，适用于 1us ~ 几十万 us）
 * @param us: 延时时间（微秒）
 */
void delay_us(uint32_t us)
{
    if (us == 0) return;
    
    uint32_t start = DWT->CYCCNT;
    // 等待 CYCCNT - start >= us * fac_us
    while ((DWT->CYCCNT - start) < us * fac_us);
}

/**
 * @brief 毫秒级延时（基于 delay_us 实现，简单可靠）
 * @param ms: 延时时间（毫秒）
 */
void delay_ms(uint32_t ms)
{
    // 避免大循环导致看门狗复位（若使用 WDG）
    while (ms--)
    {
        delay_us(1000);
    }
}