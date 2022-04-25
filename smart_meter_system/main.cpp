//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "display.h"
#include "uart_user_service.h"
#include "readings_and_tariffs.h"
#include "repository.h"

//===============================================================

static Ticker take_partial_reading_interruption;

bool RELEASE_PARTIAL = false;

/**
 *
 * Este metodo es llamado por una interrupcion que libera la lectura parcial dentro del
 * intervalo de una hora. Esta lectura parcial es almacenada para poder calcular un estimado
 * entre todas las lecturas parciales al final de la hora.
 *
 */
void release_partial_reading()
{
    RELEASE_PARTIAL = true;
}

static void display_update()
{
    char *format = "%02d %.2f kWh $%.1f";
    char hh[14];
    char dd[14];
    char mm[14];
    char tt[14];

    int current_tariff_index = repository_active_tariff_get();
    float current_tariff = repository_available_tariffs_get(current_tariff_index);

    sprintf(tt, "%.2f kWh | %.2f $/kWh", repository_total_meter_reading_get(), current_tariff );
    displayCharPositionWrite(0, 0);
    displayStringWrite(tt);

    sprintf(hh, format, repository_partial_inter_hour_readings_count_get(), repository_hourly_reading_get(), readings_and_tariffs_get_current_spending(repository_hourly_reading_get()));
    displayCharPositionWrite(0, 1);
    displayStringWrite(hh);

    sprintf(dd, format, repository_hourly_readings_count_get(), repository_daily_reading_get(), readings_and_tariffs_get_current_spending(repository_daily_reading_get()));
    displayCharPositionWrite(0, 2);
    displayStringWrite(dd);

    sprintf(mm, format, repository_daily_readings_count_get(), repository_monthly_reading_get(), readings_and_tariffs_get_current_spending(repository_monthly_reading_get()));
    displayCharPositionWrite(0, 3);
    displayStringWrite(mm);

}

void display_init()
{
    displayInit(DISPLAY_CONNECTION_I2C_PCF8574_IO_EXPANDER);
}

int main()
{

    repository_initialize();
    user_flow_initialize();
    display_init();

    take_partial_reading_interruption.attach(&release_partial_reading, 0.1);

    while (true)
    {

        user_flow_execute();

        if (RELEASE_PARTIAL)
        {

            readings_and_tariffs_execute();
            float reading = readings_and_tariffs_get_current_reading();
            repository_add_partial_inter_hour_reading(reading);

            int intra_readings_count = repository_partial_inter_hour_readings_count_get();

            //
            // si ya tenemos 10 lecturas parciales entonces estimamos el consumo de la hora
            //
            if (intra_readings_count >= 10)
            {

                float estimatedReading = repository_partial_inter_hour_reading_get(9);
                repository_add_hourly_reading(estimatedReading);
            }

            display_update();

            RELEASE_PARTIAL = false;

        }

        
    }
}

//=====================================================================

//======================== UART ================================
