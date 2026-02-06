#include "xgpio.h"
#include "xparameters.h"

int main()
{
    XGpio led_sw, bouton;
    int sw_state;

    /* Initialisation avec BASEADDR au lieu de DEVICE_ID */
    XGpio_Initialize(&led_sw, XPAR_LED_SWITCH_BASEADDR);
    XGpio_Initialize(&bouton, XPAR_BOUTONS_BASEADDR);

    XGpio_SetDataDirection(&led_sw, 1, 0xF);
    XGpio_SetDataDirection(&led_sw, 2, 0x0000);
    XGpio_SetDataDirection(&bouton, 1, 0x7);

    while (1)
    {
        sw_state = XGpio_DiscreteRead(&led_sw, 1);
        XGpio_DiscreteWrite(&led_sw, 2, sw_state);
    }

    return 0;
}