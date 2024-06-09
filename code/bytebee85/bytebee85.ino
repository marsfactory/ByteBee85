#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/power.h>

// Incluye tu archivo .h
//#include "neolib_core.h"

#define SONGS_COUNT 10
#define NUM_SAMPLES 10

volatile unsigned long t;  // long
volatile uint8_t snd;      // 0...255

volatile uint8_t pot1;  // 0...255 //pin3 use THIS
volatile uint8_t pot2;  // 0...255 //pin7
int c4 = 0;
volatile uint8_t songs = 0;

//ADMUX ADC

volatile uint8_t adc1 = _BV(ADLAR) | _BV(MUX0);  //PB2-ADC1
volatile uint8_t adc2 = _BV(ADLAR) | _BV(MUX1);  //PB4-ADC2

#define ENTER_CRIT() \
  { \
    byte volatile saved_sreg = SREG; \
    cli()
#define LEAVE_CRIT() \
  SREG = saved_sreg; \
  }

#define true 1
#define false 0

#define BUTTON_PIN 3
//button state
#define BUTTON_NORMAL 0
#define BUTTON_PRESS 1
#define BUTTON_RELEASE 2
#define BUTTON_HOLD 3

uint16_t pot1_accumulator = 0;
uint8_t pot1_sample_count = 0;

void adc_init() {
  ADCSRA |= _BV(ADIE);                             //adc interrupt enable
  ADCSRA |= _BV(ADEN);                             //adc enable
  ADCSRA |= _BV(ADATE);                            //auto trigger
  ADCSRA |= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);  //prescale 128
  ADMUX = adc1;                                    // Inicializar con pot1
  ADCSRB = 0;
}

void adc_start() {
  ADCSRA |= _BV(ADSC);  //start adc conversion
}

void timer_init() {
  //no prescale
  clock_prescale_set(clock_div_1);

  //PWM SOUND OUTPUT - FIX
  TCCR0A |= (1 << WGM00) | (1 << WGM01);  //Fast pwm

  TCCR0A |= (1 << COM0B1);  //USE PB1 --> Clear OC0A/OC0B on Compare Match when up-counting.

  TCCR0B |= (1 << CS00);  //no prescale

  //TIMER1 SOUND GENERATOR @ 44100hz
  //babygnusb attiny85 clock frequency = 16.5 Mhz

  //TIMER SETUP -- FIX
  TCCR1 |= _BV(CTC1);    //clear timer on compare
  TIMSK |= _BV(OCIE1A);  //activate compare interruppt
  TCNT1 = 0;             //init count

  //TIMER FREQUENCY
  TCCR1 |= _BV(CS10) | _BV(CS12);  // prescale 16

  //SAMPLE RATE - FIX
  OCR1C = 120;  // (16500000/16)/8000 = 128

  // babygnusb led pin
  DDRB |= (1 << PB1);  //pin connected to led
}

