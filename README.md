# RGBMatrix5x5 Arduino ライブラリ

IS31FL3731チップを使用した5×5 RGB LEDマトリクスをArduinoで制御するためのライブラリです。特にMegaTinyCoreを使用してATtiny1616マイコンで動作するように設計されています。

## 特徴

- IS31FL3731チップとI2C通信を行い、5×5 RGB LEDマトリクスを制御
- 個別のLEDの色（RGB値）と明るさを設定可能
- 全てのLEDを一括で設定可能
- ガンマ補正テーブルを適用可能
- フレームバッファを使用して表示をスムーズに更新

## インストール方法

1. このリポジトリをクローンまたはダウンロードします
2. Arduinoライブラリディレクトリに配置します
   - Windows: `Documents\Arduino\libraries\RGBMatrix5x5`
   - Mac/Linux: `~/Documents/Arduino/libraries/RGBMatrix5x5`
3. Arduino IDEを再起動します

## 使用方法

### 基本的な使い方

```cpp
#include <Wire.h>
#include "RGBMatrix5x5.h"

// RGBMatrix5x5のインスタンスを作成（I2Cアドレス: 0x74）
RGBMatrix5x5 rgbmatrix5x5(0x74);

void setup() {
  // I2Cを初期化
  Wire.begin();
  
  // RGBMatrix5x5を初期化
  rgbmatrix5x5.begin();
  
  // 明るさを設定（0.0〜1.0）
  rgbmatrix5x5.setBrightness(0.8);
}

void loop() {
  // 赤色を表示
  rgbmatrix5x5.setAll(255, 0, 0);
  rgbmatrix5x5.show();
  delay(500);
  
  // 緑色を表示
  rgbmatrix5x5.setAll(0, 255, 0);
  rgbmatrix5x5.show();
  delay(500);
  
  // 青色を表示
  rgbmatrix5x5.setAll(0, 0, 255);
  rgbmatrix5x5.show();
  delay(500);
}
```

### 個別のLEDを制御する

```cpp
// 座標(x, y)のLEDを赤色に設定
rgbmatrix5x5.setPixel(0, 0, 255, 0, 0);

// 座標(x, y)のLEDを緑色に設定（明るさ50%）
rgbmatrix5x5.setPixel(1, 1, 0, 255, 0, 0.5);

// 変更を反映
rgbmatrix5x5.show();
```

## サンプルスケッチ

- **Solid**: 単色表示のサンプル
- **Rainbow**: レインボーパターン表示のサンプル

## ATtiny1616での使用方法

1. Arduino IDEにMegaTinyCoreをインストールします
   - ボードマネージャーURLに以下を追加: `http://drazzy.com/package_drazzy.com_index.json`
   - ボードマネージャーから「megaTinyCore」をインストール
2. ボードの設定:
   - ボード: "ATtiny1616"
   - クロック: "20 MHz internal"
   - ミリ秒タイマー: "TCB0"
   - I2C: "Enabled"
3. ATtiny1616のピン配置:
   - SDA: PA1
   - SCL: PA2

## ライセンス

MIT

## 謝辞

このライブラリは、Pimoroni社のMicroPython用RGBMatrix5x5ライブラリを参考に作成されています。
