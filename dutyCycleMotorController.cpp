/*
 ------------------------------------------------------------
  Project #4 – Duty Cycle Motor Controller
  Author: Emily Bearden
  Course: CMPE 311 – Fall 2025
  File: dutyCycleMotorController.cpp

  Description:
    Implements a software-based PWM generator (ISR-driven),
    debounced pushbutton input, and asynchronous task-based
    duty-cycle stepping (0/8 → 2/8 → 4/8 → 6/8 → 8/8 → 6/8 → … → 0/8).

    The PWM output drives an LED for testing and can be
    connected to a MOSFET motor driver for final integration.

    This code is designed to run concurrently with the
    asynchronous task manager from Project #2.
 ------------------------------------------------------------
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>


const uint8_t PWM_PIN      = 9;   // OC1A pin
const uint8_t BUTTON_PIN   = 2;   // External interrupt pin or polled input


const uint8_t DUTY_STATES[5] = {0, 2, 4, 6, 8};
const uint8_t NUM_STATES     = 5;


volatile uint8_t duty_index = 0;      // Which step in DUTY_STATES[]
volatile uint8_t current_duty = 0;    // Actual duty being used by ISR

volatile uint8_t pwm_counter = 0;     // ISR counter (0–7)
volatile int8_t duty_direction = 1;   // +1 going up, -1 going down


enum DebounceState {
    DB_IDLE,
    DB_WAIT_HIGH,
    DB_STABLE_HIGH,
    DB_WAIT_LOW
};

DebounceState db_state = DB_IDLE;
uint16_t db_timer = 0;
const uint16_t DB_DELAY = 25; // ms debounce time


void task_button_handler(void);
void task_update_pwm(void);


void init_pwm_timer()
{
    cli();

    // Timer1 configured for CTC mode, OCR1A used for timing
    TCCR1A = 0x00;
    TCCR1B = (1 << WGM12) | (1 << CS11); // CTC mode, prescaler=8

    OCR1A = 250; // ~2 kHz ISR rate (depends on CPU clock)

    TIMSK1 = (1 << OCIE1A); // Enable interrupt

    sei();
}


ISR(TIMER1_COMPA_vect)
{
    pwm_counter++;
    if (pwm_counter >= 8)
        pwm_counter = 0;

    if (pwm_counter < current_duty)
        PORTB |= (1 << PB1);     // PWM pin high (OC1A)
    else
        PORTB &= ~(1 << PB1);    // PWM pin low
}


void init_io()
{
    // PWM pin (PB1 → digital 9)
    DDRB |= (1 << PB1);

    // Button pin input
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2);  // enable pull-up
}


void task_button_handler()
{
    uint8_t raw = (PIND & (1 << PD2)) == 0 ? 1 : 0; // active LOW button

    switch (db_state)
    {
        case DB_IDLE:
            if (raw == 1) {
                db_timer = 0;
                db_state = DB_WAIT_HIGH;
            }
            break;

        case DB_WAIT_HIGH:
            if (raw == 1) {
                if (++db_timer >= DB_DELAY) {
                    db_state = DB_STABLE_HIGH;
                    // *** CLEAN BUTTON PRESS DETECTED ***
                    duty_index += duty_direction;

                    if (duty_index >= NUM_STATES - 1)
                        duty_direction = -1;
                    else if (duty_index == 0)
                        duty_direction = +1;

                    current_duty = DUTY_STATES[duty_index];
                }
            } else {
                db_state = DB_IDLE;
            }
            break;

        case DB_STABLE_HIGH:
            if (raw == 0) {
                db_timer = 0;
                db_state = DB_WAIT_LOW;
            }
            break;

        case DB_WAIT_LOW:
            if (raw == 0) {
                if (++db_timer >= DB_DELAY) {
                    db_state = DB_IDLE;
                }
            } else {
                db_state = DB_STABLE_HIGH;
            }
            break;
    }
}


void task_update_pwm()
{
    current_duty = DUTY_STATES[duty_index];
}

int main(void)
{
    init_io();
    init_pwm_timer();

    taskManager_register(task_button_handler, 5);  // every 5 ms
    taskManager_register(task_update_pwm, 1);      // frequent update

    sei();

    // Main task loop
    while (1)
    {
        taskManager_execute();  // Project #2 scheduler
    }

    return 0;
}

