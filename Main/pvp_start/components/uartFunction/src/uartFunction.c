#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/uart.h"

#include <string.h>
#include "uartFunction.h"

#define UART_PORT UART_NUM_2
#define BUF_SIZE (128)//緩衝區128bytes( 1個loop最多只能收128bytes)

// 發送訊息的種類
#define SIGNAL 1
#define DATA 2

// request, init, sync
#define REQUEST 3
#define INIT 4
#define SYNC 5

// 接收到訊息種類錯誤的回傳值
#define UINT8_RECEIVE_WRONG_TYPE 0
#define CHAR_ARRAY_RECEIVE_WRONG_TYPE NULL
#define CHAR_RECEIVE_WRONG_TYPE '\0'
#define SIGNAL_RECEIVE_WRONG_TYPE 0

int timeoutMs=100;
uint8_t msgType = 0; //預設發送種類為DATA
uint8_t signalType = 0;
void uartSetup() {
    uart_config_t uart_config = {
        .baud_rate = 1843200,
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
    msgType = DATA ;
    uart_write_bytes(UART_PORT, (const char*)&msgType, 1);//前綴訊息
    uart_write_bytes(UART_PORT, (const char*)&num, 1);//傳uint8
}

uint8_t receiveUint8() {

    // 讀取前綴msgType 確認是否為DATA
    char checkMsgType;
    if (uart_read_bytes(UART_PORT, &checkMsgType, 1, timeoutMs/portTICK_PERIOD_MS)) {
        if ((uint8_t)checkMsgType != DATA) {
            return UINT8_RECEIVE_WRONG_TYPE ;
        }
        // printf("Receive DATA\n");
    }

    char ch;
    int len = uart_read_bytes(UART_PORT, &ch, 1, timeoutMs/portTICK_PERIOD_MS);
    if (len > 0) {
        return (uint8_t)ch;//接收uint8
    } else{
        return 0;//超過timeout
    }
}

void sendCharArray(const char* a) {
    msgType = DATA ;
    uart_write_bytes(UART_PORT, (const char*)&msgType, 1);//前綴訊息
    uart_write_bytes(UART_PORT, a, strlen(a) + 1); // 发送字符串包括终止符
}

char* receiveCharArray() {

    // 讀取前綴msgType 確認是否為DATA
    char checkMsgType;
    if (uart_read_bytes(UART_PORT, &checkMsgType, 1, timeoutMs/portTICK_PERIOD_MS)) {
        if ((uint8_t)checkMsgType != DATA) {
            return CHAR_ARRAY_RECEIVE_WRONG_TYPE ;
        }
    }

    char* str = (char*)malloc(40);//動態陣列設空間為40bytes
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
    return str;
}

void sendChar(char a) {
    msgType = DATA ;
    uart_write_bytes(UART_PORT, (const char*)&msgType, 1);//前綴訊息
    uart_write_bytes(UART_PORT, &a, 1);//傳char
}

char receiveChar() {

    // 讀取前綴msgType 確認是否為DATA
    char checkMsgType;
    if (uart_read_bytes(UART_PORT, &checkMsgType, 1, timeoutMs/portTICK_PERIOD_MS)) {
        if ((uint8_t)checkMsgType != DATA) {
            return CHAR_RECEIVE_WRONG_TYPE ;
        }
    }

    char ans;
    int len = uart_read_bytes(UART_PORT, &ans, 1, timeoutMs/portTICK_PERIOD_MS);
    if (len > 0) {
        return ans;//接收char
    } else {
        return '\0';//超過timeout
    }
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

/* -------- 以下為signal接收函式 -------- */

void sendRequest() {
    msgType = SIGNAL ;
    signalType = REQUEST ;
    uart_write_bytes(UART_PORT, (const char*)&msgType, 1);//前綴訊息
    uart_write_bytes(UART_PORT, (const char*)&signalType, 1);//傳signal
}

bool acceptRequest() {
    // 讀取前綴msgType 確認是否為SIGNAL
    char checkMsgType;
    if (uart_read_bytes(UART_PORT, &checkMsgType, 1, timeoutMs/portTICK_PERIOD_MS)) {
        if ((uint8_t)checkMsgType != SIGNAL) {
            return SIGNAL_RECEIVE_WRONG_TYPE;
        }
        // printf("Receive SIGNAL\n");
    }
    // 檢查signalType 確認是否為REQUEST
    char checkSignalType;
    if(uart_read_bytes(UART_PORT, &checkSignalType, 1, timeoutMs/portTICK_PERIOD_MS)) {
        if ((uint8_t)checkSignalType == REQUEST) {
            return 1 ;
        }
    }
    return 0 ;
}

void sendInit() {
    msgType = SIGNAL ;
    signalType = INIT ;
    uart_write_bytes(UART_PORT, (const char*)&msgType, 1);//前綴訊息
    uart_write_bytes(UART_PORT, (const char*)&signalType, 1);//傳signal
}

bool ackInit() {
    // 讀取前綴msgType 確認是否為SIGNAL
    char checkMsgType;
    if (uart_read_bytes(UART_PORT, &checkMsgType, 1, timeoutMs/portTICK_PERIOD_MS)) {
        if ((uint8_t)checkMsgType != SIGNAL) {
            return SIGNAL_RECEIVE_WRONG_TYPE;
        }
    }
    // 檢查signalType 確認是否為INIT
    char checkSignalType;
    if(uart_read_bytes(UART_PORT, &checkSignalType, 1, timeoutMs/portTICK_PERIOD_MS)) {
        if ((uint8_t)checkSignalType == INIT) {
            return 1 ;
        }
    }
    return 0 ;
}

void sendSync() {
    msgType = SIGNAL ;
    signalType = SYNC ;
    uart_write_bytes(UART_PORT, (const char*)&msgType, 1);//前綴訊息
    uart_write_bytes(UART_PORT, (const char*)&signalType, 1);//傳signal
}

bool ackSync() {
    // 讀取前綴msgType 確認是否為SIGNAL
    char checkMsgType;
    if (uart_read_bytes(UART_PORT, &checkMsgType, 1, timeoutMs/portTICK_PERIOD_MS)) {
        if ((uint8_t)checkMsgType != SIGNAL) {
            return SIGNAL_RECEIVE_WRONG_TYPE;
        }
    }
    // 檢查signalType 確認是否為SYNC
    char checkSignalType;
    if(uart_read_bytes(UART_PORT, &checkSignalType, 1, timeoutMs/portTICK_PERIOD_MS)) {
        if ((uint8_t)checkSignalType == SYNC) {
            return 1 ;
        }
    }
    return 0 ;
}
