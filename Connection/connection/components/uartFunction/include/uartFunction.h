#ifndef UARTFUNCTION_H
#define UARTFUNCTION_H
void uartSetup();//uart初始化(放在setup(){}內)

void sendUint8(uint8_t );//傳送uint8
uint8_t receiveUint8();//接收uint8(超過timeout回傳0)

void sendCharArray(const char*);//傳送char陣列(需<40個bytes)
char* receiveCharArray();//接收char陣列(超過timeout回傳NULL)

void sendChar(char);//傳送char
char receiveChar();//接收char(超過timeout回傳\0)

void setTimeoutMs(int );//設timeout，預設200ms(不建議調短)
void clearBuffer();//清空buffer(每個loop最後都要有)
int receiveAvaliable();//查看buffer內還有幾個bytes(應該用不到)

void sendRequest() ;//主機發起邀請
bool acceptRequest() ;//從機接受邀請

void sendInit() ;//主機發起遊戲開始
bool ackInit() ;//從機接受遊戲開始

void sendSync() ;//主機發起對時
bool ackSync() ;//從機接收對時

/*傳送型別順序要和接收順序一樣，例:
sendUint8(12);
sendCharArray("try 123");
sendChar('u');
sendCharArray("321 yrt");

vTaskDelay(100/portTICK_PERIOD_MS);

printf(%u\n,receiveUint8());
printf(%s\n,receiveCharArray());
printf(%c\n,receiveChar());
printf(%s\n,receiveCharArray());
*/
//buffer為128bytes，每個loop不要傳超過128個字節
//所有的receive前都不用檢查receiveAvaliable(全部的receive函式會自己檢查)
#endif
