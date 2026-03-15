/*
 * main.c
 * Version: 1.0
 * Created: 02/23/2026
 * Last edit: See git history
 * Author: Gabriel Varao
 * Project: StepMotor
 */

/********** CPU Configuration **********/
#define F_CPU 16000000UL

/********** Includes **********/
#include <xc.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdint.h>

/********** Input Button Pins **********/
#define BTN_DIR_CW      PA0
#define BTN_DIR_CCW     PA1
#define BTN_SPEED_UP    PA7
#define BTN_SPEED_DOWN  PA6

/********** Motor Output Port **********/
#define MOTOR_PORT PORTC

/********** Timing Configuration **********/
#define LOOP_INTERVAL 0.1        // ms per FSM iteration
#define MIN_STEP_DELAY  (3.3 / LOOP_INTERVAL)
#define MAX_STEP_DELAY  (500 / LOOP_INTERVAL)
#define DEFAULT_DELAY   (50 / LOOP_INTERVAL)

/********** Speed Adjustment Parameters **********/
#define SPEED_SCALE      0.2
#define SPEED_OFFSET     0.2

/********** Motor States **********/
typedef enum {
    STATE_CW,
    STATE_CCW,
    STATE_INVALID
} MotorState;

/********** Step Sequence **********/
const uint8_t STEP_SEQUENCE[] = {
    0b00001001,
    0b00000001,
    0b00000011,
    0b00000010,
    0b00000110,
    0b00000100,
    0b00001100,
    0b00001000
};

const uint8_t STEP_COUNT = sizeof(STEP_SEQUENCE) / sizeof(STEP_SEQUENCE[0]);

/********** Hardware Utility Functions **********/

static bool read_button(volatile uint8_t *reg, uint8_t pin)
{
    return ((*reg) & (1 << pin)) != 0;
}

static void setup_io(void)
{
    /* Configure PORTA as input with pull-ups */
    DDRA = 0x00;
    PORTA = 0xFF;

    /* Configure PORTC as output for stepper driver */
    DDRC = 0xFF;
    PORTC = 0x00;
}

/********** Speed Control Functions **********/

static void increase_speed(float *delay)
{
    *delay *= (1.0 - SPEED_SCALE);
    *delay -= SPEED_OFFSET;

    if (*delay < MIN_STEP_DELAY)
        *delay = MIN_STEP_DELAY;
}

static void decrease_speed(float *delay)
{
    *delay *= (1.0 + SPEED_SCALE);
    *delay += SPEED_OFFSET;

    if (*delay > MAX_STEP_DELAY)
        *delay = MAX_STEP_DELAY;
}

/********** Motor Step Functions **********/

static uint8_t next_step(uint8_t current)
{
    return (current + 1) % STEP_COUNT;
}

static uint8_t previous_step(uint8_t current)
{
    return (current + STEP_COUNT - 1) % STEP_COUNT;
}

/********** Main Application **********/

int main(void)
{
    setup_io();

    uint8_t stepIndex = 0;
    uint16_t stepTimer = 0;
    float stepDelay = DEFAULT_DELAY;

    MotorState state = STATE_CW;

    bool prevCW  = read_button(&PINA, BTN_DIR_CW);
    bool prevCCW = read_button(&PINA, BTN_DIR_CCW);
    bool prevUp  = read_button(&PINA, BTN_SPEED_UP);
    bool prevDown = read_button(&PINA, BTN_SPEED_DOWN);

    bool current;

    while (1)
    {
        _delay_ms(LOOP_INTERVAL);

        switch (state)
        {
            case STATE_CW:

                if (stepTimer++ >= stepDelay)
                {
                    stepIndex = next_step(stepIndex);
                    stepTimer = 0;
                }

                current = read_button(&PINA, BTN_DIR_CW);
                if (prevCW != current && current)
                    state = STATE_CCW;

                prevCW = current;

                break;

            case STATE_CCW:

                if (stepTimer++ >= stepDelay)
                {
                    stepIndex = previous_step(stepIndex);
                    stepTimer = 0;
                }

                current = read_button(&PINA, BTN_DIR_CCW);
                if (prevCCW != current && current)
                    state = STATE_CW;

                prevCCW = current;

                break;

            default:
                state = STATE_CW;
                break;
        }

        /* Speed Up Button */
        current = read_button(&PINA, BTN_SPEED_UP);
        if (prevUp != current && current)
            increase_speed(&stepDelay);

        prevUp = current;

        /* Speed Down Button */
        current = read_button(&PINA, BTN_SPEED_DOWN);
        if (prevDown != current && current)
            decrease_speed(&stepDelay);

        prevDown = current;

        /* Output current step */
        MOTOR_PORT = STEP_SEQUENCE[stepIndex];
    }
}