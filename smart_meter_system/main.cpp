//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"


#include "display.h"
#include "uart_user_service.h"
#include "measurements_and_tariffs.h"

#define DISPLAY_REFRESH_TIME_MS 1000

//===============================================================

static Ticker update_display_interruption;


static bool display_ready_to_update = false;

void display_update_trigger() {
    display_ready_to_update = true;
}

static void display_update() {

    char measure[14];
    char spending[14];

    sprintf(measure, "%.2f kWh", measurements_and_tariffs_get_current_measure());
    displayCharPositionWrite(7, 0);
    displayStringWrite(measure);

    sprintf(spending, "$ %.2f", measurements_and_tariffs_get_current_spending());
    displayCharPositionWrite(7, 1);
    displayStringWrite(spending);
}

void display_init() {

    displayInit(DISPLAY_CONNECTION_I2C_PCF8574_IO_EXPANDER);

    displayCharPositionWrite(0, 0);
    displayStringWrite("E.Con:");

    displayCharPositionWrite(0, 1);
    displayStringWrite("Cost:");
}



int main() {
    
    display_init();

    update_display_interruption.attach(&display_update_trigger, 1);

    while (true)
    {

        measurements_and_tariffs_execute();

        serial_update();

        if (display_ready_to_update){
            display_update();
            display_ready_to_update = false;
        }

        delay(1000);
    }
}


//=====================================================================

//======================== UART ================================
