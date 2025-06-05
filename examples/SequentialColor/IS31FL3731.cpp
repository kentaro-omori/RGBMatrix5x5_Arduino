/**
 * @file IS31FL3731.cpp
 * @brief IS31FL3731チップを制御するためのArduinoライブラリの実装
 * @author Cascade
 */

#include "IS31FL3731.h"

/**
 * @brief コンストラクタ
 * @param i2c_addr I2Cアドレス（デフォルト: 0x74）
 */
IS31FL3731::IS31FL3731(uint8_t i2c_addr) {
    _i2c_addr = i2c_addr;
    _current_frame = 0;
    _brightness = 1.0;
    _initialized = false;
    
    // デフォルトのガンマテーブル
    static const uint8_t defaultGamma[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
        2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5,
        6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11,
        11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18,
        19, 19, 20, 21, 21, 22, 22, 23, 23, 24, 25, 25, 26, 27, 27, 28,
        29, 29, 30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 37, 38, 39, 40,
        40, 41, 42, 43, 44, 45, 46, 46, 47, 48, 49, 50, 51, 52, 53, 54,
        55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
        71, 72, 73, 74, 76, 77, 78, 79, 80, 81, 83, 84, 85, 86, 88, 89,
        90, 91, 93, 94, 95, 96, 98, 99, 100, 102, 103, 104, 106, 107, 109, 110,
        111, 113, 114, 116, 117, 119, 120, 121, 123, 124, 126, 128, 129, 131, 132, 134,
        135, 137, 138, 140, 142, 143, 145, 146, 148, 150, 151, 153, 155, 157, 158, 160,
        162, 163, 165, 167, 169, 170, 172, 174, 176, 178, 179, 181, 183, 185, 187, 189,
        191, 193, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214, 216, 218, 220,
        222, 224, 227, 229, 231, 233, 235, 237, 239, 241, 244, 246, 248, 250, 252, 255
    };
    
    _gamma_table = defaultGamma;
}

/**
 * @brief 初期化処理
 * @return 初期化に成功したらtrue
 */
bool IS31FL3731::begin() {
    if (_initialized) {
        return true;
    }
    
    Wire.begin();
    
    // チップをリセット
    reset();
    
    // コンフィグバンクに切り替え
    setBank(CONFIG_BANK);
    
    // ピクチャーモードに設定
    Wire.beginTransmission(_i2c_addr);
    Wire.write(MODE_REGISTER);
    Wire.write(PICTURE_MODE);
    Wire.endTransmission();
    
    // オーディオ同期を無効化
    Wire.beginTransmission(_i2c_addr);
    Wire.write(AUDIOSYNC_REGISTER);
    Wire.write(0);
    Wire.endTransmission();
    
    _initialized = true;
    return true;
}

/**
 * @brief チップをリセットする
 */
void IS31FL3731::reset() {
    sleep(true);
    delayMicroseconds(10);
    sleep(false);
}

/**
 * @brief スリープモードを設定する
 * @param value trueでスリープ、falseで通常動作
 */
void IS31FL3731::sleep(bool value) {
    writeRegister(CONFIG_BANK, SHUTDOWN_REGISTER, !value);
}

/**
 * @brief 表示フレームを設定する
 * @param frame フレーム番号（0-8）
 * @param show trueなら即座に表示を切り替える
 * @return 現在のフレーム番号
 */
uint8_t IS31FL3731::setFrame(uint8_t frame, bool show) {
    if (frame != 0xFF) {
        if (frame > 8) {
            frame = 0;  // フレーム番号が範囲外の場合は0に設定
        }
        _current_frame = frame;
        
        if (show) {
            writeRegister(CONFIG_BANK, FRAME_REGISTER, frame);
        }
    }
    
    return _current_frame;
}

/**
 * @brief メモリバンクを設定する
 * @param bank バンク番号
 * @return 現在のバンク番号
 */
uint8_t IS31FL3731::setBank(uint8_t bank) {
    if (bank != 0xFF) {
        Wire.beginTransmission(_i2c_addr);
        Wire.write(BANK_ADDRESS);
        Wire.write(bank);
        Wire.endTransmission();
    } else {
        Wire.beginTransmission(_i2c_addr);
        Wire.write(BANK_ADDRESS);
        Wire.endTransmission();
        
        Wire.requestFrom(_i2c_addr, (uint8_t)1);
        bank = Wire.read();
    }
    
    return bank;
}

/**
 * @brief レジスタに値を書き込む
 * @param bank バンク番号
 * @param reg レジスタアドレス
 * @param value 書き込む値
 * @return 書き込んだ値
 */
uint8_t IS31FL3731::writeRegister(uint8_t bank, uint8_t reg, uint8_t value) {
    setBank(bank);
    
    Wire.beginTransmission(_i2c_addr);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
    
    return value;
}

/**
 * @brief レジスタから値を読み込む
 * @param bank バンク番号
 * @param reg レジスタアドレス
 * @return 読み込んだ値
 */
uint8_t IS31FL3731::readRegister(uint8_t bank, uint8_t reg) {
    setBank(bank);
    
    Wire.beginTransmission(_i2c_addr);
    Wire.write(reg);
    Wire.endTransmission();
    
    Wire.requestFrom(_i2c_addr, (uint8_t)1);
    return Wire.read();
}
