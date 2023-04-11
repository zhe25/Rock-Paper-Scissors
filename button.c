/** File -> game.c
 * Authors -> Zhengjingrui He, Stanley Perry
 * Date -> 12/10/2020
 * Includes funcitons to intitialise and check if the
 * start button is pushed to start the game
 **/

#include "pio.h"
#include "button.h"
#define PD7_PIO PIO_DEFINE(PORT_D, 7)

/** Return non-zero if button pressed.  **/
int button_pressed_p (void)
{
    /* Returns state of pin 7  */
    return pio_input_get (PD7_PIO);
}


/** Initialise button1.  **/
void button_init (void)
{
    /* Configures pin 7 as an input */
    pio_config_set (PD7_PIO, PIO_INPUT);
}



