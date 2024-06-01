#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/power.h>

// Incluye tu archivo .h
#include "neolib_core.h"

#define SONGS_COUNT 7
#define NUM_SAMPLES 10

int potMap1 = 0 ;
int potMap2 = 0 ;
//
volatile unsigned long t; // long
volatile unsigned long u; // long
volatile uint8_t snd; // 0...255

volatile uint8_t pot1; // 0...255 //pin3
volatile uint8_t pot2; // 0...255 //pin7 use THIS
volatile uint8_t pot3; // 0...255 //pin2 botones

volatile uint8_t songs = 0;

volatile uint8_t btn1_previous = 1;
volatile uint8_t btn2_previous = 1;

//ADMUX ADC

volatile uint8_t adc1 = _BV(ADLAR) | _BV(MUX0); //PB2-ADC1 
volatile uint8_t adc2 = _BV(ADLAR) | _BV(MUX1); //PB4-ADC2 
volatile uint8_t adc3 = _BV(ADLAR) | _BV(MUX0) | _BV(MUX1); //PB3-ADC3 

#define ENTER_CRIT()    {byte volatile saved_sreg = SREG; cli()
#define LEAVE_CRIT()    SREG = saved_sreg;}

#define true 1
#define false 0

//button state
#define BUTTON_NORMAL 0
#define BUTTON_PRESS 1
#define BUTTON_RELEASE 2
#define BUTTON_HOLD 3

uint16_t pot1_accumulator = 0;
uint8_t pot1_sample_count = 0;

void adc_init()
{
  ADCSRA |= _BV(ADIE); //adc interrupt enable
  ADCSRA |= _BV(ADEN); //adc enable
  ADCSRA |= _BV(ADATE); //auto trigger
  ADCSRA |= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); //prescale 128
  ADMUX  = adc3;
  ADCSRB = 0;
}

void adc_start()
{
  ADCSRA |= _BV(ADSC); //start adc conversion
}

void timer_init()
{
  //no prescale
  clock_prescale_set(clock_div_1);

  //PWM SOUND OUTPUT - FIX
  TCCR0A |= (1 << WGM00) | (1 << WGM01); //Fast pwm

  //TCCR0A |= (1<<WGM00) ; //Phase correct pwm
  //TCCR0A |= (1<<COM0A1); //Clear OC0A/OC0B on Compare Match when up-counting.
  TCCR0A |= (1 << COM0B1); //USE PB1 --> Clear OC0A/OC0B on Compare Match when up-counting.

  TCCR0B |= (1 << CS00); //no prescale

  //TIMER1 SOUND GENERATOR @ 44100hz
  //babygnusb attiny85 clock frequency = 16.5 Mhz

  //TIMER SETUP -- FIX
  TCCR1 |= _BV(CTC1); //clear timer on compare
  TIMSK |= _BV(OCIE1A); //activate compare interruppt
  TCNT1 = 0; //init count

  //TIMER FREQUENCY
  //TCCR1 |= _BV(CS10); // prescale 1
  //TCCR1 |= _BV(CS11); // prescale 2
  TCCR1 |= _BV(CS10) | _BV(CS12); // prescale 16
  //TCCR1 |= _BV(CS11)|_BV(CS12); // prescale 32
  //TCCR1 |= _BV(CS10)|_BV(CS11)|_BV(CS12); // prescale 64
  //TCCR1 |= _BV(CS13); // prescale 128
  //TCCR1 |= _BV(CS10) | _BV(CS13); // prescale 256

  //SAMPLE RATE - FIX
  OCR1C = 120; // (16500000/16)/8000 = 128
  //OCR1C = 45; // (16500000/16)/11025 = 93
  //OCR1C = 22; // (16500000/16)/22050 = 46
  //OCR1C = 23; // (16500000/16)/44100 = 23

  // babygnusb led pin
  DDRB |= (1 << PB1); //pin connected to led
}

int main(void)
{
  timer_init(); // initialize timer & Pwm
  adc_init(); //init adc
  sei(); //enable global interrupt
  adc_start(); //start adc conversion

  // run forever
  unsigned int loop_timer = 0;
  unsigned int btn_timer = 0;

  while (1)
  {
    loop_timer++;
    btn_timer++;

    if (btn_timer > 2000)
    {
      uint8_t b = wasButtonPressed(pot3);

      if ( b == BUTTON_RIGHT )   songs-- ;
      if ( b == BUTTON_LEFT )   songs++ ; //Use This

      if (songs > SONGS_COUNT) songs = 0;
      if (songs < 0 ) songs = SONGS_COUNT;
      btn_timer = 0;
    }

    if (loop_timer > 10000)
    {
      loop_timer = 0;
    }

    // Promediar valores de pot1
    if (pot1_sample_count >= NUM_SAMPLES) {
      pot1 = pot1_accumulator / NUM_SAMPLES;
      pot1_accumulator = 0;
      pot1_sample_count = 0;
    }

    // Mapea el valor de pot1 a un rango de 1 a 20
    int pot1_mapped = map(pot1, 0, 150, 1, 30);

    switch (songs)
    {
      case 0:
        snd = t * pot1_mapped; 
        break;
      case 1:
        snd = pot1_mapped; // Usa el valor mapeado
        break;
      case 2:
        snd = t >> pot2;
        break;
      case 3:
        snd = t >> pot1_mapped; // Usa el valor mapeado
        break;
      case 4:
        snd = t * pot2;
        break;
      case 5:
        snd = t * pot1_mapped; // Usa el valor mapeado
        break;
      case 6:
        snd = t % pot2;
        break;
      case 7:
        snd = t % pot1_mapped; // Usa el valor mapeado
        break;
    }
  }
  return 0;
}

ISR(TIMER1_COMPA_vect)
{
  //sound generator pwm out - FIX
  OCR0B = snd;
  t++;
}

ISR(ADC_vect)
{
  //http://joehalpin.wordpress.com/2011/06/19/multi-channel-adc-with-an-attiny85/

  static uint8_t firstTime = 1;
  static uint8_t val;

  val = ADCH;

  if (firstTime == 1) {
    firstTime = 0;
  }
  else if (ADMUX == adc1) {
    pot3 = val;
    ADMUX = adc2;
  }
  else if (ADMUX == adc2) {
    pot2 = val;
    ADMUX = adc3;
  }
  else if (ADMUX == adc3) {
    pot1_accumulator += val;
    pot1_sample_count++;
    ADMUX = adc1;
  }
}
