#ifndef UART_H
#define UART_H

#include "zf_common_headfile.h"

#define MAX_PACKET_LEN 64
extern uint8 packge1_finish_flag, packge4_finish_flag; 
extern uint8 uart1_data_art_raw[6], uart4_data_map_raw[6]; 
extern union data_pack art1_data_pack;
extern uint8 dat;
void my_uart_init();
void my_uart_callback(uart_index_enum uart_n);


#endif