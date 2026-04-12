#include "zf_common_headfile.h"
#include "pid.h"
#include "math.h"
#include "encoder.h"
#include "imu660ra.h"
#include "spi.h"
#include "uart.h"

#define MAX_PACKET_LEN 64

static uint8 uart1_rx_buf[MAX_PACKET_LEN];
static uint8 uart4_rx_buf[MAX_PACKET_LEN];

// 2. 定义索引变量，记录当前收到第几个字节
static uint8 uart1_idx = 0;
static uint8 uart4_idx = 0;

// 3. 状态定义 (保持你的逻辑)
static uint8 uart1_rx_state = 0; 
static uint8 uart4_rx_state = 0; 

// 全局标志位
uint8 packge1_finish_flag = 0, packge4_finish_flag = 0;
uint8 uart4_data_map_raw[6]; 
uint8 uart1_data_art_raw[6]; 

void my_uart_init()
{
    uart_init(UART_1, 115200, UART1_TX_B12, UART1_RX_B13);
    uart_init(UART_4, 115200, UART4_TX_C16, UART4_RX_C17);
    
//     fifo_init(&uart1_fifo, FIFO_DATA_8BIT, uart1_buffer, UART_BUFFER_SIZE);
//     fifo_init(&uart4_fifo, FIFO_DATA_8BIT, uart4_buffer, UART_BUFFER_SIZE);
//    
     uart_rx_interrupt(UART_1, 1); // 1表示开启
     uart_rx_interrupt(UART_4, 1);
}

void my_uart_callback(uart_index_enum uart_n)
{
      uint8 temp_data;
    // 读取硬件寄存器中的字节
    if(uart_query_byte(uart_n, &temp_data) != 0)
    {
        switch (uart_n)
        {
            case UART_1:
            {
                // --- UART1 状态机 ---
                if(uart1_rx_state == 0) // 等待 0xAA
                {
                    if(temp_data == 0xAA) 
                    {
                        uart1_idx = 0; // 重置索引
                        uart1_rx_buf[uart1_idx++] = temp_data; // 存入帧头
                        uart1_rx_state = 1;
                    }
                }
                else if(uart1_rx_state == 1) // 等待 0x55
                {
                    if(temp_data == 0x55)
                    {
                        uart1_rx_buf[uart1_idx++] = temp_data; // 存入帧头
                        uart1_rx_state = 2;
                    }
                    else
                    {
                        uart1_rx_state = 0; // 匹配失败，回退
                        // 防止漏掉连续的 0xAA
                        if(temp_data == 0xAA) uart1_rx_state = 1; 
                    }
                }
                else if(uart1_rx_state == 2) // 接收中间数据
                {
                    // 检查缓冲区是否溢出
                    if(uart1_idx < MAX_PACKET_LEN)
                    {
                        if(temp_data == 0x0A) // 假设 0x0A 是帧尾
                        {
                            uart1_rx_buf[uart1_idx++] = temp_data; // 存入帧尾
                            uart1_rx_state = 3; // 进入完成状态
                        }
                        else
                        {
                            uart1_rx_buf[uart1_idx++] = temp_data; // 存入数据
                        }
                    }
                    else
                    {
                        uart1_rx_state = 0; // 溢出错误，重置
                    }
                }
                else if(uart1_rx_state == 3) // 完成处理
                {
                    // 将缓冲区数据拷贝到全局变量供主循环使用
                    // 假设包长固定为 6，或者你可以用 uart1_idx
                    memcpy(uart1_data_art_raw, uart1_rx_buf, 6); 
                    
                    packge1_finish_flag = 1; // 置位标志
                    uart1_rx_state = 0;      // 重置状态，准备接收下一帧
                }
                break;
            }
            
            case UART_4:
            {
                // --- UART4 状态机 (逻辑同上) ---
                if(uart4_rx_state == 0) 
                {
                    if(temp_data == 0xAA) 
                    {
                        uart4_idx = 0;
                        uart4_rx_buf[uart4_idx++] = temp_data;
                        uart4_rx_state = 1;
                    }
                }
                else if(uart4_rx_state == 1) 
                {
                    if(temp_data == 0x55)
                    {
                        uart4_rx_buf[uart4_idx++] = temp_data;
                        uart4_rx_state = 2;
                    }
                    else
                    {
                        uart4_rx_state = 0;
                        if(temp_data == 0xAA) uart4_rx_state = 1;
                    }
                }
                else if(uart4_rx_state == 2) 
                {
                    if(uart4_idx < MAX_PACKET_LEN)
                    {
                        if(temp_data == 0x0A)
                        {
                            uart4_rx_buf[uart4_idx++] = temp_data;
                            uart4_rx_state = 3;
                        }
                        else
                        {
                            uart4_rx_buf[uart4_idx++] = temp_data;
                        }
                    }
                    else
                    {
                        uart4_rx_state = 0;
                    }
                }
                else if(uart4_rx_state == 3) 
                {
                    memcpy(uart4_data_map_raw, uart4_rx_buf, 6);
                    packge4_finish_flag = 1;
                    uart4_rx_state = 0;
                }
                break;
            }
        }
    }

}