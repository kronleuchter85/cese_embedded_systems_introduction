
#include "mbed.h"
#include "arm_book_lib.h"

#include <string.h>
#include "readings_and_tariffs.h"
#include "uart_user_service.h"
#include "repository.h"

static Serial uartUsb(USBTX, USBRX);
static char receivedChar = '\0';

static char tariff_digits[6];
static int last_tariff_digit = 0;

static UserFlowState state = HELP;

static void user_flow_step_waiting_for_user_actions();
static void user_flow_step_print_help();
static void user_flow_step_print_available_tariffs();
static void user_flow_step_print_readings();
static void user_flow_step_set_new_tariff();
static void user_flow_step_activate_tariff();

void user_flow_initialize()
{
    uartUsb.printf("-----------------------------------------------------------------------------------\r\n");
    uartUsb.printf("--------------------------- SMART METER SYSTEM ------------------------------------\r\n");
    uartUsb.printf("-----------------------------------------------------------------------------------\r\n");
    state = HELP;
}

void user_flow_execute()
{

    switch (state)
    {

    case WAITING_FOR_USER_ACTIONS:

        user_flow_step_waiting_for_user_actions();

        break;

    case PRINT_CONSUMPTION_HOME:

        user_flow_step_print_readings();
        state = WAITING_FOR_USER_ACTIONS;

        break;

    case PRINT_AVAILABLE_TARIFS:
        user_flow_step_print_available_tariffs();

        state = WAITING_FOR_USER_ACTIONS;
        break;

    case SET_NEW_TARIF_PRICE:

        user_flow_step_set_new_tariff();

        break;

    case RECORDED_TARIFF:

        state = PRINT_AVAILABLE_TARIFS;

        break;

    case ACTIVATE_TARIFF:

        user_flow_step_activate_tariff();
        break;

    case TARIFF_ACTIVATED:
        state = PRINT_AVAILABLE_TARIFS;
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
    uartUsb.printf("[T] Ingresar una tarifa \r\n");
    uartUsb.printf("[A] Activar una tarifa \r\n");
    uartUsb.printf("[H] Help \r\n");
}

static void user_flow_step_print_available_tariffs()
{
    uartUsb.printf("-----------------------------------------------------------------------\r\n");
    uartUsb.printf("Tarifas disponibles: \r\n");
    uartUsb.printf("-----------------------------------------------------------------------\r\n");

    int active_tariff_index = repository_active_tariff_get();
    int i;
    for (i = 0; i < repository_available_tariffs_count_get(); i++)
    {
        float tariff = repository_available_tariffs_get(i);

        if (i == active_tariff_index)
        {
            uartUsb.printf("%i: %.2f $/kWh  (active)\r\n", i, tariff);
        }
        else
        {
            uartUsb.printf("%i: %.2f $/kWh \r\n", i, tariff);
        }
    }
}

static void user_flow_step_print_readings()
{
    char *format = "%02d %.2f kWh $%.1f";
    char hh[44];
    char dd[44];
    char mm[44];
    char tt[44];

    strncpy(hh, "", 24);
    strncpy(dd, "", 24);
    strncpy(mm, "", 24);
    strncpy(tt, "", 24);

    sprintf(hh, "Ultima Hora: %02d %.2f kWh $%.1f", repository_partial_inter_hour_readings_count_get(), repository_hourly_reading_get(), readings_and_tariffs_get_current_spending(repository_hourly_reading_get()));
    sprintf(dd, "Consumo Diario: %02d %.2f kWh $%.1f", repository_hourly_readings_count_get(), repository_daily_reading_get(), readings_and_tariffs_get_current_spending(repository_daily_reading_get()));
    sprintf(mm, "Consumo Mensual: %02d %.2f kWh $%.1f", repository_daily_readings_count_get(), repository_monthly_reading_get(), readings_and_tariffs_get_current_spending(repository_monthly_reading_get()));
    sprintf(tt, "Consumo Total: %7.2f kWh", repository_total_meter_reading_get());

    uartUsb.printf("-----------------------------------------------------------------------\r\n");
    uartUsb.printf("Metricas de Consumo: \r\n");
    uartUsb.printf("-----------------------------------------------------------------------\r\n");
    uartUsb.printf("%s \r\n", hh);
    uartUsb.printf("%s \r\n", dd);
    uartUsb.printf("%s \r\n", mm);
    uartUsb.printf("%s \r\n", tt);

    state = WAITING_FOR_USER_ACTIONS;
}

static void user_flow_step_waiting_for_user_actions()
{

    if (uartUsb.readable())
    {
        char receivedChar = uartUsb.getc();
        uartUsb.printf("%c\r\n", receivedChar);

        if (receivedChar == 'i' || receivedChar == 'I')
        {
            state = PRINT_CONSUMPTION_HOME;
        }

        else if (receivedChar == 'p' || receivedChar == 'P')
        {
            state = PRINT_AVAILABLE_TARIFS;
        }

        else if (receivedChar == 't' || receivedChar == 'T')
        {
            uartUsb.printf("%c\r\n", receivedChar);
            state = SET_NEW_TARIF_PRICE;
        }

          else if (receivedChar == 'a' || receivedChar == 'A')
        {
            uartUsb.printf("%c\r\n", receivedChar);
            state = ACTIVATE_TARIFF;
        }


        else if (receivedChar == 'h' || receivedChar == 'H')
        {
            state = HELP;
        }
    }
}

static void user_flow_step_set_new_tariff()
{

    if (uartUsb.readable())
    {
        char receivedChar = uartUsb.getc();
        uartUsb.printf("%c", receivedChar);

        //
        // si el caracter ingresado es un numero
        //
        if (receivedChar == '0' || receivedChar == '1'

            || receivedChar == '2' || receivedChar == '3'

            || receivedChar == '4' || receivedChar == '5' || receivedChar == '6'

            || receivedChar == '7' || receivedChar == '8' || receivedChar == '9' || receivedChar == '.')
        {

            if (last_tariff_digit < 6)
            {
                tariff_digits[last_tariff_digit] = receivedChar;
                last_tariff_digit++;
            }
            else
            {

                //
                // resetea,ps e; arrau de caracteres para ingresar la tarifa
                //
                strncpy(tariff_digits, "", strlen(tariff_digits));
                last_tariff_digit = 0;
            }
        }

        //
        // de lo contrario , si ingresamos un Enter para finalizar el ingreso
        //
        else if (receivedChar == '\r')
        {

            float new_tariff = atof(tariff_digits);

            repository_available_tariff_add(new_tariff);

            //
            // resetea,ps e; arrau de caracteres para ingresar la tarifa
            //
            strncpy(tariff_digits, "", strlen(tariff_digits));
            last_tariff_digit = 0;

            state = RECORDED_TARIFF;
        }
    }
}

static void user_flow_step_activate_tariff()
{

    if (uartUsb.readable())
    {
        char receivedChar = uartUsb.getc();
        uartUsb.printf("%c", receivedChar);

        //
        // si el caracter ingresado es un numero
        //
        if (receivedChar == '0' || receivedChar == '1'

            || receivedChar == '2' || receivedChar == '3'

            || receivedChar == '4' || receivedChar == '5' || receivedChar == '6'

            || receivedChar == '7' || receivedChar == '8' || receivedChar == '9')
        {

            char seq[2];
            strncpy(seq, "", strlen(seq));
            seq[0] = receivedChar;
            int tariff_index = atoi(seq);

            repository_active_tariff_set(tariff_index);

            state = TARIFF_ACTIVATED;
        }
    }
}