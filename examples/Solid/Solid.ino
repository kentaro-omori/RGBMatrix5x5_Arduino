/**
 * @file Solid.ino
 * @brief 単色表示のサンプルスケッチ
 * 
 * このサンプルでは、5×5 RGB LEDマトリクスに単色を表示し、
 * 赤、緑、青の順に色を切り替えます。
 * 
 * ATtiny1616マイコン用にMegaTinyCoreで動作するように設計されています。
 * 
 * @author Cascade
 */

#include <Wire.h>
#include "RGBMatrix5x5.h"

// RGBMatrix5x5のインスタンスを作成（I2Cアドレス: 0x74）
RGBMatrix5x5 rgbmatrix5x5(0x74);

// 表示する色の配列
const uint8_t colors[][3] = {
  {255, 0, 0},    // 赤
  {0, 255, 0},    // 緑
  {0, 0, 255}     // 青
};

uint8_t colorIndex = 0;  // 現在の色のインデックス

void setup() {
  // I2Cを初期化
  Wire.begin();
  
  // RGBMatrix5x5を初期化
  rgbmatrix5x5.begin();
  
  // 明るさを設定（0.0〜1.0）
  rgbmatrix5x5.setBrightness(0.8);
}

void loop() {
  // 次の色を取得
  uint8_t r = colors[colorIndex][0];
  uint8_t g = colors[colorIndex][1];
  uint8_t b = colors[colorIndex][2];
  
  // 全てのLEDに色を設定
  rgbmatrix5x5.setAll(r, g, b);
  
  // LEDマトリクスに表示
  rgbmatrix5x5.show();
  
  // 次の色のインデックスに更新
  colorIndex = (colorIndex + 1) % 3;
  
  // 0.5秒待機
  delay(500);
}
