#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include <string.h>
#include "uartFunction.h"
// 全域變數
char* array;
char * a;
uint8_t numm;
uint8_t num;
char ch;
char received_ch;
void setup() {
    // 配置 UART
    uartSetup();
    // 初始化测试数据
    array = (char*)malloc(40 * sizeof(char));//開超過需要的記憶體空間(需要38)
    strcpy(array, "data try data try sign try !@#$^ &*()");
    numm = 85;
    ch = 'y';
    //初始化timeout
    setTimeoutMs(200);
}

void loop() {
    sendCharArray(array);//傳送兩次
    sendUint8(numm);
    sendChar(ch);
    sendCharArray(array);//多傳送為了測試clearbuffer
    sendUint8(numm);
    sendChar(ch);

    vTaskDelay(500/portTICK_PERIOD_MS);//每個loop 500ms(並等待數據傳進buffer)
    printf("initial bufferavaliable:%d\n\n",receiveAvaliable());//應是80

    a = receiveCharArray();//接收char array
    printf("Received array: %s\n", a ? a : "NULL");
    printf("bufferavaliable after receive charArray:%d\n\n",receiveAvaliable());//應是42

    num = receiveUint8();//接收uint8
    printf("Received num: %u\n", num);
    printf("bufferavaliable after receive charArray uint8:%d\n\n",receiveAvaliable());//應是41

    received_ch = receiveChar();//接收char
    printf("Received char: %c\n", received_ch);
    printf("bufferavaliable after receive charArray uint8 char:%d\n\n",receiveAvaliable());//應是40

    clearBuffer();
    printf("bufferavaliable after clearBuffer:%d\n\n\n\n",receiveAvaliable());//應是0
    free(a);
}
void app_main() {
    setup();
    while (true) {
        loop();
    }
}
