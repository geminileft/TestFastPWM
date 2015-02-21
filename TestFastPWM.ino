#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_PIN 9
#define LED_PIN_BITMASK 1 << (LED_PIN % 8)
#define TICKS_PER_SECOND (F_CPU / 1024)
#define DUTY_CYCLE TICKS_PER_SECOND / 3

typedef unsigned int uint;

volatile uint pulseCount;
volatile bool on;

void setup() {
  cli();
  
  //set LED_PIN to output
  DDRB = LED_PIN_BITMASK;
  
  //clear
  TCCR1A = 0;
  TCCR1B = 0;
  
  //fast pwm mode 14
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  
  //clear on compare, set at bottom
  TCCR1A |= (1 << COM1A1);
  
  //select clock 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  
  //set top to ticks
  ICR1 = TICKS_PER_SECOND - 1;
  
  //set to 25% duty cycle
  OCR1A = DUTY_CYCLE - 1;
  
  TCNT1 = 0;
  pulseCount = 0;
  on = true;
  TIMSK1 = (1 << TOIE1);
  sei();
}

void loop() {
}

ISR(TIMER1_OVF_vect) {
  ++pulseCount;
  if (pulseCount == 5) {
    on = !on;
    if (on) {
      TCCR1A |= (1 << COM1A1);
    } else {
      TCCR1A &= ~(1 << COM1A1);
    }
    pulseCount = 0;
  }
}
