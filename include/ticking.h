#ifndef __AVR_TICKING__
#define __AVR_TICKING__

#include "__ticking_arch.h"

#define ENABLE_TIMER_0      // enable the timer 0
#define ENABLE_TIMER_1      // enable the timer 1
#define ENABLE_TIMER_2      // enable the timer 2
//#define ENABLE_TIMER_3      // enable the timer 3
//#define ENABLE_TIMER_4      // enable the timer 4
//#define ENABLE_TIMER_5      // enable the timer 5
//#define ENABLE_TIMER_6      // enable the timer 6
#define SYSTEM_CLOCK        // use timer 0 for system clock
#define CLOCK_TIMER         // use timer 0 for ms timer
#ifdef CLOCK_TIMER
#define MS_TIMER_NUM 4
#endif

#define ms_ticks(ms, prescaler) \
    ((unsigned int)(F_CPU / 1000UL / (prescaler) * (ms)))

#define ticking_start(n, prescaler, ticks, func) __ticking_start(n, prescaler, ticks, func)
#define ticking_stop(n)                          __ticking_stop(n)

#ifdef ENABLE_TIMER_0
    #if !defined(SYSTEM_CLOCK) && !defined(CLOCK_TIMER)
        void (* __timer_func_0)();
    #endif
    #ifdef SYSTEM_CLOCK
        #include <avr/eeprom.h>
        #define TICKING_SYS_TIME_ADDR (E2END - 16)
        void sysclock_set(int64_t timestamp, uint16_t millisecond);
        void sysclock_save(void);
        void sysclock_read(void);
        void sysclock_clear(void);
        int64_t sysclock_get_s(void);
        int16_t sysclock_get_ms(void);
    #endif
    #ifdef CLOCK_TIMER
        #if MS_TIMER_NUM > 4
            #error "Max support 4 ms_timer"
        #endif
        #if MS_TIMER_NUM > 0
            void ms_timer_start_0(void);
            void ms_timer_stop_0(void);
            void ms_timer_clear_0(void);
            uint32_t ms_timer_get_0(void);
        #endif
        #if MS_TIMER_NUM > 1
            void ms_timer_start_1(void);
            void ms_timer_stop_1(void);
            void ms_timer_clear_1(void);
            uint32_t ms_timer_get_1(void);
        #endif
        #if MS_TIMER_NUM > 2
            void ms_timer_start_2(void);
            void ms_timer_stop_2(void);
            void ms_timer_clear_2(void);
            uint32_t ms_timer_get_2(void);
        #endif
        #if MS_TIMER_NUM > 3
            void ms_timer_start_3(void);
            void ms_timer_stop_3(void);
            void ms_timer_clear_3(void);
            uint32_t ms_timer_get_3(void);
        #endif
        #define ms_timer_start(n)  ms_timer_start_ ## n()
        #define ms_timer_stop(n)   ms_timer_stop_ ## n()
        #define ms_timer_clear(n)  ms_timer_clear_ ## n()
        #define ms_timer_get(n)    ms_timer_get_ ## n()
    #endif
#endif

#ifdef ENABLE_TIMER_1
void (* __timer_func_1)();
#endif

#ifdef ENABLE_TIMER_2
void (* __timer_func_2)();
#endif

#ifdef ENABLE_TIMER_3
void (* __timer_func_3)();
#endif

#ifdef ENABLE_TIMER_4
void (* __timer_func_4)();
#endif

#ifdef ENABLE_TIMER_5
void (* __timer_func_5)();
#endif

#define TIMER_SET_CTC(n)        __TIMER_SET_CTC_ ## n
#define __TIMER_SET_CTC_8_Bit(n)                                \
    TIMER_CONTROL(n, A) &= ~(1 << TIMER_WGM(n, 0));             \
    TIMER_CONTROL(n, A) |= (1 << TIMER_WGM(n, 1));
#define __TIMER_SET_CTC_16_Bit(n)                               \
    TIMER_CONTROL(n, A) &= ~(1 << TIMER_WGM(n, 0));             \
    TIMER_CONTROL(n, A) &= ~(1 << TIMER_WGM(n, 1));             \
    TIMER_CONTROL(n, B) |= (1 << TIMER_WGM(n, 2));              \
    TIMER_CONTROL(n, B) &= ~(1 << TIMER_WGM(n, 3));


// Functions for the timers
#define __timer_func(n)         \
    __timer_func_ ## n      /* timer function */
#define __ticking_disable(n)    \
    TIMER_INTRP_MASK(n) &= ~(1 << TIMER_CTC_INTRP(n))   /* disable timer interrupt first */
#define __ticking_enable(n)     \
    TIMER_INTRP_MASK(n) |= (1 << TIMER_CTC_INTRP(n))    /* enable the timer interrupt */
#define __ticking_flag(n)      \
    ((TIMER_INTRP_MASK(n) >> TIMER_CTC_INTRP(n)) & 1)   /* return the timer interrupt status */
#define __ticking_setup(n, prescaler, ticks)                                \
    do {                                                                    \
        TIMER_SET_CTC(n);               /* set the timer as CTC mode */     \
        timer_prescaler(n, prescaler);  /* set the clock selection */       \
        TIMER_COUNTER_VALUE(n) = 0;     /* set the start ticks */           \
        TIMER_TOP_VALUE(n) = ticks;     /* set the top ticks */             \
    } while (0)
#define __ticking_start(n, prescaler, ticks, func)                          \
    do {                                                                    \
        __ticking_disable(n);                                               \
        __ticking_setup(n, prescaler, ticks);                               \
        __timer_func(n) = func;         /* assign the user function */      \
        __ticking_enable(n);                                                \
    } while (0)
#define __ticking_stop(n)                                                   \
    do {                                                                    \
        timer_prescaler(n, 0);      /* set the clock selection to None */   \
    } while (0)


#endif
