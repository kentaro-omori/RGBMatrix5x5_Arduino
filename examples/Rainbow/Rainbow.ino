/**
 * @file Rainbow.ino
 * @brief レインボーパターン表示のサンプルスケッチ
 * 
 * このサンプルでは、5×5 RGB LEDマトリクスにレインボーパターンを表示します。
 * 時間経過とともに色が変化し、美しいアニメーションを作成します。
 * 
 * ATtiny1616マイコン用にMegaTinyCoreで動作するように設計されています。
 * 
 * @author Cascade
 */

#include <Wire.h>
#include "RGBMatrix5x5.h"

// RGBMatrix5x5のインスタンスを作成（I2Cアドレス: 0x74）
RGBMatrix5x5 rgbmatrix5x5(0x74);

// HSV色空間からRGB色空間に変換する関数
void hsvToRgb(float h, float s, float v, uint8_t* r, uint8_t* g, uint8_t* b) {
  int i;
  float f, p, q, t;
  
  h = fmod(h, 360.0f);
  s = constrain(s, 0.0f, 1.0f);
  v = constrain(v, 0.0f, 1.0f);
  
  if (s == 0.0f) {
    // 彩度が0の場合はグレースケール
    *r = *g = *b = (uint8_t)(v * 255.0f);
    return;
  }
  
  h /= 60.0f;
  i = (int)h;
  f = h - i;
  p = v * (1.0f - s);
  q = v * (1.0f - s * f);
  t = v * (1.0f - s * (1.0f - f));
  
  switch (i) {
    case 0:
      *r = (uint8_t)(v * 255.0f);
      *g = (uint8_t)(t * 255.0f);
      *b = (uint8_t)(p * 255.0f);
      break;
    case 1:
      *r = (uint8_t)(q * 255.0f);
      *g = (uint8_t)(v * 255.0f);
      *b = (uint8_t)(p * 255.0f);
      break;
    case 2:
      *r = (uint8_t)(p * 255.0f);
      *g = (uint8_t)(v * 255.0f);
      *b = (uint8_t)(t * 255.0f);
      break;
    case 3:
      *r = (uint8_t)(p * 255.0f);
      *g = (uint8_t)(q * 255.0f);
      *b = (uint8_t)(v * 255.0f);
      break;
    case 4:
      *r = (uint8_t)(t * 255.0f);
      *g = (uint8_t)(p * 255.0f);
      *b = (uint8_t)(v * 255.0f);
      break;
    default:
      *r = (uint8_t)(v * 255.0f);
      *g = (uint8_t)(p * 255.0f);
      *b = (uint8_t)(q * 255.0f);
      break;
  }
}

// 時間計測用の変数
unsigned long previousMillis = 0;
float hue = 0.0f;

void setup() {
  // I2Cを初期化
  Wire.begin();
  
  // RGBMatrix5x5を初期化
  rgbmatrix5x5.begin();
  
  // 明るさを設定（0.0〜1.0）
  rgbmatrix5x5.setBrightness(0.8);
}

void loop() {
  // 現在の時間を取得
  unsigned long currentMillis = millis();
  
  // 10ミリ秒ごとに更新
  if (currentMillis - previousMillis >= 10) {
    previousMillis = currentMillis;
    
    // 色相を更新
    hue = fmod(hue + 1.0f, 360.0f);
    
    // 各LEDの色を設定
    float spacing = 360.0f / 5.0f;
    
    for (uint8_t x = 0; x < rgbmatrix5x5.width(); x++) {
      for (uint8_t y = 0; y < rgbmatrix5x5.height(); y++) {
        // 位置に基づいて色相をオフセット
        float offset = (x * y) / 25.0f * spacing;
        float h = fmod(hue + offset, 360.0f);
        
        // HSVからRGBに変換
        uint8_t r, g, b;
        hsvToRgb(h, 1.0f, 1.0f, &r, &g, &b);
        
        // LEDに色を設定
        rgbmatrix5x5.setPixel(x, y, r, g, b);
      }
    }
    
    // LEDマトリクスに表示
    rgbmatrix5x5.show();
  }
}