void setup() {
  timer_init();  // initialize timer & Pwm
  adc_init();    //init adc
  sei();         //enable global interrupt
  adc_start();   //start adc conversion

  // Configurar el pin 2 como entrada para el botón
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

uint8_t wasButtonPressed(uint8_t pin) {
  static uint8_t lastState = HIGH;
  uint8_t currentState = digitalRead(pin);

  if (currentState == LOW && lastState == HIGH) {
    lastState = currentState;
    return BUTTON_PRESS;
  } else if (currentState == HIGH && lastState == LOW) {
    lastState = currentState;
    return BUTTON_RELEASE;
  } else {
    return BUTTON_NORMAL;
  }
}

int main(void) {
  setup();

  unsigned int btn_timer = 0;

  while (1) {
    btn_timer++;

    if (btn_timer > 2000) {
      uint8_t b = wasButtonPressed(BUTTON_PIN);  // Leer el estado del botón en el pin 2

      if (b == BUTTON_PRESS) {
        songs++;
        if (songs > SONGS_COUNT) songs = 0;
      }

      btn_timer = 0;
    }

    // Promediar valores de pot1
    if (pot1_sample_count >= NUM_SAMPLES) {
      pot1 = pot1_accumulator / NUM_SAMPLES;
      pot1_accumulator = 0;
      pot1_sample_count = 0;
    }

    // Mapea el valor de pot1 a un rango de a 20
    int pot1_mapped = map(pot1, 0, 150, 1, 30);
    int pot1_mapped3 = map(pot1, 0, 150, 1, 50);
    int pot1_mapped2 = map(pot1, 0, 150, 1, 12);

    switch (songs) {
      case 0:
        snd = t * 5 & (t >> 7) | t * 3 & (t * 4 >> 10);  // La dulce tranqui
        break;
      case 1:
        //snd = t*((42&t>>10)%14) //melody 42
        c4 = ((pot1 >> 5) + 1);  //mapeo entre 1  y 6
        snd = t * (((c4 + 42) & t >> 10) % 14);

        break;
      case 2:
        snd = 2 * t * (t & 16384 ? 6 : 5) * (4 - (3 & t >> 8)) >> (3 & -t >> (t & 4096 ? 2 : 15)) | t >> (t & 8192 ? t & 4096 ? 4 : 5 : 3);
        break;
      case 3:
        //snd = t * 4 & (t >> 5) | t * 6 & (t * 2 >> 5) | t * 2 & (t * 7 >> 2);  //el mantra
        snd = t * 4 & (t >> 9) | t * 6 & (t * 2 >> 5) | t * pot1 & (t * 7 >> 6);
        break;
      case 4:
        snd = t * 4 & (t >> 9) | t * 6 & (t * 2 >> 5) | t * pot1_mapped & (t * 7 >> 0);  // muy buen , one pot shoot
        break;
      case 5:
        snd = ((t << 2) | t >> 2 & (t << 4) | t >> 4 | (t / 40));  //africanita
        break;
      case 6:
        snd = ((t & 4096) ? ((t * (t ^ t % 255) | (t >> (4 + pot1))) >> 1) : (t >> 8) | ((t & 8192) ? t << 2 : t));  //tiroteo loco
        break;
      case 7:
        snd = (t & t >> 12) * (t >> (4 + pot1_mapped) | t >> (8 + pot1_mapped)) ^ t >> 6;  //la misteriosa
        break;
      case 8:
        snd = 10 * (t >> 6 | t | t >> (t >> 16)) + (7 & t >> pot1);
        break;
      case 9:
        c4 = ((pot1 >> 2) + 25);
        snd = (~t >> 2) * ((127 & t * (7 & t >> 10)) < (c4 & t * (2 + (5 & t >> 14))));
        break;
      case 10:
        c4 = ((pot1 >> 4) + 0);
        snd = t >> 5 | (t >> 2) * (t >> c4) ;
        break;
      case 11:
        snd = ((t*(t&131072?t&65536?3/2:4/3:1)>>(1&~t>>13)&128)*(-t>>5<<(1&t>>15)&pot1)>>7)+(t>>5<<(1&t>>15)&255)/2;
        break;
      case 12:
        snd = t>>t%(t%2?t&32768?41:t&16384?pot1:61:34)&(t/9&t/7|t/5&t/3|t|t*3&t*5|t*7&t*9)>>4;
      break;
    }
  }
  return 0;
}

ISR(TIMER1_COMPA_vect) {
  //sound generator pwm out - FIX
  OCR0B = snd;
  t++;
}

ISR(ADC_vect) {
  //http://joehalpin.wordpress.com/2011/06/19/multi-channel-adc-with-an-attiny85/

  static uint8_t firstTime = 1;
  static uint8_t val;

  val = ADCH;

  if (firstTime == 1) {
    firstTime = 0;
  } else if (ADMUX == adc1) {
    ADMUX = adc2;
    pot1_accumulator += val;
    pot1_sample_count++;
  } else if (ADMUX == adc2) {
    ADMUX = adc1;
    pot2 = val;
  }
}
