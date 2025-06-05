# RGBMatrix5x5 Arduino ライブラリ設計書

## 要件定義書

### 目的
IS31FL3731チップを使用した5×5 RGB LEDマトリクスをArduinoプラットフォーム上で制御するためのライブラリを開発する。特にMegaTinyCoreを使用してATtiny1616マイコンで動作することを目標とする。

### 機能要件
1. IS31FL3731チップとI2C通信を行い、5×5 RGB LEDマトリクスを制御する
2. 個別のLEDの色（RGB値）と明るさを設定できる
3. 全てのLEDを一括で設定できる
4. ガンマ補正テーブルを適用できる
5. フレームバッファを使用して表示をスムーズに更新できる
6. MegaTinyCoreを使用してATtiny1616マイコンで動作する

### 非機能要件
1. メモリ使用量を最小限に抑える（ATtiny1616のリソース制約を考慮）
2. 処理速度を最適化する
3. Arduino IDEで簡単に使用できるようにする

## 設計書

### 概略設計
MicroPythonのライブラリをArduino C++に変換し、同等の機能を提供する。ただし、ATtiny1616のリソース制約を考慮して最適化を行う。

### 機能設計

#### IS31FL3731との通信
- I2Cプロトコルを使用してIS31FL3731チップと通信
- レジスタ操作によるLED制御
- フレームバッファの切り替えによるスムーズな表示更新

#### LEDマトリクス制御
- 個別のLED制御（座標指定によるRGB値と明るさの設定）
- 全LEDの一括設定
- ガンマ補正による視覚的に自然な明るさ調整

### クラス構成

#### IS31FL3731クラス
基本的なIS31FL3731チップの制御を行うクラス

**メンバ変数**
- `_i2c_addr`: I2Cアドレス
- `_current_frame`: 現在表示中のフレーム番号
- `_brightness`: 全体の明るさ（0.0〜1.0）
- `_gamma_table`: ガンマ補正テーブル（256要素の配列）

**メソッド**
- `begin()`: 初期化処理
- `reset()`: チップのリセット
- `sleep(bool value)`: スリープモードの設定
- `setFrame(uint8_t frame)`: 表示フレームの設定
- `setBank(uint8_t bank)`: メモリバンクの設定
- `writeRegister(uint8_t bank, uint8_t reg, uint8_t value)`: レジスタ書き込み
- `readRegister(uint8_t bank, uint8_t reg)`: レジスタ読み込み

#### RGBMatrix5x5クラス
IS31FL3731クラスを継承し、5×5 RGB LEDマトリクスの制御を行うクラス

**メンバ変数**
- `_width`, `_height`: マトリクスのサイズ（5×5）
- `_buffer`: LEDの色情報を保持するバッファ

**メソッド**
- `begin()`: 初期化処理
- `setPixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b, float brightness)`: 個別のLEDの設定
- `setAll(uint8_t r, uint8_t g, uint8_t b, float brightness)`: 全LEDの一括設定
- `clear()`: 全LEDをオフにする
- `show()`: バッファの内容をLEDマトリクスに反映
- `setBrightness(float brightness)`: 全体の明るさ設定
- `setGamma(const uint8_t* gamma_table)`: ガンマ補正テーブルの設定
