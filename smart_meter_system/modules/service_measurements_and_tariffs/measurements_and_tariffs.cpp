
#include "mbed.h"
#include "arm_book_lib.h"

#include "measurements_and_tariffs.h"


static AnalogIn potentiometer(A0);
static float potentiometerReading = 0.0;

static float max_consumo_kwh_month = 400.0;
static float max_consumo_kwh_day = max_consumo_kwh_month / 30.0;
static float max_consumo_kwh_hour = max_consumo_kwh_day / 24.0;

static float current_tariff = 3.05;


void measurements_and_tariffs_initialize(){
    
}

void measurements_and_tariffs_execute(){
    potentiometerReading = potentiometer.read();
}

float measurements_and_tariffs_get_current_measure()
{
    return max_consumo_kwh_hour * potentiometerReading;
}

float measurements_and_tariffs_get_current_spending() {
    return measurements_and_tariffs_get_current_measure() * current_tariff;
}


