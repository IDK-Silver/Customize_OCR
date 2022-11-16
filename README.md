# 客製化圖像辨識系統

## 編譯說明
<a name="10"></a>
#### 運行環境
- Windows環境，目前及於 `Visual Studio 2019 Community` 進行編譯
- CMake 3.0+
- Python 3

#### 依賴庫
- Qt 6.3.1 `MSVC 2019 64 Bit`
- Xlnt
- OpenCV 
- Tesseract

#### 操作步驟
1. 把程式的原始碼下載下來
<pre><code>git clone https://github.com/IDK-Silver/Customize_OCR.git </code></pre>

2. Python Install Package
<pre><code>pip install conan</code></pre>

3. 切換到專案目錄
<pre><code>cd Customize_OCR</code></pre>

4. 初始化專案
<pre><code>./project_init.py</code></pre>

5. conan 創建 build folder
<pre><code>mkdir build
cd build</code></pre>

6. conan 安裝依賴庫
<pre><code>conan install .. --build=missing</code></pre>

7. CMake Build
<pre><code>cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_BUILD_TYPE=Release ..
cmake --build .  -j --target --config Release
</code></pre>





