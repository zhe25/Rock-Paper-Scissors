/** File -> game.c
 * Authors -> Zhengjingrui He, Stanley Perry
 * Date -> 12/10/2020
 * Game allows players to select either rock, paper or scissors
 * and display a result (W,L,T) based on rules of game.
 * */


#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "button.h"
#include <stdbool.h>


/** Initialising game variables**/
#define PACER_RATE 500
#define MESSAGE_RATE 10
uint16_t check = 1;
char RPS[] = {'R', 'P', 'S'};
char character;
char ans = '\0';
char player = '\0';
// int display_ans = 0;
int count = 0;
bool check_start = true;


/** Function for initialising game**/
void game_init(void)
{
    system_init ();
    tinygl_init (PACER_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    navswitch_init ();
    ir_uart_init ();
    pacer_init (PACER_RATE);
}

/** Function to display a character on LED matrix**/
void display_character (char character)
{
    char buffer[2];
    buffer[0] = character;
    buffer[1] = '\0';
    tinygl_text (buffer);
}


/** Function to run the game**/
void run_game(void)
{
    pacer_wait ();
    tinygl_update ();
    navswitch_update ();

    /** Cycle through R, P ,S using nav switch.**/

    if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
        count++;
        if (count == 3) {
            count = 0;
        }
        character = RPS[count];
    }

    if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
        count--;
        if (count == -1) {
            count = 2;
        }
        character = RPS[count];
    }

    /** Send selected character through IR_UART to opponent when nav switch pushed**/
    if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
        ir_uart_putc (character);
    }

    /** decide whether player wins, loses or ties **/
    if (ir_uart_read_ready_p()) {
        player = ir_uart_getc ();
        if (player == 'W') {
            character = 'L';
        } else if (player == 'L') {
            character = 'W';
        } else if (player == 'T') {
            character = 'T';
        } else if (player == 'R') {
            if (character == 'P') {
                ans = 'W';
            } else if (character == 'S') {
                ans = 'L';
            } else if (character == 'R') {
                ans = 'T';
            }
            character = ans;
        } else if(player == 'P') {
            if (character == 'R') {
                ans = 'L';

            } else if (character == 'S') {
                ans = 'W';
            } else if(character == 'P') {
                ans = 'T';
            }
            character = ans;
        } else if (player == 'S') {
            if (character == 'R') {
                ans = 'W';

            } else if (character == 'P') {
                ans = 'L';
            } else if(character == 'S') {
                ans = 'T';
            }
            character = ans;
        }

    }
    display_character (character);
}

/** initialise game and display starting message**/
void welcome(void)
{
    system_init();
    tinygl_init (PACER_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    pacer_init (PACER_RATE);
    tinygl_text ("START");

    /** returns false if button has been pressed after results displayed to restart*/
    while(check_start) {
        pacer_wait();
        tinygl_update ();
        if (button_pressed_p ()) {
            check_start = false;
        }

    }
}

/** main loop to run game**/
int main (void)
{

    character = RPS[0];
    game_init();

    while(1) {

        pacer_wait();
        tinygl_update ();

        if (check_start) {
            welcome();

        } else {

            run_game();
        }

    }

}
