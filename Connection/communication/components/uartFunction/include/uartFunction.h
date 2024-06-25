#ifndef UARTFUNCTION_H
#define UARTFUNCTION_H

//全部的receive前都不用檢查receiveAvaliable()，receiveUint8 receiveCharArray receiveChar都會自己檢查avaliable
//傳送的型別順序必須和接收的相同
void uartSetup();//uart初始化
void sendUint8(uint8_t );
uint8_t receiveUint8();
void sendCharArray(const char*);
char* receiveCharArray();
void sendChar(char);
char receiveChar();
void setTimeoutMs(int );//設timeout，預設100ms(不建議調短)
void clearBuffer();//清空buffer
int receiveAvaliable();//只回傳當下buffer裡剩餘的bytes喔，如果數據還沒進buffer就不會算進去
//recieveAvaliable應該用不到，只是備用
#endif
