/**
 * @file LEDTest2.ino
 * @brief 修正したアドレスマッピングを使用したLEDテストスケッチ
 * 
 * このスケッチでは、修正したアドレスマッピングを使用して、
 * 問題のあった10番目と23番目のLEDが正しく動作するかテストします。
 * 
 * @author Cascade
 */

#include <Wire.h>
#include "RGBMatrix5x5.h"

// RGBMatrix5x5のインスタンスを作成（I2Cアドレス: 0x74）
RGBMatrix5x5 rgbmatrix5x5(0x74);

// テストモード
enum TestMode {
  TEST_LED_9,   // 10番目のLED（インデックス9）
  TEST_LED_22,  // 23番目のLED（インデックス22）
  TEST_ALL_RED,
  TEST_ALL_GREEN,
  TEST_ALL_BLUE,
  TEST_MODE_COUNT
};

// 現在のテストモード
TestMode currentMode = TEST_LED_9;

// 色のインデックス（0:赤、1:緑、2:青）
uint8_t colorIndex = 0;

// 時間計測用の変数
unsigned long previousMillis = 0;
const unsigned long interval = 1000; // モード切替の間隔（ミリ秒）

// テストモードの名前
const char* modeNames[] = {
  "LED 10 (インデックス9)",
  "LED 23 (インデックス22)",
  "ALL - RED",
  "ALL - GREEN",
  "ALL - BLUE"
};

void setup() {
  // シリアル通信を初期化
  Serial.begin(9600);
  while (!Serial) {
    ; // シリアルポートが接続されるのを待つ（USBネイティブポートのみ）
  }
  
  // I2Cを初期化
  Wire.begin();
  
  // RGBMatrix5x5を初期化
  rgbmatrix5x5.begin();
  
  // 明るさを設定（0.0〜1.0）
  rgbmatrix5x5.setBrightness(0.8);
  
  // 最初はすべてのLEDを消灯
  rgbmatrix5x5.clear();
  rgbmatrix5x5.show();
  
  Serial.println(F("RGBMatrix5x5 LED Test 2 - 修正版"));
  Serial.println(F("-----------------------------"));
  Serial.println(F("現在のモード: LED 10 (インデックス9) - RED"));
}

void loop() {
  // 現在の時間を取得
  unsigned long currentMillis = millis();
  
  // 一定間隔ごとに色またはモードを切り替える
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // 色を切り替え
    colorIndex = (colorIndex + 1) % 3;
    
    // 色が一周したら次のモードに切り替え
    if (colorIndex == 0) {
      currentMode = static_cast<TestMode>((currentMode + 1) % TEST_MODE_COUNT);
    }
    
    // すべてのLEDを消灯
    rgbmatrix5x5.clear();
    
    // 現在のモードと色に応じてLEDを点灯
    updateLEDs();
    
    // シリアルモニタに現在のモードと色を表示
    Serial.print(F("現在のモード: "));
    Serial.print(modeNames[currentMode]);
    Serial.print(F(" - "));
    
    switch (colorIndex) {
      case 0:
        Serial.println(F("RED"));
        break;
      case 1:
        Serial.println(F("GREEN"));
        break;
      case 2:
        Serial.println(F("BLUE"));
        break;
    }
  }
}

// 現在のモードと色に応じてLEDを点灯する関数
void updateLEDs() {
  // すべてのLEDを消灯
  rgbmatrix5x5.clear();
  
  // 色の値を設定
  uint8_t r = (colorIndex == 0) ? 255 : 0;
  uint8_t g = (colorIndex == 1) ? 255 : 0;
  uint8_t b = (colorIndex == 2) ? 255 : 0;
  
  // 現在のモードに応じてLEDを点灯
  switch (currentMode) {
    case TEST_LED_9:
      // 10番目のLED（インデックス9）を点灯
      rgbmatrix5x5.setPixel(1, 4, r, g, b);
      break;
    case TEST_LED_22:
      // 23番目のLED（インデックス22）を点灯
      rgbmatrix5x5.setPixel(4, 2, r, g, b);
      break;
    case TEST_ALL_RED:
      // すべてのLEDを赤で点灯
      rgbmatrix5x5.setAll(255, 0, 0);
      break;
    case TEST_ALL_GREEN:
      // すべてのLEDを緑で点灯
      rgbmatrix5x5.setAll(0, 255, 0);
      break;
    case TEST_ALL_BLUE:
      // すべてのLEDを青で点灯
      rgbmatrix5x5.setAll(0, 0, 255);
      break;
  }
  
  // LEDマトリクスに表示
  rgbmatrix5x5.show();
}
