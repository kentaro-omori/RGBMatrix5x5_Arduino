/**
 * @file IS31FL3731.h
 * @brief IS31FL3731チップを制御するためのArduinoライブラリ
 * @author Cascade
 */

#ifndef IS31FL3731_H
#define IS31FL3731_H

#include <Arduino.h>
#include <Wire.h>

// レジスタアドレス定義
#define MODE_REGISTER        0x00
#define FRAME_REGISTER       0x01
#define AUTOPLAY1_REGISTER   0x02
#define AUTOPLAY2_REGISTER   0x03
#define BLINK_REGISTER       0x05
#define AUDIOSYNC_REGISTER   0x06
#define BREATH1_REGISTER     0x08
#define BREATH2_REGISTER     0x09
#define SHUTDOWN_REGISTER    0x0a
#define GAIN_REGISTER        0x0b
#define ADC_REGISTER         0x0c

#define CONFIG_BANK          0x0b
#define BANK_ADDRESS         0xfd

#define PICTURE_MODE         0x00
#define AUTOPLAY_MODE        0x08
#define AUDIOPLAY_MODE       0x18

#define ENABLE_OFFSET        0x00
#define BLINK_OFFSET         0x12
#define COLOR_OFFSET         0x24

/**
 * @brief IS31FL3731チップを制御するクラス
 */
class IS31FL3731 {
public:
    /**
     * @brief コンストラクタ
     * @param i2c_addr I2Cアドレス（デフォルト: 0x74）
     */
    IS31FL3731(uint8_t i2c_addr = 0x74);

    /**
     * @brief 初期化処理
     * @return 初期化に成功したらtrue
     */
    virtual bool begin();

    /**
     * @brief チップをリセットする
     */
    void reset();

    /**
     * @brief スリープモードを設定する
     * @param value trueでスリープ、falseで通常動作
     */
    void sleep(bool value);

    /**
     * @brief 表示フレームを設定する
     * @param frame フレーム番号（0-8）
     * @param show trueなら即座に表示を切り替える
     * @return 現在のフレーム番号
     */
    uint8_t setFrame(uint8_t frame = 0xFF, bool show = true);

    /**
     * @brief メモリバンクを設定する
     * @param bank バンク番号
     * @return 現在のバンク番号
     */
    uint8_t setBank(uint8_t bank = 0xFF);

    /**
     * @brief レジスタに値を書き込む
     * @param bank バンク番号
     * @param reg レジスタアドレス
     * @param value 書き込む値
     * @return 書き込んだ値
     */
    uint8_t writeRegister(uint8_t bank, uint8_t reg, uint8_t value);

    /**
     * @brief レジスタから値を読み込む
     * @param bank バンク番号
     * @param reg レジスタアドレス
     * @return 読み込んだ値
     */
    uint8_t readRegister(uint8_t bank, uint8_t reg);

protected:
    uint8_t _i2c_addr;       ///< I2Cアドレス
    uint8_t _current_frame;  ///< 現在のフレーム番号
    float _brightness;       ///< 全体の明るさ（0.0〜1.0）
    const uint8_t* _gamma_table; ///< ガンマ補正テーブル
    bool _initialized;       ///< 初期化済みフラグ
};

#endif // IS31FL3731_H
