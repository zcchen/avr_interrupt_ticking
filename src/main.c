#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ticking.h"

void arduino_uno_led_setup()
{
    DDRB |= (1 << DDB5);
    PORTB &= ~(1 << PORTB5);
}

void arduino_uno_led_toggle()
{
    PORTB ^= (1 << PORTB5);
}

void arduino_uno_led_ctrl(char n)
{
    if (n) {
        PORTB |= (1 << PORTB5);
    }
    else {
        PORTB &= ~(1 << PORTB5);
    }
}

int main(void)
{
    arduino_uno_led_setup();
    arduino_uno_led_ctrl(0);
    sysclock_read();
    if (!sysclock_get_s()) {
        sysclock_set(1531379730, 0);
    }
    else {
        sysclock_set(0, 0);
        arduino_uno_led_ctrl(1);
        _delay_ms(3000);
        arduino_uno_led_ctrl(0);
    }
    sysclock_clear();
    /*ticking_start(1, 1024, ms_ticks(1000, 1024), arduino_uno_led_toggle);*/
    /*ticking_start(0, 64, ms_ticks(1, 64), arduino_uno_led_toggle);*/
    sei();
    while (1) {
        arduino_uno_led_ctrl(sysclock_get_s() & 1);
        /*sysclock_save();*/
    }
    return 0;
}


