
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define DDR_JESUS (DDRB)
#define DDR_MARY (DDRB)
#define PORT_JESUS (PORTB)
#define PORT_MARY (PORTB)
#define PIN_JESUS_LEFT (PINB0)
#define PIN_JESUS_RIGHT (PINB1)
#define PIN_MARY_LEFT (PINB3)
#define PIN_MARY_RIGHT (PINB4)
#define jesus_on() PORT_JESUS |= ((1 << PIN_JESUS_LEFT) | (1 << PIN_JESUS_RIGHT));
#define jesus_off() PORT_JESUS &= ~((1 << PIN_JESUS_LEFT) | (1 << PIN_JESUS_RIGHT));
#define mary_on() PORT_MARY |= ((1 << PIN_MARY_LEFT) | (1 << PIN_MARY_RIGHT));
#define mary_off() PORT_MARY &= ~((1 << PIN_MARY_LEFT) | (1 << PIN_MARY_RIGHT));
#define JESUS (1)
#define MARY (2)
#define BOTH (3)
#define TIME_SHORT_PULSE (1)
#define TIME_LONG_PULSE (5)
#define TIME_AFTER_PULSE (2)
#define TIME_AFTER_CHARACTER (5)
#define TIME_AFTER_WORD (10)
#define PATER_NOSTER_LEN (979)
#define AMEN_LEN (12)

/*
pater noster, qui es in caelis,
sanctificetur nomen tuum,
adveniat regnum tuum,
fiat voluntas tua,
sicut in caelo et in terra.
panem nostrum quotidianum
da nobis hodie,
et dimitte nobis debita nostra,
sicut et nos dimittimus
debitoribus nostris.
et ne nos inducas in tentationem,
sed libera nos a malo.
*/
const char pater_noster[PATER_NOSTER_LEN] PROGMEM = ".--. .- - . .-. / -. --- ... - . .-. --..-- / --.- ..- .. / . ... / .. -. / -.-. .- . .-.. .. ... --..-- / ... .- -. -.-. - .. ..-. .. -.-. . - ..- .-. / -. --- -- . -. / - ..- ..- -- --..-- / .- -.. ...- . -. .. .- - / .-. . --. -. ..- -- / - ..- ..- -- --..-- / ..-. .. .- - / ...- --- .-.. ..- -. - .- ... / - ..- .- --..-- / ... .. -.-. ..- - / .. -. / -.-. .- . .-.. --- / . - / .. -. / - . .-. .-. .- .-.-.- / .--. .- -. . -- / -. --- ... - .-. ..- -- / --.- ..- --- - .. -.. .. .- -. ..- -- / -.. .- / -. --- -... .. ... / .... --- -.. .. . --..-- / . - / -.. .. -- .. - - . / -. --- -... .. ... / -.. . -... .. - .- / -. --- ... - .-. .- --..-- / ... .. -.-. ..- - / . - / -. --- ... / -.. .. -- .. - - .. -- ..- ... / -.. . -... .. - --- .-. .. -... ..- ... / -. --- ... - .-. .. ... .-.-.- / . - / -. . / -. --- ... / .. -. -.. ..- -.-. .- ... / .. -. / - . -. - .- - .. --- -. . -- --..-- / ... . -.. / .-.. .. -... . .-. .- / -. --- ... / .- / -- .- .-.. --- .-.-.- /";
/*
amen
*/
const char amen[AMEN_LEN] PROGMEM = ".- -- . -. /";

static void __attribute__ ((noinline)) delay_100ms(uint8_t n) {
    for (uint8_t i = 0; i < n; ++i) {
        _delay_ms(50);
        _delay_ms(50);
    }
}

static void init_hw(void) {
    DDR_JESUS |= ((1 << PIN_JESUS_LEFT) | (1 << PIN_JESUS_RIGHT));
    DDR_MARY |= ((1 << PIN_MARY_LEFT) | (1 << PIN_MARY_RIGHT));
}

static void __attribute__ ((noinline)) morse_pulse(uint8_t who, uint8_t on_time) {
    if (who & JESUS)    jesus_on();
    if (who & MARY)     mary_on();
    delay_100ms(on_time);
    jesus_off();
    mary_off();
    delay_100ms(TIME_AFTER_PULSE);
}

static void __attribute__ ((noinline)) morse_play(uint8_t who, char *str, uint16_t str_len) {
    for (uint16_t i = 0; i < str_len; ++i) {
        uint8_t ch = pgm_read_byte(&(str[i]));
        switch (ch) {
            case '.':
                morse_pulse(who, TIME_SHORT_PULSE);
                break;

            case '-':
                morse_pulse(who, TIME_LONG_PULSE);
                break;

            case ' ':
                delay_100ms(TIME_AFTER_CHARACTER);
                break;

            case '/' :
                delay_100ms(TIME_AFTER_WORD);
                break;
        }
    }
}

int main(void) {
    init_hw();
    while (1) {
        morse_play(JESUS, pater_noster, PATER_NOSTER_LEN);
        morse_play(MARY, amen, AMEN_LEN);
        morse_play(MARY, pater_noster, PATER_NOSTER_LEN);
        morse_play(JESUS, amen, AMEN_LEN);
        morse_play(BOTH, pater_noster, PATER_NOSTER_LEN);
        morse_play(BOTH, amen, AMEN_LEN);
    }
}
