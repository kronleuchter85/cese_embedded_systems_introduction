#ifndef _REPOSIOTRY
#define _REPOSIOTRY

void repository_initialize_readings();

void repository_add_partial_inter_hour_reading(float m);


void repository_add_daily_reading_to_month(float m);

float repository_partial_inter_hour_reading_get(int index);


void repository_daily_reading_set(float reading);

void repository_monthly_reading_set(float reading);

float repository_hourly_reading_get();

float repository_daily_reading_get();

float repository_monthly_reading_get();


int repository_partial_inter_hour_readings_count_get();

int repository_hourly_readings_count_get();

int repository_daily_readings_count_get();


void repository_add_hourly_reading(float f);

float repository_total_meter_reading_get();

#endif // _DISPLAY_H_