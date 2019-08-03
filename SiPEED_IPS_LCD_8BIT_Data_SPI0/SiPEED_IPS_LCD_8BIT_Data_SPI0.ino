#include "gpio.h"
#include "fpioa.h"
#include "./kendryte-standalone-sdk/lib/drivers/include/spi.h"

// Pin Control
#define SSD1963_RS     38    // Data or Command
#define SSD1963_WR     39    // Write
// #define SSD1963_RD     34    // Read
#define SSD1963_CS     36    // Chip select
#define SSD1963_RESET  37    // Reset

#define SET_LCD_RESET() { digitalWrite(SSD1963_RESET, 1); \
    delay(50); \
    digitalWrite(SSD1963_RESET, 0); \
    delay(100); \
    digitalWrite(SSD1963_RESET, 1); \
    delay(100); }


#define SET_LCD_CS(a) digitalWrite(SSD1963_CS, a)
#define SET_LCD_RS(a) digitalWrite(SSD1963_RS, a)
#define SET_LCD_WR(a) digitalWrite(SSD1963_WR, a)

/*
#define SET_LCD_WR_ACTIVE() *gpiohs->output_val.u32 |= (1<<17);
#define SET_LCD_WR_NON_ACTIVE() *gpiohs->output_val.u32 &= ~(1<<17);
*/

#define SET_LCD_RS_DATA() *gpiohs->output_val.u32 |= (1<<(SSD1963_RS-16)); // IO_16 = GPIOHS0
#define SET_LCD_RS_COMMAND() *gpiohs->output_val.u32 &= ~(1<<(SSD1963_RS-16));

// #define SET_LCD_DATA(a) *gpiohs->output_val.u32=(*gpiohs->output_val.u32&0xFFFFFF00)|(uint32_t)(a)

#define SSD1963_DMA_CH DMAC_CHANNEL3
#define SSD1963_SPI_CH SPI_DEVICE_0

/*
#define SET_LCD_WRITE() { SET_LCD_WR_NON_ACTIVE(); \
    SET_LCD_WR_ACTIVE(); }
*/

// Config LCD
#define LCD_WIDTH  800
#define LCD_HEIGHT 480

void write_command(uint8_t c) {
  SET_LCD_RS_COMMAND();

  /*
    SET_LCD_DATA(c);
    SET_LCD_WRITE();
  */
  spi_init(SSD1963_SPI_CH, SPI_WORK_MODE_2, SPI_FF_OCTAL, 8, 0);
  spi_init_non_standard(SPI_DEVICE_0, 8, 0, 0, SPI_AITM_AS_FRAME_FORMAT);

  spi_set_clk_rate(SSD1963_SPI_CH, 1E3);
  
  spi_send_data_normal_dma(SSD1963_DMA_CH, SSD1963_SPI_CH, SPI_CHIP_SELECT_0, &c, 1, SPI_TRANS_CHAR);

  dmac_wait_done(SSD1963_DMA_CH);
}

void Write_Data_Register(uint8_t d) {
  SET_LCD_RS_DATA();

  /*
    SET_LCD_DATA(d);
    SET_LCD_WRITE();
  */
  spi_init(SSD1963_SPI_CH, SPI_WORK_MODE_2, SPI_FF_OCTAL, 8, 0);
  spi_init_non_standard(SPI_DEVICE_0, 8, 0, 0, SPI_AITM_AS_FRAME_FORMAT);
  
  spi_set_clk_rate(SSD1963_SPI_CH, 1E3);
  
  spi_send_data_normal_dma(SSD1963_DMA_CH, SSD1963_SPI_CH, SPI_CHIP_SELECT_0, &d, 1, SPI_TRANS_CHAR);

  dmac_wait_done(SSD1963_DMA_CH);
}

uint8_t dataBuffer[3];

