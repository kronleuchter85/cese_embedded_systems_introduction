#include <stdlib.h>
#include <time.h>
#include "repository.h"

//======================== Repository ================================

static float available_tariffs[10];

static float partial_inter_hour_readings[10];
static int partial_inter_hour_readings_count = 0;

static float last_hour_reading = 0;

//
// al dia podemos tener un maximo de 24 lecturas
//
static int hourly_readings_count = 0;

static float daily_reading_accum = 0;

//
// al mes podemos tener un maximo de 30 lecturas
//
static int daily_readings_count = 0;

static float monthly_reading_accum = 0;

static float total_meter_accum = 0;

float repository_total_meter_reading_get(){
    return total_meter_accum;
}

void repository_initialize_readings()
{
    int i;
    for (i = 0; i < 10; i++)
    {
        partial_inter_hour_readings[i] = 0;
    }
}

void repository_add_partial_inter_hour_reading(float m)
{
    //
    // si es la primera vez que llamamos a add_reading o ya agregamos diez lecturas
    // entonces inicializamos el arreglo poniendo todas las lecturas en cero
    // y reseteamos el indice a cero tambien.
    //
    if (partial_inter_hour_readings_count == 0 || partial_inter_hour_readings_count >= 10)
    {
        repository_initialize_readings();
        partial_inter_hour_readings_count = 0;
    }

    //
    // agregamos la lectura m en el indice actual, e incrementamos el valor del indice
    //
    partial_inter_hour_readings[partial_inter_hour_readings_count] = m;
    partial_inter_hour_readings_count++;
}


float repository_partial_inter_hour_reading_get(int index)
{
    return partial_inter_hour_readings[index];
}

float repository_hourly_reading_get()
{
    return last_hour_reading;
}

float repository_daily_reading_get()
{
    return daily_reading_accum;
}

float repository_monthly_reading_get()
{
    return monthly_reading_accum;
}

void repository_initialize()
{
    srand(time(NULL));

    int i;
    for (i = 0; i < 10; i++)
    {
        available_tariffs[i] = 1 + rand();
    }
}

float *repository_available_tariffs_get()
{
    return available_tariffs;
}

int repository_partial_inter_hour_readings_count_get()
{
    return partial_inter_hour_readings_count;
}

int repository_hourly_readings_count_get()
{
    return hourly_readings_count;
}

int repository_daily_readings_count_get()
{
    return daily_readings_count;
}

void repository_add_hourly_reading(float hourly_reading)
{

    //
    // si pasaron 24 horas
    //
    if (hourly_readings_count >= 24)
    {

        //
        // reseteamos los contadores y acumulados del dia
        //
        daily_reading_accum = 0;
        hourly_readings_count = 0;
        
        
        //
        // incrementamos un dia del mes
        //
        daily_readings_count++;
    }


    //
    // si pasaron 30 dias
    //
    if (daily_readings_count >= 30 )
    {
        //
        // reseteamos el acumulado mensual
        //
        monthly_reading_accum = 0;
        daily_readings_count = 0;

        //
        // podemos actualizar el contador de meses del anio
        //
    }

    last_hour_reading = hourly_reading;

    daily_reading_accum = daily_reading_accum + hourly_reading;
    
    monthly_reading_accum = monthly_reading_accum + hourly_reading;
    
    hourly_readings_count++;

    total_meter_accum = total_meter_accum + hourly_reading;
}


