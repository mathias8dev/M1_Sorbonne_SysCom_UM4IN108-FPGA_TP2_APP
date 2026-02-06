/**
 * TP2 - Partie 5 - Exercice 2 : Chenillard
 *
 * Fait parcourir une LED allumee sur les 16 LEDs de la carte
 * avec une vitesse de defilement parametrable par les 4 interrupteurs.
 *
 * Vitesses (SW[3:0]) :
 *   0000 (0)  -> tres lent
 *   0001 (1)  -> lent
 *   0010 (2)  -> moyen-lent
 *   0011 (3)  -> moyen
 *   ...
 *   1111 (15) -> tres rapide
 *
 * Plus la valeur des switches est grande, plus le chenillard est rapide.
 */

#include "xgpio.h"
#include "xparameters.h"

void delay(volatile int count)
{
    while (count > 0)
        count--;
}

int main()
{
    XGpio led_sw;
    int sw_state;
    int led_pos = 0;
    int direction = 1;    /* 1 = vers la gauche, -1 = vers la droite */
    int delay_val;

    XGpio_Initialize(&led_sw, XPAR_LED_SWITCH_BASEADDR);

    XGpio_SetDataDirection(&led_sw, 1, 0xF);
    XGpio_SetDataDirection(&led_sw, 2, 0x0000);

    XGpio_DiscreteWrite(&led_sw, 2, 0x0000);

    while (1)
    {
        /* Lecture des interrupteurs pour la vitesse */
        sw_state = XGpio_DiscreteRead(&led_sw, 1) & 0xF;

        /* Calcul du delai :
         * sw = 0  -> 8 000 000 (tres lent)
         * sw = 15 -> 500 000   (tres rapide)
         */
        delay_val = 8000000 - (sw_state * 500000);
        if (delay_val < 500000)
            delay_val = 500000;

        /* Allumer uniquement la LED a la position actuelle */
        XGpio_DiscreteWrite(&led_sw, 2, 1 << led_pos);

        delay(delay_val);

        /* Deplacement aller-retour */
        led_pos += direction;

        if (led_pos >= 15)
        {
            led_pos = 15;
            direction = -1;
        }
        else if (led_pos <= 0)
        {
            led_pos = 0;
            direction = 1;
        }
    }

    return 0;
}