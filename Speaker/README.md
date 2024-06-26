## 音響組

### 摘要

利用 MAX98375 作為 Codec Chip  
mp3 音檔撥放流程：  
```
[flash] ---> spiffs_stream ---> mp3_decoder ---> i2s_stream ---> [codec_chip]
```  
使用前須先載好 ESP-ADF 擴充，為專門為 audio 所開發的 framwork ， 以下會有說明  
預計包含音檔上傳、播放暫停、重新播放、音量控制等 function

### 目前進度

- [ ] Download ESP-ADF
- [ ] README 
- [ ] File Upload
- [ ] Play / Pause / Stop
- [ ] Volume Control

### ESP-ADF 環境設置

#### 方法一（可能失敗）

1. In vscode, open ```command palette```(in ```View``` at the top), enter ```install adf```. Then, a progress bar shows up in the lower right corner.
2.  You can open the example project of ESP-ADF now.

#### 方法二

