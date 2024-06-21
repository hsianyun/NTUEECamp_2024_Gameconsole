#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/uart.h"

#include "uartFunction.h"

#define UART_PORT UART_NUM_2
#define BUF_SIZE (128)//緩衝區128bytes( 1個loop最多只能收128bytes)
int timeoutMs=100;

void uartSetup() {
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_PORT, &uart_config);
    uart_set_pin(UART_PORT, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);// 设置 UART 引脚
    uart_driver_install(UART_PORT, BUF_SIZE * 2, 0, 0, NULL, 0);// 安装 UART 驱动程序
}

void sendUint8(uint8_t num) {
    uart_write_bytes(UART_PORT, (const char*)&num, 1);//傳uint8
}

uint8_t receiveUint8() {
    char ch;
    int len = uart_read_bytes(UART_PORT, &ch, 1, timeoutMs/portTICK_PERIOD_MS);
    if (len > 0) {
        return (uint8_t)ch;//接收uint8
    } else return 0;//超過timeout
}
void sendCharArray(const char* a) {
    int i = 0;
    while (a[i] != '\0') {
        uart_write_bytes(UART_PORT, &a[i], 1);//charArray一項一項傳送
        i++;
    }
    uart_write_bytes(UART_PORT, "\0", 1); // 傳送终止符
}

char* receiveCharArray() {
    char* str = (char*)malloc(BUF_SIZE);//動態陣列設空間為128bytes，暫時的(會釋放)
    int index = 0;//紀錄接收幾個字元(從0開始)
    char a;
    while (true) {
        int len = uart_read_bytes(UART_PORT, &a, 1,timeoutMs/portTICK_PERIOD_MS);
        if (len > 0) {
            str[index++] = a;
            if (a == '\0' || index >= BUF_SIZE)break;
        } else {
            free(str);
            return NULL;//超過timeout
        }
    }
    char* ans=(char*)malloc(index+1);//ans的記憶體空間=陣列長度+1(截止符)
    for(int i=0;i<index+1;i++)ans[i]=str[i];//把str移到ans
    free(str);
    return ans;//回傳的ans不會有空著但被占用的記憶體
}

void sendChar(char a) {
    uart_write_bytes(UART_PORT, &a, 1);//傳char
}

char receiveChar() {
    char ans;
    int len = uart_read_bytes(UART_PORT, &ans, 1, timeoutMs/portTICK_PERIOD_MS);
    if (len > 0) {
        return ans;//接收char
    } else return '\0';//超過timeout
}
void setTimeoutMs(int time){
    timeoutMs=time;//設置timeout(預設100ms)
}
int receiveAvaliable(){//回傳buffer剩餘的bytes
    size_t bufferSize;
    uart_get_buffered_data_len(UART_PORT,&bufferSize);
    return (int)bufferSize;
}
void clearBuffer() {//清空緩衝區
    uint8_t data[BUF_SIZE];
    while (uart_read_bytes(UART_PORT, (char*)data, BUF_SIZE, 1) > 0) {}
}