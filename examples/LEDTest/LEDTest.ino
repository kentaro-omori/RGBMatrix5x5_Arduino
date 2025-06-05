/**
 * @file LEDTest.ino
 * @brief 特定のLEDの色チャンネルをテストするスケッチ
 * 
 * このスケッチでは、問題のある10番目と23番目のLEDの各色チャンネル（R、G、B）を
 * 個別に制御し、どのチャンネルがどのように影響しているかを特定します。
 * ボタン操作で表示するLEDと色を切り替えることができます。
 * 
 * @author Cascade
 */

#include <Wire.h>
#include "RGBMatrix5x5.h"

// RGBMatrix5x5のインスタンスを作成（I2Cアドレス: 0x74）
RGBMatrix5x5 rgbmatrix5x5(0x74);

// テスト対象のLED
#define LED_10_X 1
#define LED_10_Y 4
#define LED_23_X 4
#define LED_23_Y 2

// テストモード
enum TestMode {
  LED_10_RED,
  LED_10_GREEN,
  LED_10_BLUE,
  LED_23_RED,
  LED_23_GREEN,
  LED_23_BLUE,
  ALL_LEDS_RED,
  ALL_LEDS_GREEN,
  ALL_LEDS_BLUE,
  TEST_MODE_COUNT
};

// 現在のテストモード
TestMode currentMode = LED_10_RED;

// 時間計測用の変数
unsigned long previousMillis = 0;
const unsigned long interval = 2000; // モード切替の間隔（ミリ秒）

// テストモードの名前
const char* modeNames[] = {
  "LED 10 - RED",
  "LED 10 - GREEN",
  "LED 10 - BLUE",
  "LED 23 - RED",
  "LED 23 - GREEN",
  "LED 23 - BLUE",
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
  
  Serial.println(F("RGBMatrix5x5 LED Test"));
  Serial.println(F("---------------------"));
  Serial.println(F("現在のモード: LED 10 - RED"));
}

void loop() {
  // 現在の時間を取得
  unsigned long currentMillis = millis();
  
  // 一定間隔ごとにモードを切り替える
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // 次のモードに切り替え
    currentMode = static_cast<TestMode>((currentMode + 1) % TEST_MODE_COUNT);
    
    // すべてのLEDを消灯
    rgbmatrix5x5.clear();
    
    // 現在のモードに応じてLEDを点灯
    updateLEDs();
    
    // シリアルモニタに現在のモードを表示
    Serial.print(F("現在のモード: "));
    Serial.println(modeNames[currentMode]);
  }
}

// 現在のモードに応じてLEDを点灯する関数
void updateLEDs() {
  // すべてのLEDを消灯
  rgbmatrix5x5.clear();
  
  // 現在のモードに応じてLEDを点灯
  switch (currentMode) {
    case LED_10_RED:
      rgbmatrix5x5.setPixel(LED_10_X, LED_10_Y, 255, 0, 0);
      break;
    case LED_10_GREEN:
      rgbmatrix5x5.setPixel(LED_10_X, LED_10_Y, 0, 255, 0);
      break;
    case LED_10_BLUE:
      rgbmatrix5x5.setPixel(LED_10_X, LED_10_Y, 0, 0, 255);
      break;
    case LED_23_RED:
      rgbmatrix5x5.setPixel(LED_23_X, LED_23_Y, 255, 0, 0);
      break;
    case LED_23_GREEN:
      rgbmatrix5x5.setPixel(LED_23_X, LED_23_Y, 0, 255, 0);
      break;
    case LED_23_BLUE:
      rgbmatrix5x5.setPixel(LED_23_X, LED_23_Y, 0, 0, 255);
      break;
    case ALL_LEDS_RED:
      rgbmatrix5x5.setAll(255, 0, 0);
      break;
    case ALL_LEDS_GREEN:
      rgbmatrix5x5.setAll(0, 255, 0);
      break;
    case ALL_LEDS_BLUE:
      rgbmatrix5x5.setAll(0, 0, 255);
      break;
  }
  
  // LEDマトリクスに表示
  rgbmatrix5x5.show();
}
