# 鍵盤組
[接線與腳位](https://bright-planet-5ad.notion.site/PCB-a127fce569ee4b1ebe905d3c21eb58ca)\
[運作原理](https://ergotaiwan.tw/self-keyboard-basic-1/)\
[Notion](https://bright-planet-5ad.notion.site/b7f4ea1eec5748988c577210553e8992)
## 程式碼介紹
bool* curr_key_state: 當前鍵盤狀態。\
bool* last_key_state: 上一個鍵盤狀態。\
void handleUltimateSkill(): 用於處理鍵盤輸入是否符合大招指令，於detectKeyboard()中被呼叫。\
void detectKeyBoard(): 處理鍵盤輸入，改變curr_key_state與last_key_state，並執行handleUltimateSkill()。\
int getUltimateSkill(): 回傳ultSkillExecute（被觸發的大招，沒觸發為-1），並將此值重置為-1。
## 可調參數
BOUNCE_DELAY_MS: 判斷鍵盤bounce的毫秒數，按鍵變動持續時間超過此值才視為變動\
ULT_DELAY_MS: 大招觸發的指令間可容許最大毫秒數（接技間隔）\
ULT_LEN: 大招指令長度\
ULT_NUM: 大招數量\
ULT_CODE[ULT_NUM][ULT_LEN]: 大招按鍵指令
## 大招相關機制
目前的設計如下，若需修改可告知。
1. 指令不可重疊：如果有一個大招指令是{1, 2, 3}，另一個是{3, 4, 5}，則輸入1, 2, 3, 4, 5只會觸發第一個大招。
2. 多個按鍵同時按下：若將所有輸入按鍵透過某種排序可做為一個大招接下來的部分或全部輸入，或是部分輸入按鍵透過排序可完成觸發一個大招，則針對該大招的輸入判定為成功輸入該排序的指令，否則判定為失敗。
3. 大招指令間容許時間：目前設定為1000毫秒，即代表接技時任兩個按鍵之間隔最大為1秒，可再測試看看是否要縮短。