/**
 * @file RGBMatrix5x5.cpp
 * @brief 5×5 RGB LEDマトリクスを制御するためのArduinoライブラリの実装
 * @author Cascade
 */

 #include "RGBMatrix5x5.h"

 /**
  * @brief コンストラクタ
  * @param i2c_addr I2Cアドレス（デフォルト: 0x74）
  */
 RGBMatrix5x5::RGBMatrix5x5(uint8_t i2c_addr) : IS31FL3731(i2c_addr) {
     // バッファを初期化
     clear();
 }
 
 /**
  * @brief 初期化処理
  * @return 初期化に成功したらtrue
  */
 bool RGBMatrix5x5::begin() {
     if (!IS31FL3731::begin()) {
         return false;
     }
     
     // 5×5 RGB LEDマトリクスのLEDを有効化するパターン
     const uint8_t enablePattern[18] = {
         0b00000000, 0b10000111,
         0b00111110, 0b00111110,
         0b00111111, 0b10111110,
         0b00000111, 0b10000110,
         0b00110000, 0b00000000,
         0b00111111, 0b10001110,
         0b00111111, 0b10001110,
         0b01111111, 0b11111110,
         0b01111111, 0b00000000
     };
     
     // フレーム1に切り替え
     setBank(1);
     
     // LEDを有効化
     Wire.beginTransmission(_i2c_addr);
     Wire.write(ENABLE_OFFSET);
     for (uint8_t i = 0; i < 18; i++) {
         Wire.write(enablePattern[i]);
     }
     Wire.endTransmission();
     
     // フレーム0に切り替え
     setBank(0);
     
     // LEDを有効化
     Wire.beginTransmission(_i2c_addr);
     Wire.write(ENABLE_OFFSET);
     for (uint8_t i = 0; i < 18; i++) {
         Wire.write(enablePattern[i]);
     }
     Wire.endTransmission();
     
     return true;
 }
 
 /**
  * @brief 個別のLEDの色と明るさを設定する
  * @param x X座標（0-4）
  * @param y Y座標（0-4）
  * @param r 赤の強さ（0-255）
  * @param g 緑の強さ（0-255）
  * @param b 青の強さ（0-255）
  * @param brightness 明るさ（0.0-1.0）
  */
 void RGBMatrix5x5::setPixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b, float brightness) {
     if (x >= _width || y >= _height) {
         return;  // 範囲外のアクセスを防止
     }
     
     // 奇数列の場合、Yの向きが逆になる（MicroPythonの実装と同様）
     if (x % 2 == 1) {
         y = 4 - y;
     }
     
     uint8_t index = y + (x * _height);
     _buffer[index].r = r;
     _buffer[index].g = g;
     _buffer[index].b = b;
     _buffer[index].brightness = brightness;
 }
 
 /**
  * @brief 全てのLEDの色と明るさを設定する
  * @param r 赤の強さ（0-255）
  * @param g 緑の強さ（0-255）
  * @param b 青の強さ（0-255）
  * @param brightness 明るさ（0.0-1.0）
  */
 void RGBMatrix5x5::setAll(uint8_t r, uint8_t g, uint8_t b, float brightness) {
     for (uint8_t x = 0; x < _width; x++) {
         for (uint8_t y = 0; y < _height; y++) {
             setPixel(x, y, r, g, b, brightness);
         }
     }
 }
 
 /**
  * @brief バッファをクリアする（全てのLEDをオフにする）
  */
 void RGBMatrix5x5::clear() {
     for (uint8_t i = 0; i < _width * _height; i++) {
         _buffer[i].r = 0;
         _buffer[i].g = 0;
         _buffer[i].b = 0;
         _buffer[i].brightness = 1.0;
     }
 }
 
 /**
  * @brief バッファの内容をLEDマトリクスに反映する
  */
 void RGBMatrix5x5::show() {
     // 初期化されていない場合は初期化する
     if (!_initialized) {
         begin();
     }
     
     // 次のフレームを計算（現在のフレームが0なら1、1なら0）
     uint8_t nextFrame = (_current_frame == 0) ? 1 : 0;
     
     // 出力バッファを準備
     uint8_t output[144] = {0};
     
     // バッファの内容をLEDマトリクスに反映
     for (uint8_t i = 0; i < _width * _height; i++) {
         // ガンマ補正と明るさ調整を適用
         uint8_t r = _gamma_table[int(_buffer[i].r * _brightness * _buffer[i].brightness)];
         uint8_t g = _gamma_table[int(_buffer[i].g * _brightness * _buffer[i].brightness)];
         uint8_t b = _gamma_table[int(_buffer[i].b * _brightness * _buffer[i].brightness)];
         
         // LEDのアドレスを取得
         uint8_t ir = _pixelAddr(i, 0);
         uint8_t ig = _pixelAddr(i, 1);
         uint8_t ib = _pixelAddr(i, 2);
         
         // 出力バッファに値を設定
         output[ir] = r;
         output[ig] = g;
         output[ib] = b;
     }
     
     // 次のフレームに切り替え
     setBank(nextFrame);
     
     // 出力バッファをLEDマトリクスに書き込む（32バイトずつ）
     for (uint8_t offset = 0; offset < 144; offset += 32) {
         Wire.beginTransmission(_i2c_addr);
         Wire.write(COLOR_OFFSET + offset);
         for (uint8_t i = 0; i < 32 && (offset + i) < 144; i++) {
             Wire.write(output[offset + i]);
         }
         Wire.endTransmission();
     }
     
     // 表示フレームを切り替え
     setFrame(nextFrame);
 }
 
 /**
  * @brief 全体の明るさを設定する
  * @param brightness 明るさ（0.0-1.0）
  */
 void RGBMatrix5x5::setBrightness(float brightness) {
     if (brightness < 0.0f) brightness = 0.0f;
     if (brightness > 1.0f) brightness = 1.0f;
     _brightness = brightness;
 }
 
 /**
  * @brief ガンマ補正テーブルを設定する
  * @param gamma_table ガンマ補正テーブル（256要素の配列）
  */
 void RGBMatrix5x5::setGamma(const uint8_t* gamma_table) {
     if (gamma_table != nullptr) {
         _gamma_table = gamma_table;
     }
 }
 
 /**
  * @brief LEDのアドレスを取得する
  * @param x LEDのインデックス
  * @param rgb RGB成分のインデックス（0:R, 1:G, 2:B）
  * @return レジスタアドレス
  */
 uint8_t RGBMatrix5x5::_pixelAddr(uint8_t x, int8_t rgb) {
     // 5×5 RGB LEDマトリクスのLEDアドレスマッピング
     static const uint8_t lookup[25][3] = {
         {118, 69, 85},
         {117, 68, 101},
         {116, 84, 100},
         {115, 83, 99},
         {114, 82, 98},
         {113, 81, 97},
         {112, 80, 96},
         {134, 21, 37},
         {133, 20, 36},
         {132, 19, 35},
         {131, 18, 34},
         {130, 17, 50},
         {129, 33, 49},
         {128, 32, 48},
         {127, 47, 63},
         {121, 41, 57},
         {122, 25, 58},
         {123, 26, 42},
         {124, 27, 43},
         {125, 28, 44},
         {126, 29, 45},
         {15, 95, 111},
         {8, 89, 105},
         {9, 90, 106},
         {10, 91, 107}
     };
     
     if (x >= 25) {
         return 0;  // 範囲外のアクセスを防止
     }
     
     if (rgb < 0 || rgb > 2) {
         return x;  // RGB成分のインデックスが範囲外の場合はLEDのインデックスを返す
     }
     
     return lookup[x][rgb];
 }
 