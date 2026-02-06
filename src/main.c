/**
 * TP2 - Partie 5 - Programme modifie
 *
 * Comportement :
 *   - SW0 releve : les 8 LED de gauche (LED 15 a 8) clignotent
 *                  les LED de droite ne sont pas impactees
 *   - SW1 releve :
 *       * LED 3 a 0 : compteur modulo 16, incremente par bouton Center
 *         (gestion du rebond)
 *       * Bouton Left  : allume les LED 7 a 4
 *       * Bouton Right : eteint les LED 7 a 4
 *
 * Mapping des LEDs (16 bits) :
 *   Bits [15:8] = 8 LED de gauche
 *   Bits  [7:4] = LED 7 a 4 (milieu)
 *   Bits  [3:0] = LED 3 a 0 (droite, compteur)
 *
 * Boutons : Right[0], Center[1], Left[2]
 */

#include "xgpio.h"
#include "xparameters.h"

void delay(volatile int count)
{
    while (count > 0)
        count--;
}

#define BTN_RIGHT  0x01
#define BTN_CENTER 0x02
#define BTN_LEFT   0x04

#define SW0 0x01
#define SW1 0x02

int main()
{
    XGpio led_sw, bouton;
    int sw_state, btn_state;
    int compteur = 0;
    int led_mid = 0;
    int led_high = 0;
    int blink_on = 0;
    int prev_center = 0;
    int led_output;

    XGpio_Initialize(&led_sw, XPAR_LED_SWITCH_BASEADDR);
    XGpio_Initialize(&bouton, XPAR_BOUTONS_BASEADDR);

    XGpio_SetDataDirection(&led_sw, 1, 0xF);
    XGpio_SetDataDirection(&led_sw, 2, 0x0000);
    XGpio_SetDataDirection(&bouton, 1, 0x7);

    XGpio_DiscreteWrite(&led_sw, 2, 0x0000);

    while (1)
    {
        sw_state  = XGpio_DiscreteRead(&led_sw, 1);
        btn_state = XGpio_DiscreteRead(&bouton, 1);

        /* ======================================================
         * SW0 : Clignotement des 8 LED de gauche (bits 15 a 8)
         * ====================================================== */
        if (sw_state & SW0)
        {
            if (blink_on)
                led_high = 0xFF00;
            else
                led_high = 0x0000;
            blink_on = !blink_on;
        }
        else
        {
            led_high = 0x0000;
        }

        /* ======================================================
         * SW1 : Compteur + controle LED 7-4
         * ====================================================== */
        if (sw_state & SW1)
        {
            /* Compteur modulo 16 sur bouton Center (front montant) */
            if ((btn_state & BTN_CENTER) && !prev_center)
            {
                compteur = (compteur + 1) % 16;
                delay(500000);
            }
            prev_center = (btn_state & BTN_CENTER) ? 1 : 0;

            /* Bouton Left : allume LED 7-4 */
            if (btn_state & BTN_LEFT)
                led_mid = 0xF0;

            /* Bouton Right : eteint LED 7-4 */
            if (btn_state & BTN_RIGHT)
                led_mid = 0x00;
        }

        /* Construction sortie 16 bits */
        led_output = led_high | (led_mid & 0xF0) | (compteur & 0x0F);
        XGpio_DiscreteWrite(&led_sw, 2, led_output);

        /* Delai pour le clignotement */
        delay(2000000);
    }

    return 0;
}