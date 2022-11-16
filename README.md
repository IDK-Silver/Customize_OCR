# 客製化圖像辨識系統

## 編譯說明
<a name="10"></a>
#### 運行環境
- Windows環境，目前及於 `Visual Studio 2019 Community` 以上進行編譯
- CMake 3.0+
#### 依賴庫
- Qt 5.15.2 `MSVC 2019 64 Bit`
- PaddleOCR
- OpenCV 
- QXlsx

#### 操作步驟
1. 把程式的原始碼下載下來
<pre><code>git clone https://github.com/IDK-Silver/Customize_OCR.git </code></pre>


2. 接下來下載 [Paddle的預測庫](https://bit.ly/3q4Aj4l) ，到Windows預測庫下的C++預測庫

選擇cpu_avx_mkl的預測庫下載並解壓縮到專案目錄下
![Alt text](/doc/image/PaddleOCR預測庫-安裝.png)


3. 接下來開啟Visual Studio開啟專案資料夾，設置CMake
點擊管理組態進入CMake設定，設定組態類型為Release否則會編譯不過，如果有把Qt設定到環境變數中Qt的路徑會自動抓取，需要手動設定的是OpenCV的路徑，把OpenCV_DIR設為 opencv_dir/build、OPEN_CV_DIR設為opencv_dir就配置完成了，可以順利編譯出執行檔。
 
### System Path
#### Windows 
系統 Path 要新增 "OpenCV_DIR"，路徑為 OpenCV的安裝位置
![Alt text](/doc/image/how-to-build-set-system-path-opencv.png)