void Write_Data_Color(uint32_t d) {
  SET_LCD_RS_DATA();

  dataBuffer[0] = (d >> 8) & 0xFF;
  dataBuffer[1] = d & 0xFF;

  spi_init(SSD1963_SPI_CH, SPI_WORK_MODE_2, SPI_FF_OCTAL, 8, 0);
  spi_init_non_standard(SPI_DEVICE_0, 8, 0, 0, SPI_AITM_AS_FRAME_FORMAT);

  spi_set_clk_rate(SSD1963_SPI_CH, 20E6);
  
  spi_send_data_normal_dma(SSD1963_DMA_CH, SSD1963_SPI_CH, SPI_CHIP_SELECT_0, dataBuffer, 2, SPI_TRANS_CHAR);

  dmac_wait_done(SSD1963_DMA_CH);
}

/*
void LCD_clear(uint32_t i) {
  LCD_SetPos(0, LCD_WIDTH - 1, 0, LCD_HEIGHT - 1);
  for (int h = 0; h < LCD_HEIGHT; h++) {
    for (int w = 0; w < LCD_WIDTH; w++) {
      Write_Data_Color(i);
    }
  }
}
*/

uint8_t colorBuffer[LCD_WIDTH * LCD_HEIGHT * 2];
void LCD_clear(uint32_t d) {
  uint8_t inx = 0;
  for (uint32_t x=0;x<(LCD_WIDTH * LCD_HEIGHT);x++) {
    colorBuffer[inx++] = (d >> 8) & 0xFF;
    colorBuffer[inx++] = d & 0xFF;
  }

  
  LCD_SetPos(0, LCD_WIDTH - 1, 0, LCD_HEIGHT - 1);

  SET_LCD_RS_DATA();
  
  spi_init(SSD1963_SPI_CH, SPI_WORK_MODE_2, SPI_FF_OCTAL, 8, 0);
  spi_init_non_standard(SPI_DEVICE_0, 8, 0, 0, SPI_AITM_AS_FRAME_FORMAT);

  spi_set_clk_rate(SSD1963_SPI_CH, 1E6);
  
  spi_send_data_normal_dma(SSD1963_DMA_CH, SSD1963_SPI_CH, SPI_CHIP_SELECT_0, colorBuffer, (LCD_WIDTH * LCD_HEIGHT * 2), SPI_TRANS_CHAR);

  dmac_wait_done(SSD1963_DMA_CH);
}

/*
  void LCD_clear(uint16_t i) {
  LCD_SetPos(0, LCD_WIDTH - 1, 0, LCD_HEIGHT - 1);

  SET_LCD_RS_DATA();

  // Set data output
  SET_LCD_DATA(i);

  for(int h=0;h<LCD_HEIGHT;h++) {
    for(int w=0;w<LCD_WIDTH;w++) {
      // Write to LCD
      SET_LCD_WRITE();
    }
  }
  }
*/

void LCD_SetPos(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye) {
  write_command(0x002A);
  Write_Data_Register(xs >> 8);
  Write_Data_Register(xs & 0x00ff);
  Write_Data_Register(xe >> 8);
  Write_Data_Register(xe & 0x00ff);

  write_command(0x002b);
  Write_Data_Register(ys >> 8);
  Write_Data_Register(ys & 0x00ff);
  Write_Data_Register(ye >> 8);
  Write_Data_Register(ye & 0x00ff);
  write_command(0x002c);
}


