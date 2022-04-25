

#ifndef _UART_USER_SERVICE
#define _UART_USER_SERVICE




typedef enum
{
    WAITING_FOR_USER_ACTIONS,
    PRINT_CONSUMPTION_HOME,
    PRINT_AVAILABLE_TARIFS,
    SET_NEW_TARIF_PRICE,
    RECORDED_TARIFF,
    ACTIVATE_TARIFF,
    TARIFF_ACTIVATED,
    HELP,
} UserFlowState;

void user_flow_initialize();
void user_flow_execute();

#endif // _DISPLAY_H_