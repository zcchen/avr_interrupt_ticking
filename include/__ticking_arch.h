#ifndef __AVR_TICKING_ARCH__
#define __AVR_TICKING_ARCH__

#ifndef __AVR_TICKING__
    #error "This header file should be used under 'ticking.h', NOT standalone."
#endif

#ifndef __TIMER_PRESCALER
    #define __TIMER_PRESCALER(n)        __TIMER_PRESCALER_ ## n
    #define __TIMER_PRESCALER_0         0b000
    #define __TIMER_PRESCALER_1         0b001
    #define __TIMER_PRESCALER_8         0b010
    #define __TIMER_PRESCALER_64        0b011
    #define __TIMER_PRESCALER_256       0b100
    #define __TIMER_PRESCALER_1024      0b101
    #define __TIMER_PRESCALER_falling   0b110
    #define __TIMER_PRESCALER_rising    0b111
#endif

#if   defined(__AVR_ATmega48__) ||defined(__AVR_ATmega88__) || defined(__AVR_ATmega168__) || \
      defined(__AVR_ATmega48P__) ||defined(__AVR_ATmega88P__) || defined(__AVR_ATmega168P__) || \
      defined(__AVR_ATmega328P__)
    #define ATMEGA_TIMER_0
    #define ATMEGA_TIMER_1
    #define ATMEGA_TIMER_2
    #define TIMER_CONTROL(n, m)         TCCR ## n ## m
    #define TIMER_CLOCK_SELECT(n, m)    CS ## n ## m
    #define TIMER_COUNTER_VALUE(n)      TCNT ## n
    #define TIMER_TOP_VALUE(n)          OCR ## n ## A
    #define TIMER_INTRP_MASK(n)         TIMSK ## n
    #define TIMER_CTC_INTRP(n)          OCIE ## n ## A
    #define TIMER_WGM(n, m)             WGM ## n ## m
    #define TIMER_FLAG(n)               (TIFR ## n & (1 << OCF ## n ## A))
    #define __TIMER_SET_CTC_0           __TIMER_SET_CTC_8_Bit(0)
    #define __TIMER_SET_CTC_1           __TIMER_SET_CTC_16_Bit(1)
    #define __TIMER_SET_CTC_2           __TIMER_SET_CTC_8_Bit(2)
    #define timer_prescaler(n, prescaler)                       \
        TIMER_CONTROL(n, B) &= 0b11111000;                      \
        TIMER_CONTROL(n, B) ^= __TIMER_PRESCALER_ ## prescaler
#elif defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega640__) || \
      defined(__AVR_ATmega2561__) || defined(__AVR_ATmega1281__)
    #define ATMEGA_TIMER_0
    #define ATMEGA_TIMER_1
    #define ATMEGA_TIMER_2
    #define ATMEGA_TIMER_3
    #define ATMEGA_TIMER_4
    #define ATMEGA_TIMER_5
    #define TIMER_CONTROL(n, m)         TCCR ## n ## m
    #define TIMER_CLOCK_SELECT(n, m)    CS ## n ## m
    #define TIMER_COUNTER_VALUE(n)      TCNT ## n
    #define TIMER_TOP_VALUE(n)          OCR ## n ## A
    #define TIMER_INTRP_MASK(n)         TIMSK ## n
    #define TIMER_CTC_INTRP(n)          OCIE ## n ## A
    #define TIMER_WGM(n, m)             WGM ## n ## m
    #define TIMER_FLAG(n)               (TIFR ## n & (1 << OCF ## n ## A))
    #define __TIMER_SET_CTC_0           __TIMER_SET_CTC_8_Bit(0)
    #define __TIMER_SET_CTC_1           __TIMER_SET_CTC_16_Bit(1)
    #define __TIMER_SET_CTC_2           __TIMER_SET_CTC_8_Bit(2)
    #define __TIMER_SET_CTC_3           __TIMER_SET_CTC_16_Bit(3)
    #define __TIMER_SET_CTC_4           __TIMER_SET_CTC_16_Bit(4)
    #define __TIMER_SET_CTC_5           __TIMER_SET_CTC_16_Bit(5)
    #define timer_prescaler(n, prescaler)                       \
        TIMER_CONTROL(n, B) &= 0b11111000;                      \
        TIMER_CONTROL(n, B) ^= __TIMER_PRESCALER(prescaler)
#endif

#endif
