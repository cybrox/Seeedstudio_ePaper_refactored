#if !defined(EPD_H)
#define EPD_H 1

#include <Arduino.h>
#include <SPI.h>
#include "ePaperDfs.h"

#if defined(__MSP430_CPU__) || defined(__SAM3X8E__)
#define PROGMEM
#else
#include <avr/pgmspace.h>
#endif


typedef enum {
    EPD_1_44,        // 128 x 96
    EPD_2_0,         // 200 x 96
    EPD_2_7          // 264 x 176
} EPD_size;

typedef enum {           // Image pixel -> Display pixel
    EPD_compensate,  // B -> W, W -> B (Current Image)
    EPD_white,       // B -> N, W -> W (Current Image)
    EPD_inverse,     // B -> N, W -> B (New Image)
    EPD_normal       // B -> B, W -> W (New Image)
} EPD_stage;

typedef void EPD_reader(void *buffer, uint32_t address, uint16_t length);

class EPD_Class
{
    private:

    int EPD_Pin_EPD_CS;
    int EPD_Pin_PANEL_ON;
    int EPD_Pin_BORDER;
    int EPD_Pin_DISCHARGE;
    int EPD_Pin_PWM;
    int EPD_Pin_RESET;
    int EPD_Pin_BUSY;

    EPD_size size;
    uint16_t stage_time;

    uint16_t factored_stage_time;
    uint16_t lines_per_display;
    uint16_t dots_per_line;
    uint16_t bytes_per_line;
    uint16_t bytes_per_scan;
    PROGMEM const uint8_t *gate_source;
    uint16_t gate_source_length;
    PROGMEM const uint8_t *channel_select;
    uint16_t channel_select_length;

    bool filler;

    public:
    unsigned char lineDta[33];

    public:
    // power up and power down the EPD panel
    void begin(EPD_size sz);
    void start();
    void end();

    // clear display (anything -> white)
    void clear()
    {
        this->frame_fixed_repeat(0xff, EPD_compensate);
        this->frame_fixed_repeat(0xff, EPD_white);
        this->frame_fixed_repeat(0xaa, EPD_inverse);
        this->frame_fixed_repeat(0xaa, EPD_normal);
    }

    // assuming a clear (white) screen output an image (PROGMEM data)
    void image(PROGMEM const uint8_t *image)
    {
        this->frame_fixed_repeat(0xaa, EPD_compensate);
        this->frame_fixed_repeat(0xaa, EPD_white);
        this->frame_data_repeat(image, EPD_inverse);
        this->frame_data_repeat(image, EPD_normal);
    }

    // change from old image to new image (PROGMEM data)
    void image(PROGMEM const uint8_t *old_image, PROGMEM const uint8_t *new_image)
    {
        this->frame_data_repeat(old_image, EPD_compensate);
        this->frame_data_repeat(old_image, EPD_white);
        this->frame_data_repeat(new_image, EPD_inverse);
        this->frame_data_repeat(new_image, EPD_normal);
    }

    // change from old image to new image (SRAM version)
    void image_sram(unsigned char *new_image) 
    {
        this->frame_fixed_repeat(0xaa, EPD_compensate);
        this->frame_fixed_repeat(0xaa, EPD_white);
        this->frame_sram_repeat(new_image, EPD_inverse);
        this->frame_sram_repeat(new_image, EPD_normal);
    }

    // Low level API calls
    void frame_fixed(uint8_t fixed_value, EPD_stage stage);
    void frame_data(PROGMEM const uint8_t *new_image, EPD_stage stage);
    void frame_sram(const uint8_t *new_image, EPD_stage stage);
    void frame_cb(uint32_t address, EPD_reader *reader, EPD_stage stage);
    void frame_fixed_repeat(uint8_t fixed_value, EPD_stage stage);
    void frame_data_repeat(PROGMEM const uint8_t *new_image, EPD_stage stage);
    void frame_sram_repeat(const uint8_t *new_image, EPD_stage stage);
    void frame_cb_repeat(uint32_t address, EPD_reader *reader, EPD_stage stage);
    void line(uint16_t line, const uint8_t *data, uint8_t fixed_value, bool read_progmem, EPD_stage stage);
    void line_sd(uint16_t line, const uint8_t *data, uint8_t fixed_value, bool read_progmem, EPD_stage stage);
};

extern EPD_Class EPD;

#endif


