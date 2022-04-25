
#include "readings_and_tariffs.h"
#include "uart_user_service.h"
#include "mbed.h"
#include "arm_book_lib.h"

static Serial uartUsb(USBTX, USBRX);
static char receivedChar = '\0';

void serial_update() {

    if (uartUsb.readable())
    {
        char receivedChar = uartUsb.getc();
        uartUsb.printf("Received: %c\r\n", receivedChar);
    }
    else
    {
        uartUsb.printf("Nada... \r\n");
    }

    uartUsb.printf("Pote Reading: %f\r\n", readings_and_tariffs_get_current_measure());
}




typedef enum
{
    WAITING_FOR_USER_ACTIONS,
    PRINT_CONSUMPTION_HOME,
    PRINT_AVAILABLE_TARIFS,
    SET_NEW_TARIF_PRICE,
    HELP,
} UserFlowState;

static UserFlowState state;

static void user_flow_step_waiting_for_user_actions();
static void user_flow_step_print_help();

void user_flow_initialize()
{
    state = WAITING_FOR_USER_ACTIONS;
}

void user_flow_execute()
{

    switch (state)
    {

    case WAITING_FOR_USER_ACTIONS:

        user_flow_step_waiting_for_user_actions();

        break;

    case PRINT_CONSUMPTION_HOME:

        break;

    case PRINT_AVAILABLE_TARIFS:

        break;

    case SET_NEW_TARIF_PRICE:

        break;

    case HELP:

        user_flow_step_print_help();

        state = WAITING_FOR_USER_ACTIONS;
        break;
    }
}

static void user_flow_step_print_help()
{
    uartUsb.printf("Opciones disponibles: \r\n");
    uartUsb.printf("[I] Imprimir Consumo en Tiempo Real \r\n");
    uartUsb.printf("[P] Imprimir Tarifas Disponibles \r\n");
    uartUsb.printf("[T] Activar una tarifa \r\n");
    uartUsb.printf("[H] Help \r\n");
}

static void user_flow_step_waiting_for_user_actions()
{

    if (uartUsb.readable())
    {
        char receivedChar = uartUsb.getc();

        if (receivedChar == 'i' || receivedChar == 'I')
        {
            uartUsb.printf("%c\r\n", receivedChar);
        }

        else if (receivedChar == 'p' || receivedChar == 'P')
        {
            uartUsb.printf("%c\r\n", receivedChar);
        }

        else if (receivedChar == 't' || receivedChar == 'T')
        {
            uartUsb.printf("%c\r\n", receivedChar);
        }

        else if (receivedChar == 'h' || receivedChar == 'H')
        {
            state = HELP;
        }
    }
}
