#include <avr/io.h>
#include <avr/interrupt.h>
#include "ticking.h"

#if defined(ENABLE_TIMER_0)
#if !defined(ATMEGA_TIMER_0)
    #error "Enabled Timer 0, But the hardware may NOT support Timer 0"
#endif

#if !defined(SYSTEM_CLOCK) && !defined(CLOCK_TIMER)
ISR(TIMER0_COMPA_vect){         __timer_func_0();}
#endif

#ifdef SYSTEM_CLOCK
struct __system_clock {
    volatile int64_t timestamp;      // seconds from 1970-1-1 UTC
    volatile uint16_t millisecond;   // milli second now;
} sys_clock;

void sysclock_set(int64_t timestamp, uint16_t millisecond)
{
    __ticking_disable(0);
    if (timestamp) {
        sys_clock.timestamp = timestamp;
    }
    else {
        sysclock_read();
    }
    sys_clock.millisecond = millisecond;
    __ticking_setup(0, 64, ms_ticks(1, 64));
    __ticking_enable(0);
}

void sysclock_save(void)
{   // save the timestamp to eeprom
    eeprom_busy_wait();     // wait until eeprom ready
    eeprom_update_block((void *)(&sys_clock.timestamp),
                        (void *)TICKING_SYS_TIME_ADDR,
                        sizeof(sys_clock.timestamp));
}

void sysclock_read(void)
{   // read the timestamp from eeprom
    eeprom_read_block((void *)(&sys_clock.timestamp),
                      (void *)TICKING_SYS_TIME_ADDR,
                      sizeof(sys_clock.timestamp));
}

void sysclock_clear(void)
{   // read the timestamp from eeprom
    for (uint16_t i = TICKING_SYS_TIME_ADDR; i <= E2END; ++i) {
        eeprom_busy_wait();     // wait until eeprom ready
        eeprom_update_byte((uint8_t *)i, (uint8_t)0);
    }
}

int64_t sysclock_get_s(void)
{
    return sys_clock.timestamp;
}

int16_t sysclock_get_ms(void)
{
    return sys_clock.millisecond;
}
#endif

#ifdef CLOCK_TIMER
struct __clock_timer {
    volatile uint32_t ms_timer;      // the val to save the ms_timer
    volatile uint8_t enabled;       // the enabled flag
} clk_timer[MS_TIMER_NUM];

#ifndef SYSTEM_CLOCK
#define __ms_timer_start(n)                                 \
    do {                                                    \
        clk_timer[n].enabled = 1;                           \
        __ticking_setup(0, 64, ms_ticks(1, 64));            \
        __ticking_enable(0);                                \
    } while (0)
#define __ms_timer_stop(n)                                  \
    do {                                                    \
        clk_timer[n].enabled = 0;                           \
        __ticking_disable(0);                               \
    } while (0)
#else
#define __ms_timer_start(n)                                 \
    do {                                                    \
        clk_timer[n].enabled = 1;                           \
        if (!__ticking_flag(0)) {                            \
            __ticking_setup(0, 64, ms_ticks(1, 64));        \
            __ticking_enable(0);                            \
        }                                                   \
    } while (0)
#define __ms_timer_stop(n)                                  \
    do {                                                    \
        clk_timer[n].enabled = 0;                           \
    } while (0)
#endif
#define __ms_timer_clear(n)                                 \
    do {                                                    \
        clk_timer[n].ms_timer = 0;                          \
    } while (0)
#define __ms_timer_get(n)               (clk_timer[n].ms_timer)

#if MS_TIMER_NUM > 0
void ms_timer_start_0(void)     {__ms_timer_start(0);}
void ms_timer_stop_0(void)      {__ms_timer_stop(0);}
void ms_timer_clear_0(void)     {__ms_timer_clear(0);}
uint32_t ms_timer_get_0(void)   {return __ms_timer_get(0);}
#endif
#if MS_TIMER_NUM > 1
void ms_timer_start_1(void)     {__ms_timer_start(1);}
void ms_timer_stop_1(void)      {__ms_timer_stop(1);}
void ms_timer_clear_1(void)     {__ms_timer_clear(1);}
uint32_t ms_timer_get_1(void)   {return __ms_timer_get(1);}
#endif
#if MS_TIMER_NUM > 2
void ms_timer_start_2(void)     {__ms_timer_start(2);}
void ms_timer_stop_2(void)      {__ms_timer_stop(2);}
void ms_timer_clear_2(void)     {__ms_timer_clear(2);}
uint32_t ms_timer_get_2(void)   {return __ms_timer_get(2);}
#endif
#if MS_TIMER_NUM > 3
void ms_timer_start_3(void)     {__ms_timer_start(3);}
void ms_timer_stop_3(void)      {__ms_timer_stop(3);}
void ms_timer_clear_3(void)     {__ms_timer_clear(3);}
uint32_t ms_timer_get_3(void)   {return __ms_timer_get(3);}
#endif

#endif

#if defined(SYSTEM_CLOCK) || defined(CLOCK_TIMER)
ISR(TIMER0_COMPA_vect)
{
#ifdef SYSTEM_CLOCK
    sys_clock.millisecond++;
    if (sys_clock.millisecond >= 1000) {
        sys_clock.timestamp++;
        sys_clock.millisecond = 0;
    }
#endif
#ifdef CLOCK_TIMER
#if MS_TIMER_NUM > 0
    if (clk_timer[0].enabled) {
        clk_timer[0].ms_timer ++;
    }
#endif
#if MS_TIMER_NUM > 1
    if (clk_timer[1].enabled) {
        clk_timer[1].ms_timer ++;
    }
#endif
#if MS_TIMER_NUM > 2
    if (clk_timer[2].enabled) {
        clk_timer[2].ms_timer ++;
    }
#endif
#if MS_TIMER_NUM > 3
    if (clk_timer[3].enabled) {
        clk_timer[3].ms_timer ++;
    }
#endif
#endif
}
#endif
#endif

#if defined(ENABLE_TIMER_1)
#if !defined(ATMEGA_TIMER_1)
    #error "Enabled Timer 1, But the hardware may NOT support Timer 1"
#endif
ISR(TIMER1_COMPA_vect){         __timer_func_1();}
#endif

#if defined(ENABLE_TIMER_2)
#if !defined(ATMEGA_TIMER_2)
    #error "Enabled Timer 2, But the hardware may NOT support Timer 2"
#endif
ISR(TIMER2_COMPA_vect){         __timer_func_2();}
#endif

#if defined(ENABLE_TIMER_3)
#if !defined(ATMEGA_TIMER_3)
    #error "Enabled Timer 3, But the hardware may NOT support Timer 3"
#endif
ISR(TIMER3_COMPA_vect){         __timer_func_3();}
#endif

#if defined(ENABLE_TIMER_4)
#if !defined(ATMEGA_TIMER_4)
    #error "Enabled Timer 4, But the hardware may NOT support Timer 4"
#endif
ISR(TIMER4_COMPA_vect){         __timer_func_4();}
#endif

#if defined(ENABLE_TIMER_5)
#if !defined(ATMEGA_TIMER_5)
    #error "Enabled Timer 5, But the hardware may NOT support Timer 5"
#endif
ISR(TIMER5_COMPA_vect){         __timer_func_5();}
#endif