void SSD1963_Initial() {
  SET_LCD_RESET();
  delay(1);

  fpioa_set_function(SSD1963_CS, (fpioa_function_t)FUNC_SPI0_SS0);
  fpioa_set_function(SSD1963_WR, (fpioa_function_t)FUNC_SPI0_SCLK);
  sysctl_set_spi0_dvp_data(1);

  dmac_init();

  // SPI init
  spi_init(SSD1963_SPI_CH, SPI_WORK_MODE_2, SPI_FF_OCTAL, 8, 0);
  spi_set_clk_rate(SSD1963_SPI_CH, 20E6);

  write_command(0x11); // exit_sleep_mode
  Write_Data_Register(0x00);
  delay(40);
  write_command(0xB0); // MCAP: Manufacturer Command Access Protect
  Write_Data_Register(0x04); // Manufacturer Command inputs are enabled - enabled all
  write_command(0xB3); // Frame Memory Access and Interface Setting (B3h)
  Write_Data_Register(0x02); // The write start position is reset to the start of window address area to overwrite the subsequent data to the previous data.
  Write_Data_Register(0x00); // OK !
  write_command(0xB6); // DSI Control (B6h)
  Write_Data_Register(0x52);
  Write_Data_Register(0x83); // Operating frequency (MHz), Min: 140 Max: 210
  write_command(0xB7); // I don't know !
  Write_Data_Register(0x80);
  Write_Data_Register(0x72);
  Write_Data_Register(0x11);
  Write_Data_Register(0x25);
  write_command(0xB8); // I don't know !
  Write_Data_Register(0x00);
  Write_Data_Register(0x0F);
  Write_Data_Register(0x0F);
  Write_Data_Register(0xFF);
  Write_Data_Register(0xFF);
  Write_Data_Register(0xC8);
  Write_Data_Register(0xC8);
  Write_Data_Register(0x02);
  Write_Data_Register(0x18);
  Write_Data_Register(0x10);
  Write_Data_Register(0x10);
  Write_Data_Register(0x37);
  Write_Data_Register(0x5A);
  Write_Data_Register(0x87);
  Write_Data_Register(0xBE);
  Write_Data_Register(0xFF);
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  write_command(0xB9); // Backlight Control (2) (B9h)
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  write_command(0xBD); // Resizing Control (BDh)
  Write_Data_Register(0x00);
  write_command(0xC0); // Panel Driving Setting 1 (C0h)
  Write_Data_Register(0x02);
  Write_Data_Register(0x76);
  write_command(0xC1); // Panel Driving Setting 2 (C1h)
  Write_Data_Register(0x63);
  Write_Data_Register(0x31);
  Write_Data_Register(0x00);
  Write_Data_Register(0x27);
  Write_Data_Register(0x27);
  Write_Data_Register(0x32);
  Write_Data_Register(0x12);
  Write_Data_Register(0x28);
  Write_Data_Register(0x4E);
  Write_Data_Register(0x10);
  Write_Data_Register(0xA5);
  Write_Data_Register(0x0F);
  Write_Data_Register(0x58);
  Write_Data_Register(0x21);
  Write_Data_Register(0x01);
  write_command(0xC2); // Display V-Timing Setting (C2h)
  Write_Data_Register(0x28);
  Write_Data_Register(0x06);
  Write_Data_Register(0x06);
  Write_Data_Register(0x01);
  Write_Data_Register(0x03);
  Write_Data_Register(0x00);
  write_command(0xC3); // Test Mode 1 (C3h)
  Write_Data_Register(0x40);
  Write_Data_Register(0x00);
  Write_Data_Register(0x03);
  write_command(0xC4); // Panel Driving Setting 3 (C4h)
  Write_Data_Register(0x00);
  Write_Data_Register(0x01);
  write_command(0xC6); // Outline Sharpening Control (C6h)
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  write_command(0xC7); // Panel Driving Setting 4 (C7h)
  Write_Data_Register(0x11);
  Write_Data_Register(0x8D);
  Write_Data_Register(0xA0);
  Write_Data_Register(0xF5);
  Write_Data_Register(0x27);
  write_command(0xC8); // Gamma Setting A Set (C8h)
  Write_Data_Register(0x02);
  Write_Data_Register(0x13);
  Write_Data_Register(0x18);
  Write_Data_Register(0x25);
  Write_Data_Register(0x34);
  Write_Data_Register(0x4E);
  Write_Data_Register(0x36);
  Write_Data_Register(0x23);
  Write_Data_Register(0x17);
  Write_Data_Register(0x0E);
  Write_Data_Register(0x0C);
  Write_Data_Register(0x02);
  Write_Data_Register(0x02);
  Write_Data_Register(0x13);
  Write_Data_Register(0x18);
  Write_Data_Register(0x25);
  Write_Data_Register(0x34);
  Write_Data_Register(0x4E);
  Write_Data_Register(0x36);
  Write_Data_Register(0x23);
  Write_Data_Register(0x17);
  Write_Data_Register(0x0E);
  Write_Data_Register(0x0C);
  Write_Data_Register(0x02);
  write_command(0xC9); // Gamma Setting B Set (C9h)
  Write_Data_Register(0x02);
  Write_Data_Register(0x13);
  Write_Data_Register(0x18);
  Write_Data_Register(0x25);
  Write_Data_Register(0x34);
  Write_Data_Register(0x4E);
  Write_Data_Register(0x36);
  Write_Data_Register(0x23);
  Write_Data_Register(0x17);
  Write_Data_Register(0x0E);
  Write_Data_Register(0x0C);
  Write_Data_Register(0x02);
  Write_Data_Register(0x02);
  Write_Data_Register(0x13);
  Write_Data_Register(0x18);
  Write_Data_Register(0x25);
  Write_Data_Register(0x34);
  Write_Data_Register(0x4E);
  Write_Data_Register(0x36);
  Write_Data_Register(0x23);
  Write_Data_Register(0x17);
  Write_Data_Register(0x0E);
  Write_Data_Register(0x0C);
  Write_Data_Register(0x02);
  write_command(0xCA); // Gamma Setting C Set (CAh)
  Write_Data_Register(0x02);
  Write_Data_Register(0x13);
  Write_Data_Register(0x18);
  Write_Data_Register(0x25);
  Write_Data_Register(0x34);
  Write_Data_Register(0x4E);
  Write_Data_Register(0x36);
  Write_Data_Register(0x23);
  Write_Data_Register(0x17);
  Write_Data_Register(0x0E);
  Write_Data_Register(0x0C);
  Write_Data_Register(0x02);
  Write_Data_Register(0x02);
  Write_Data_Register(0x13);
  Write_Data_Register(0x18);
  Write_Data_Register(0x25);
  Write_Data_Register(0x34);
  Write_Data_Register(0x4E);
  Write_Data_Register(0x36);
  Write_Data_Register(0x23);
  Write_Data_Register(0x17);
  Write_Data_Register(0x0E);
  Write_Data_Register(0x0C);
  Write_Data_Register(0x02);
  write_command(0xD0); // Power Setting (Charge Pump Setting) (D0h)
  Write_Data_Register(0xA9);
  Write_Data_Register(0x03);
  Write_Data_Register(0xCC);
  Write_Data_Register(0xA5);
  Write_Data_Register(0x00);
  Write_Data_Register(0x53);
  Write_Data_Register(0x20);
  Write_Data_Register(0x10);
  Write_Data_Register(0x01);
  Write_Data_Register(0x00);
  Write_Data_Register(0x01);
  Write_Data_Register(0x01);
  Write_Data_Register(0x00);
  Write_Data_Register(0x03);
  Write_Data_Register(0x01);
  Write_Data_Register(0x00);
  write_command(0xD1); // Power Setting (Switching Regulator Setting) (D1h)
  Write_Data_Register(0x18);
  Write_Data_Register(0x0C);
  Write_Data_Register(0x23);
  Write_Data_Register(0x03);
  Write_Data_Register(0x75);
  Write_Data_Register(0x02);
  Write_Data_Register(0x50);
  write_command(0xD3); // Power Setting for Internal Mode (D3h)
  Write_Data_Register(0x33);
  write_command(0xD5); // VPLVL/VNLVL Setting (D5h)
  Write_Data_Register(0x2a);
  Write_Data_Register(0x2a);
  write_command(0xD6); // Test Mode 2 (D6h)
  Write_Data_Register(0x28); //a8
  write_command(0xD7); // Test Mode 3 (D7h)
  Write_Data_Register(0x01);
  Write_Data_Register(0x00);
  Write_Data_Register(0xAA);
  Write_Data_Register(0xC0);
  Write_Data_Register(0x2A);
  Write_Data_Register(0x2C);
  Write_Data_Register(0x22);
  Write_Data_Register(0x12);
  Write_Data_Register(0x71);
  Write_Data_Register(0x0A);
  Write_Data_Register(0x12);
  Write_Data_Register(0x00);
  Write_Data_Register(0xA0);
  Write_Data_Register(0x00);
  Write_Data_Register(0x03);
  write_command(0xD8); // Test Mode 4 (D8h)
  Write_Data_Register(0x44);
  Write_Data_Register(0x44);
  Write_Data_Register(0x22);
  Write_Data_Register(0x44);
  Write_Data_Register(0x21);
  Write_Data_Register(0x46);
  Write_Data_Register(0x42);
  Write_Data_Register(0x40);
  write_command(0xD9); // Test Mode 5 (D9h)
  Write_Data_Register(0xCF);
  Write_Data_Register(0x2D);
  Write_Data_Register(0x51);
  write_command(0xDA); // Test Mode 6 (DAh)
  Write_Data_Register(0x01);
  write_command(0xDE); // VCOMDC Setting (DEh)
  Write_Data_Register(0x01);
  Write_Data_Register(0x51);//58
  write_command(0xE1); // set_DDB_write_control (E1h)
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  write_command(0xE6); // VCOMDC Setting 2 (E6h)
  Write_Data_Register(0x55); //58
  write_command(0xF3); // Test Mode 12 (F3h)
  Write_Data_Register(0x06);
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  Write_Data_Register(0x24);
  Write_Data_Register(0x00);
  write_command(0xF8); // Test Mode 13 (F8h)
  Write_Data_Register(0x00);
  write_command(0xFA); // VDC_SEL Setting (FAh)
  Write_Data_Register(0x01);
  write_command(0xFB); // Test Mode 15 (FBh)
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  write_command(0xFC); // Test Mode 16 (FCh)
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  write_command(0xFD); // Test Mode 17 (FDh)
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  Write_Data_Register(0x70);
  Write_Data_Register(0x00);
  Write_Data_Register(0x72);
  Write_Data_Register(0x31);
  Write_Data_Register(0x37);
  Write_Data_Register(0x70);
  Write_Data_Register(0x32);
  Write_Data_Register(0x31);
  Write_Data_Register(0x07);
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  write_command(0xFE); // Test Mode 18 (FEh)
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  Write_Data_Register(0x20);
  write_command(0xB0); // MCAP: Manufacturer Command Access Protect
  Write_Data_Register(0x04); // Manufacturer Command inputs are enabled - enabled all
  delay(40);
  write_command(0x35); // set_tear_on: 35h
  Write_Data_Register(0x00); //
  write_command(0x44); // set_tear_scanline:44h
  Write_Data_Register(0x00);
  write_command(0x36); // set_address_mode: 36h -> rotation
  Write_Data_Register(0x00);
  write_command(0x3A); // set_pixel_format: 3Ah
  Write_Data_Register(0x55); // 16 bits/pixel (65,536 colors)
  write_command(0x2A); // set_column_address: 2Ah
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  Write_Data_Register(0x01);
  Write_Data_Register(0xDF);
  write_command(0x2B); // set_page_address: 2Bh
  Write_Data_Register(0x00);
  Write_Data_Register(0x00);
  Write_Data_Register(0x03);
  Write_Data_Register(0x1F);
  write_command(0x29); // set_display_on: 29h
  Write_Data_Register(0x00);
  delay(180);
  write_command(0x2C); // write_memory_start: 2Ch
  Write_Data_Register(0x00);
  delay(10);
  write_command(0x36); // set_address_mode: 36h
  Write_Data_Register(0x68);
}


void setup() {

  // LCD
  pinMode(SSD1963_RS, OUTPUT); // RS
  //pinMode(SSD1963_WR, OUTPUT); // WR
  // pinMode(SSD1963_RD, OUTPUT); // RD
  //pinMode(SSD1963_CS, OUTPUT); // CS
  pinMode(SSD1963_RESET, OUTPUT); // RESET

  //digitalWrite(SSD1963_CS, 0);
  digitalWrite(SSD1963_RESET, 0);
  digitalWrite(SSD1963_RS, 0);
  //digitalWrite(SSD1963_WR, 1);
  // digitalWrite(SSD1963_RD, 1);

  SSD1963_Initial();
}

void loop() {
  
    LCD_clear(0xF800);
    LCD_clear(0x07E0);
    LCD_clear(0x001F);
  
/*
  LCD_clear(0xFFFF);
  LCD_clear(0x0000);
  */
}
