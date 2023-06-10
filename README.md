# EPD_Final_Project


##    Pin assignment

####    ==溫溼度感測模組(DHT22)==
*    **1-wire 通訊**
     * GPIO_Output -> **PC3**


####    ==Wi-Fi 晶片==
*    **UART1**
     * USART_TX -> **PA9**
     * USART_RX -> **PA10**
     * Buad rate：**9600 Bit/s**
          ![](https://hackmd.io/_uploads/r1vrtrzr3.png)
* **Reset Pin** 
     * GPIO_Output -> **PC12**
---
##    硬體實現功能
###    聯網部分 (ESP8266)
*    ESP8266 跟基地台連線的部分 -> 先設定好，並已存入 flash 中。
     > 為了加速連線 (減少AT指令數目)
*    傳送感測資料的部分 -> 4 條 AT 指令，如下

     1. `AT+CIPSTART="TCP","184.106.153.149",80`
        > * 與 Thingspeak 伺服器的 80 埠建立 TCP 連線
        > * 正確傳送後，ESP的回應：
        > OK
        > Linked

     2. `AT+CIPSEND=xx`
        > * 告知要傳送請求的資料長度：xx(位元，含空白)
        > * 總字元長度必須恰恰好, 多跟少都不行
        > * 正確傳送後，ESP的回應：
        > $>$
     3. `GET /update?api_key=V8CUO3DJT76JJ3XW&field1=33`
        > * 對 Thingspeak 伺服器發出update 請求
        > * 格式：update?api_key=寫入金鑰&欄位1=數值1&欄位2=數值2&欄位3=數值3....
        > * 我的寫入金鑰：V8CUO3DJT76JJ3XW
        > ![](https://hackmd.io/_uploads/ryqXG_dHh.png)

     4. `AT+CIPCLOSE`
        > * 關閉目前的連線
        > * 正確傳送後，ESP的回應：
        > A

*    Reset 功能(解決所有問題)
        *    RST Pin 平常維持 High，**從 Low -> High 時 Reset**

---

###    感測部分 ( DHT22 )
[DHT22 Datasheet](https://cdn-shop.adafruit.com/datasheets/Digital+humidity+and+temperature+sensor+AM2302.pdf)
*    在資料手冊中分為 3 個步驟：
        1. 當 MCU 和 DHT22 開始通信時， MCU 會將 data-bus 拉低，==**這個過程至少要超過 1 ms**==，以確保 DHT22 能夠檢測到 MCU 的信號，接著 MCU 會拉高 20 - 40 us，等待 DHT22 的響應。
             ![](https://hackmd.io/_uploads/S1YGFgqHh.png)

        2. 當 DHT22 檢測到啟動信號時，DHT22 將總線拉低 80 us 作為響應信號，然後 DHT22 拉高 80 us 準備發送數據。
            ![](https://hackmd.io/_uploads/SJ97lFOSn.png)
        3. 存取溫溼度資料，起始皆為50us的Low，連續存取 40 個 Bits。 
            * Data "0"：==**26 ~ 28 us**== 的 HIGH
              ![](https://hackmd.io/_uploads/HynSvg9Hn.png)
            * Data "1"：==**70 us**== 的 HIGH
              ![](https://hackmd.io/_uploads/HkoFDe9S3.png)
---
