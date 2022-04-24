//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include <stdlib.h>

#include "display.h"

#define DISPLAY_REFRESH_TIME_MS 1000





//======================== Repository ================================

static float available_tariffs[10];

void repository_initialize(){
    srand (time(NULL));
    
    int i; for(i=0 ; i<10 ; i++){
        available_tariffs[i] = 1 + rand();
    }
}

float * repository_available_tariffs_get(){
    return available_tariffs;
}

//=====================================================================





//======================== UART ================================

typedef struct {
    WAITING_FOR_USER_ACTIONS,
    PRINT_CONSUMPTIONS_HOME,
    PRINT_AVAILABLE_TARIFS,
    SET_NEW_TARIF_PRICE,
    HELP,    
} UserFlowState;

static UserFlowState state;

static void user_flow_step_waiting_for_user_actions();

void user_flow_initialize(){
    state = WAITING_FOR_USER_ACTIONS;
}

void user_flow_execute(){
    
    switch(state){
        
        case WAITING_FOR_USER_ACTIONS:
        
            user_flow_step_waiting_for_user_actions();
        
            break;
        
        case PRINT_CONSUMPTIONS_HOME:
        
            break;
        
        case PRINT_AVAILABLE_TARIFS:
        
            break;
       
        case SET_NEW_TARIF_PRICE:
        
            break;
        
        case HELP:
            user_flow_step_print_help();
            break;
    }    
}

static void user_flow_step_print_help(){
    uartUsb.printf("Opciones disponibles: \r\n");
    uartUsb.printf("[I] Imprimir Consumo en Tiempo Real \r\n");
    uartUsb.printf("[P] Imprimir Tarifas Disponibles \r\n");
    uartUsb.printf("[T] Activar una tarifa \r\n");
    uartUsb.printf("[H] Help \r\n");
}


static void user_flow_step_waiting_for_user_actions(){
    
    if( uartUsb.readable() ) {
        char receivedChar = uartUsb.getc();   
        
        if(receivedChar == 'i' || receivedChar == 'I'){
            uartUsb.printf("%c\r\n", receivedChar);
        } 
        
        else if(receivedChar == 'p' || receivedChar == 'P'){
            uartUsb.printf("%c\r\n", receivedChar);
        } 
        
        else if(receivedChar == 't' || receivedChar == 'T'){
            uartUsb.printf("%c\r\n", receivedChar);
        } 
        
        else if(receivedChar == 'h' || receivedChar == 'H'){
            state = HELP;
        }         
    } 
}


//===============================================================




float max_consumo_kwh = 20.0;

Serial uartUsb(USBTX, USBRX);
AnalogIn potentiometer(A0);

char receivedChar = '\0';
float potentiometerReading = 0.0;


void pote_update(){
    potentiometerReading = potentiometer.read();
}

void serial_update(){
    
    if( uartUsb.readable() ) {
        char receivedChar = uartUsb.getc();   
        uartUsb.printf("Received: %c\r\n", receivedChar);
    }else{
        uartUsb.printf("Nada... \r\n");
    }
    
      
    uartUsb.printf("Pote Reading: %f\r\n", potentiometerReading);
}

static void display_update()
{
    static int accumulatedDisplayTime = 0;
    char temperatureString[6];
    
    if( accumulatedDisplayTime >=
        DISPLAY_REFRESH_TIME_MS ) {

        accumulatedDisplayTime = 0;

        sprintf(temperatureString, "%.2f", potentiometerReading);
        
        displayCharPositionWrite ( 6,0 );
        displayStringWrite( temperatureString );
        
        displayCharPositionWrite ( 11,0 );
        displayStringWrite( "U" );

        displayCharPositionWrite ( 6,1 );
        displayStringWrite( "Algo" );

    } else {
        accumulatedDisplayTime =
            accumulatedDisplayTime + 40;        
    } 
}

void display_init(){
    
    displayInit( DISPLAY_CONNECTION_I2C_PCF8574_IO_EXPANDER );
     
    displayCharPositionWrite ( 0,0 );
    displayStringWrite( "Pote:" );

    displayCharPositionWrite ( 0,1 );
    displayStringWrite( "Mes:" );
    
}

int main()
{
    display_init();
    
    while (true) {
        
        pote_update();
        
        serial_update();
        
        display_update();
        
        delay(1000);
    }
}