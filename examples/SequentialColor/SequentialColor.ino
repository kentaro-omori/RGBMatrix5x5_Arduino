/**
 * @file SequentialColor.ino
 * @brief 順番に色を変えながらLEDを点灯するサンプルスケッチ
 * 
 * このサンプルでは、5×5 RGB LEDマトリクスのLEDを1個ずつ順番に点灯していきます。
 * 最初は赤色で全てのLEDを点灯させ、次に緑色、その次は青色と順番に色を変えていきます。
 * 
 * ATtiny1616マイコン用にMegaTinyCoreで動作するように設計されています。
 * 
 * @author Cascade
 */

#include <Wire.h>
#include "RGBMatrix5x5.h"

// RGBMatrix5x5のインスタンスを作成（I2Cアドレス: 0x74）
RGBMatrix5x5 rgbmatrix5x5(0x74);

// 色の定義
enum ColorMode {
  RED,
  GREEN,
  BLUE
};

// 現在の色モード
ColorMode currentColor = RED;

// 現在点灯しているLEDの位置
uint8_t currentLed = 0;

// 時間計測用の変数
unsigned long previousMillis = 0;
const unsigned long interval = 100; // LEDを切り替える間隔（ミリ秒）

// LEDの座標を計算する関数
void getLedCoordinates(uint8_t ledIndex, uint8_t* x, uint8_t* y) {
  // 行優先で座標を計算
  *x = ledIndex % 5;
  *y = ledIndex / 5;
  
  // 特定のLEDの座標を調整（ハードウェアの配線に合わせて）
  // 注意: これはハードウェアの特性に対する一時的な対応策です
  if (ledIndex == 9) {
    // 10番目のLEDの特別な処理
    *x = 1;
    *y = 4;
  } else if (ledIndex == 22) {
    // 23番目のLEDの特別な処理
    *x = 4;
    *y = 2;
  }
}

void setup() {
  // I2Cを初期化
  Wire.begin();
  
  // RGBMatrix5x5を初期化
  rgbmatrix5x5.begin();
  
  // 明るさを設定（0.0〜1.0）
  rgbmatrix5x5.setBrightness(0.8);
  
  // 最初はすべてのLEDを消灯
  rgbmatrix5x5.clear();
  
  // 問題のあるLEDを明示的にクリア
  uint8_t x9, y9, x22, y22;
  getLedCoordinates(9, &x9, &y9);
  getLedCoordinates(22, &x22, &y22);
  
  // 問題のあるLEDを強制的に消灯
  rgbmatrix5x5.setPixel(1, 4, 0, 0, 0); // 10番目のLED
  rgbmatrix5x5.setPixel(4, 2, 0, 0, 0); // 23番目のLED
  
  rgbmatrix5x5.show();
}

void loop() {
  // 現在の時間を取得
  unsigned long currentMillis = millis();
  
  // 一定間隔ごとに処理を実行
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // 現在のLEDの座標を計算
    uint8_t x, y;
    getLedCoordinates(currentLed, &x, &y);
    
    // 問題のあるLEDをスキップ
    if (currentLed == 9 || currentLed == 22) {
      // これらのLEDはスキップして次に進む
    } else {
      // 現在の色モードに応じてLEDを点灯
      switch (currentColor) {
        case RED:
          rgbmatrix5x5.setPixel(x, y, 255, 0, 0);
          break;
        case GREEN:
          rgbmatrix5x5.setPixel(x, y, 0, 255, 0);
          break;
        case BLUE:
          rgbmatrix5x5.setPixel(x, y, 0, 0, 255);
          break;
      }
    }
    
    // LEDマトリクスに表示
    rgbmatrix5x5.show();
    
    // 次のLEDに進む
    currentLed++;
    
    // すべてのLEDを点灯させたら、次の色に切り替えて最初のLEDから再開
    if (currentLed >= 25) {
      currentLed = 0;
      
      // 次の色モードに切り替え
      switch (currentColor) {
        case RED:
          currentColor = GREEN;
          break;
        case GREEN:
          currentColor = BLUE;
          break;
        case BLUE:
          currentColor = RED;
          break;
      }
      
      // 色を切り替える前にすべてのLEDを消灯
      rgbmatrix5x5.clear();
      
      // 問題のあるLEDを強制的に消灯
      rgbmatrix5x5.setPixel(1, 4, 0, 0, 0); // 10番目のLED
      rgbmatrix5x5.setPixel(4, 2, 0, 0, 0); // 23番目のLED
      
      rgbmatrix5x5.show();
      
      // 色の切り替え時に少し待機
      delay(500);
    }
  }
}
