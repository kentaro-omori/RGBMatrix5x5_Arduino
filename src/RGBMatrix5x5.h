/**
 * @file RGBMatrix5x5.h
 * @brief 5×5 RGB LEDマトリクスを制御するためのArduinoライブラリ
 * @author Cascade
 */

#ifndef RGBMATRIX5X5_H
#define RGBMATRIX5X5_H

#include "IS31FL3731.h"

/**
 * @brief 5×5 RGB LEDマトリクスを制御するクラス
 */
class RGBMatrix5x5 : public IS31FL3731 {
public:
    /**
     * @brief コンストラクタ
     * @param i2c_addr I2Cアドレス（デフォルト: 0x74）
     */
    RGBMatrix5x5(uint8_t i2c_addr = 0x74);
    
    /**
     * @brief 初期化処理
     * @return 初期化に成功したらtrue
     */
    bool begin() override;
    
    /**
     * @brief 個別のLEDの色と明るさを設定する
     * @param x X座標（0-4）
     * @param y Y座標（0-4）
     * @param r 赤の強さ（0-255）
     * @param g 緑の強さ（0-255）
     * @param b 青の強さ（0-255）
     * @param brightness 明るさ（0.0-1.0）
     */
    void setPixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b, float brightness = 1.0);
    
    /**
     * @brief 全てのLEDの色と明るさを設定する
     * @param r 赤の強さ（0-255）
     * @param g 緑の強さ（0-255）
     * @param b 青の強さ（0-255）
     * @param brightness 明るさ（0.0-1.0）
     */
    void setAll(uint8_t r, uint8_t g, uint8_t b, float brightness = 1.0);
    
    /**
     * @brief バッファをクリアする（全てのLEDをオフにする）
     */
    void clear();
    
    /**
     * @brief バッファの内容をLEDマトリクスに反映する
     */
    void show();
    
    /**
     * @brief 全体の明るさを設定する
     * @param brightness 明るさ（0.0-1.0）
     */
    void setBrightness(float brightness);
    
    /**
     * @brief ガンマ補正テーブルを設定する
     * @param gamma_table ガンマ補正テーブル（256要素の配列）
     */
    void setGamma(const uint8_t* gamma_table);
    
    /**
     * @brief マトリクスの幅を取得する
     * @return マトリクスの幅（5）
     */
    uint8_t width() const { return _width; }
    
    /**
     * @brief マトリクスの高さを取得する
     * @return マトリクスの高さ（5）
     */
    uint8_t height() const { return _height; }
    
private:
    static const uint8_t _width = 5;    ///< マトリクスの幅
    static const uint8_t _height = 5;   ///< マトリクスの高さ
    
    /**
     * @brief LEDのアドレスを取得する
     * @param x LEDのインデックス
     * @param rgb RGB成分のインデックス（0:R, 1:G, 2:B）
     * @return レジスタアドレス
     */
    uint8_t _pixelAddr(uint8_t x, int8_t rgb = -1);
    
    /**
     * @brief バッファ内のLEDの色情報
     */
    struct PixelData {
        uint8_t r;           ///< 赤の強さ（0-255）
        uint8_t g;           ///< 緑の強さ（0-255）
        uint8_t b;           ///< 青の強さ（0-255）
        float brightness;    ///< 明るさ（0.0-1.0）
    };
    
    PixelData _buffer[25];   ///< LEDの色情報を保持するバッファ
};

#endif // RGBMATRIX5X5_H
