## 音響組

### 摘要

利用 MAX98375 作為 Codec Chip  
mp3 音檔撥放流程：  
```
[flash] ---> spiffs_stream ---> mp3_decoder ---> i2s_stream ---> [codec_chip]
```  
使用前須先載好 ESP-ADF 擴充，為專門為 audio 所開發的 framwork ， 以下會有說明  
預計包含音檔上傳、播放暫停、重新播放、音量控制等 function  

完成 ```ESP-ADF 環境設置``` 以及 ```Menuconfig 設置``` 便可開發 audio 相關 

### 目前進度

- [ ] Download ESP-ADF
- [ ] README 
- [ ] File Upload
- [ ] Play / Pause / Stop
- [ ] Volume Control

<details>
    <summary><h3>ESP-ADF 環境設置</h3></summary>

#### 方法一（可能失敗）

1. In vscode, open ```command palette``` (in ```View``` at the top), enter ```install adf```. Then, a progress bar shows up in the lower right corner.

2.  You can open the example project of ESP-ADF now.

#### 方法二

1. Open command prompt, run the command below at the folder ```..\esp```  

    ```git clone --recursive https://github.com/espressif/esp-adf.git```.

2. Open ```command palette``` and enter ```open settings(ui)```.

3. Type ```idf.espAdfPath``` to find the setting of ```Esp Adf Path```.  

    ![Esp Adf Path Setting](image/idf_espAdfPath.png)

4. Paste the path of the ```esp-adf``` that you clone at the first step.

5. You can open the example project of ESP-ADF now.
</details>

<details>
    <summary><h3>I2S 初始腳位設置</h3></summary>

目的：更改初始 I2S 腳位設定

1. 將 ```pipeline_spiffs_mp3/components/my_board``` 中的 ```my_board``` 放進自己專案中的 ```components``` 作為客製化的開發板（應不影響原本的開發）。

2. 開啟 ```SDK Configuration Editor (menuconfig)```，搜尋 ```Audio HAL``` ，在 ```Audio board``` 中選擇 ```Custom audio board```。

    ![Menuconfig 設置](image/Audio_HAL.png)

3. 回到 ```/my_board``` 中，開啟 ```/my_board/my_board_v1_0/board_pins_config.c```，Ctrl F 尋找 ```get_i2s_pins```，確認 pin 腳是以下的設定（注意 bck、ws、data_out）：

    ```c
    esp_err_t get_i2s_pins(int port, board_i2s_pin_t *i2s_config)
    {
        AUDIO_NULL_CHECK(TAG, i2s_config, return ESP_FAIL);
        if (port == 0) {
            i2s_config->mck_io_num = GPIO_NUM_0;
            i2s_config->bck_io_num = GPIO_NUM_27;
            i2s_config->ws_io_num = GPIO_NUM_26;
            i2s_config->data_out_num = GPIO_NUM_25;
            i2s_config->data_in_num = GPIO_NUM_39;
        } else if (port == 1) {
            i2s_config->bck_io_num = -1;
            i2s_config->ws_io_num = -1;
            i2s_config->data_out_num = -1;
            i2s_config->data_in_num = -1;
        } else {
            memset(i2s_config, -1, sizeof(board_i2s_pin_t));
            ESP_LOGE(TAG, "i2s port %d is not supported", port);
            return ESP_FAIL;
        }

        return ESP_OK;
    }
    ```
</details>

<details>
    <summary><h3>Spiffs 設置</h3></summary>

目的：將音檔存進 flash 內

1. 新增 ```partitions.csv``` ，填入以下：

    ```csv
    # Name,   Type, SubType, Offset,  Size, Flags
    # Note: if you change the phy_init or app partition offset, make sure to change the offset in Kconfig.projbuild
    nvs,      data, nvs,     ,        0x6000,
    phy_init, data, phy,     ,        0x1000,
    factory,  app,  factory, ,        1M,
    storage,  data, spiffs,  0x110000,1M, 
    ```
    其中 ```storage``` 為我們額外分割出的空間提供音檔進行儲存

2. 在工作區新增名為 ```tools``` 的資料夾，將所需音檔放入

3. 在 ```/main/CMakeLists.txt``` 中加入這行： ```spiffs_create_partition_image(storage ../tools FLASH_IN_PROJECT)```

4. 開啟 ```SDK Configuration Editor (menuconfig)```，搜尋 ```Partition table```，確認已被設定為以下的狀態：

    ![Menuconfig 設置](image/spiffs_setting.png)
</details>

