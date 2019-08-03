// Pin Control
#define SSD1963_RS     26    // Data or Command
#define SSD1963_WR     27    // Write
// #define SSD1963_RD     34    // Read
// #define SSD1963_CS     35    // Chip select
// #define SSD1963_RESET  31    // Reset

#ifdef SSD1963_RESET
#define SET_LCD_RESET() { digitalWrite(SSD1963_RESET, 1); \
    delay(50); \
    digitalWrite(SSD1963_RESET, 0); \
    delay(100); \
    digitalWrite(SSD1963_RESET, 1); \
    delay(50); }
#endif

// #define SET_LCD_CS(a) digitalWrite(SSD1963_CS, a)
#define SET_LCD_RS(a) digitalWrite(SSD1963_RS, a)
#define SET_LCD_WR(a) digitalWrite(SSD1963_WR, a)

#define SET_LCD_WR_ACTIVE() *gpiohs->output_val.u32 |= (1<<(SSD1963_WR-16));
#define SET_LCD_WR_NON_ACTIVE() *gpiohs->output_val.u32 &= ~(1<<(SSD1963_WR-16));

#define SET_LCD_RS_DATA() *gpiohs->output_val.u32 |= (1<<(SSD1963_RS-16));
#define SET_LCD_RS_COMMAND() *gpiohs->output_val.u32 &= ~(1<<(SSD1963_RS-16));

#define SET_LCD_DATA(a) *gpiohs->output_val.u32=(*gpiohs->output_val.u32&0xFFFFFC03)|((uint32_t)(a)<<2)

#define SET_LCD_WRITE() { SET_LCD_WR_NON_ACTIVE(); \
                          SET_LCD_WR_ACTIVE(); }

// Config LCD
#define LCD_WIDTH  800
#define LCD_HEIGHT 480

void write_command(uint8_t c) {
  // SET_LCD_CS(0);

  // Set to Command
  // SET_LCD_RS(0);
  SET_LCD_RS_COMMAND();

  // Set data output
  SET_LCD_DATA(c);

  // Write to LCD
  /*
    SET_LCD_WR(0);
    SET_LCD_WR(1);
  */
  SET_LCD_WRITE();

  // SET_LCD_CS(1);
}

void Write_Data_int(uint16_t d) {
  // SET_LCD_CS(0);

  // Set to Data
  SET_LCD_RS_DATA();

  // Set data output
  SET_LCD_DATA(d);

  // Write to LCD
  /*
    SET_LCD_WR(0);
    SET_LCD_WR(1);
  */
  SET_LCD_WRITE();

  // SET_LCD_CS(1);
}

void Write_Data_color(uint16_t d) {
  // SET_LCD_CS(0);

  // Set to Data
  // SET_LCD_RS(1);
  SET_LCD_RS_DATA();

  // Set data output
  SET_LCD_DATA(d >> 8);
  SET_LCD_WRITE();

  // Set data output
  SET_LCD_DATA(d & 0xFF);
  SET_LCD_WRITE();

  // SET_LCD_CS(1);
}

void LCD_clear(unsigned int i) {
  LCD_SetPos(0, LCD_WIDTH - 1, 0, LCD_HEIGHT - 1);

  SET_LCD_RS_DATA();
  for (int h = 0; h < LCD_HEIGHT; h++) {
    for (int w = 0; w < LCD_WIDTH; w++) {
      // Write_Data_color(i);
      
      // Set data output
      SET_LCD_DATA(i >> 8);
      SET_LCD_WRITE();

      // Set data output
      SET_LCD_DATA(i & 0xFF);
      SET_LCD_WRITE();
    }
  }
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
  Write_Data_int(xs >> 8);
  Write_Data_int(xs & 0x00ff);
  Write_Data_int(xe >> 8);
  Write_Data_int(xe & 0x00ff);

  write_command(0x002b);
  Write_Data_int(ys >> 8);
  Write_Data_int(ys & 0x00ff);
  Write_Data_int(ye >> 8);
  Write_Data_int(ye & 0x00ff);
  write_command(0x002c);
}


void SSD1963_Initial() {
#ifdef SSD1963_RESET
  SET_LCD_RESET();
  delay(1);
#endif

  write_command(0x11); // exit_sleep_mode
  Write_Data_int(0x00);
  delay(40);
  write_command(0xB0); // MCAP: Manufacturer Command Access Protect
  Write_Data_int(0x04); // Manufacturer Command inputs are enabled - enabled all
  write_command(0xB3); // Frame Memory Access and Interface Setting (B3h)
  Write_Data_int(0x02); // The write start position is reset to the start of window address area to overwrite the subsequent data to the previous data.
  Write_Data_int(0x00); // OK !
  write_command(0xB6); // DSI Control (B6h)
  Write_Data_int(0x52);
  Write_Data_int(0x83); // Operating frequency (MHz), Min: 140 Max: 210
  write_command(0xB7); // I don't know !
  Write_Data_int(0x80);
  Write_Data_int(0x72);
  Write_Data_int(0x11);
  Write_Data_int(0x25);
  write_command(0xB8); // I don't know !
  Write_Data_int(0x00);
  Write_Data_int(0x0F);
  Write_Data_int(0x0F);
  Write_Data_int(0xFF);
  Write_Data_int(0xFF);
  Write_Data_int(0xC8);
  Write_Data_int(0xC8);
  Write_Data_int(0x02);
  Write_Data_int(0x18);
  Write_Data_int(0x10);
  Write_Data_int(0x10);
  Write_Data_int(0x37);
  Write_Data_int(0x5A);
  Write_Data_int(0x87);
  Write_Data_int(0xBE);
  Write_Data_int(0xFF);
  Write_Data_int(0x00);
  Write_Data_int(0x00);
  Write_Data_int(0x00);
  Write_Data_int(0x00);
  write_command(0xB9); // Backlight Control (2) (B9h)
  Write_Data_int(0x00);
  Write_Data_int(0x00);
  Write_Data_int(0x00);
  Write_Data_int(0x00);
  write_command(0xBD); // Resizing Control (BDh)
  Write_Data_int(0x00);
  write_command(0xC0); // Panel Driving Setting 1 (C0h)
  Write_Data_int(0x02);
  Write_Data_int(0x76);
  write_command(0xC1); // Panel Driving Setting 2 (C1h)
  Write_Data_int(0x63);
  Write_Data_int(0x31);
  Write_Data_int(0x00);
  Write_Data_int(0x27);
  Write_Data_int(0x27);
  Write_Data_int(0x32);
  Write_Data_int(0x12);
  Write_Data_int(0x28);
  Write_Data_int(0x4E);
  Write_Data_int(0x10);
  Write_Data_int(0xA5);
  Write_Data_int(0x0F);
  Write_Data_int(0x58);
  Write_Data_int(0x21);
  Write_Data_int(0x01);
  write_command(0xC2); // Display V-Timing Setting (C2h)
  Write_Data_int(0x28);
  Write_Data_int(0x06);
  Write_Data_int(0x06);
  Write_Data_int(0x01);
  Write_Data_int(0x03);
  Write_Data_int(0x00);
  write_command(0xC4); // Panel Driving Setting 3 (C4h)
  Write_Data_int(0x00);
  Write_Data_int(0x01);
  write_command(0xC6); // Outline Sharpening Control (C6h)
  Write_Data_int(0x00);
  Write_Data_int(0x00);
  write_command(0xC7); // Panel Driving Setting 4 (C7h)
  Write_Data_int(0x11);
  Write_Data_int(0x8D);
  Write_Data_int(0xA0);
  Write_Data_int(0xF5);
  Write_Data_int(0x27);
  write_command(0xC8); // Gamma Setting A Set (C8h)
  Write_Data_int(0x02);
  Write_Data_int(0x13);
  Write_Data_int(0x18);
  Write_Data_int(0x25);
  Write_Data_int(0x34);
  Write_Data_int(0x4E);
  Write_Data_int(0x36);
  Write_Data_int(0x23);
  Write_Data_int(0x17);
  Write_Data_int(0x0E);
  Write_Data_int(0x0C);
  Write_Data_int(0x02);
  Write_Data_int(0x02);
  Write_Data_int(0x13);
  Write_Data_int(0x18);
  Write_Data_int(0x25);
  Write_Data_int(0x34);
  Write_Data_int(0x4E);
  Write_Data_int(0x36);
  Write_Data_int(0x23);
  Write_Data_int(0x17);
  Write_Data_int(0x0E);
  Write_Data_int(0x0C);
  Write_Data_int(0x02);
  write_command(0xC9); // Gamma Setting B Set (C9h)
  Write_Data_int(0x02);
  Write_Data_int(0x13);
  Write_Data_int(0x18);
  Write_Data_int(0x25);
  Write_Data_int(0x34);
  Write_Data_int(0x4E);
  Write_Data_int(0x36);
  Write_Data_int(0x23);
  Write_Data_int(0x17);
  Write_Data_int(0x0E);
  Write_Data_int(0x0C);
  Write_Data_int(0x02);
  Write_Data_int(0x02);
  Write_Data_int(0x13);
  Write_Data_int(0x18);
  Write_Data_int(0x25);
  Write_Data_int(0x34);
  Write_Data_int(0x4E);
  Write_Data_int(0x36);
  Write_Data_int(0x23);
  Write_Data_int(0x17);
  Write_Data_int(0x0E);
  Write_Data_int(0x0C);
  Write_Data_int(0x02);
  write_command(0xCA); // Gamma Setting C Set (CAh)
  Write_Data_int(0x02);
  Write_Data_int(0x13);
  Write_Data_int(0x18);
  Write_Data_int(0x25);
  Write_Data_int(0x34);
  Write_Data_int(0x4E);
  Write_Data_int(0x36);
  Write_Data_int(0x23);
  Write_Data_int(0x17);
  Write_Data_int(0x0E);
  Write_Data_int(0x0C);
  Write_Data_int(0x02);
  Write_Data_int(0x02);
  Write_Data_int(0x13);
  Write_Data_int(0x18);
  Write_Data_int(0x25);
  Write_Data_int(0x34);
  Write_Data_int(0x4E);
  Write_Data_int(0x36);
  Write_Data_int(0x23);
  Write_Data_int(0x17);
  Write_Data_int(0x0E);
  Write_Data_int(0x0C);
  Write_Data_int(0x02);
  write_command(0xD0); // Power Setting (Charge Pump Setting) (D0h)
  Write_Data_int(0xA9);
  Write_Data_int(0x03);
  Write_Data_int(0xCC);
  Write_Data_int(0xA5);
  Write_Data_int(0x00);
  Write_Data_int(0x53);
  Write_Data_int(0x20);
  Write_Data_int(0x10);
  Write_Data_int(0x01);
  Write_Data_int(0x00);
  Write_Data_int(0x01);
  Write_Data_int(0x01);
  Write_Data_int(0x00);
  Write_Data_int(0x03);
  Write_Data_int(0x01);
  Write_Data_int(0x00);
  write_command(0xD1); // Power Setting (Switching Regulator Setting) (D1h)
  Write_Data_int(0x18);
  Write_Data_int(0x0C);
  Write_Data_int(0x23);
  Write_Data_int(0x03);
  Write_Data_int(0x75);
  Write_Data_int(0x02);
  Write_Data_int(0x50);
  write_command(0xD3); // Power Setting for Internal Mode (D3h)
  Write_Data_int(0x33);
  write_command(0xD5); // VPLVL/VNLVL Setting (D5h)
  Write_Data_int(0x2a);
  Write_Data_int(0x2a);
  write_command(0xDE); // VCOMDC Setting (DEh)
  Write_Data_int(0x01);
  Write_Data_int(0x51);//58
  write_command(0xE1); // set_DDB_write_control (E1h)
  Write_Data_int(0x00);
  Write_Data_int(0x00);
  Write_Data_int(0x00);
  Write_Data_int(0x00);
  Write_Data_int(0x00);
  Write_Data_int(0x00);
  write_command(0xE6); // VCOMDC Setting 2 (E6h)
  Write_Data_int(0x55); //58
  write_command(0xFA); // VDC_SEL Setting (FAh)
  Write_Data_int(0x01);
  write_command(0xB0); // MCAP: Manufacturer Command Access Protect
  Write_Data_int(0x04); // Manufacturer Command inputs are enabled - enabled all
  delay(40);
  write_command(0x35); // set_tear_on: 35h
  Write_Data_int(0x00); //
  write_command(0x44); // set_tear_scanline:44h
  Write_Data_int(0x00);
  write_command(0x36); // set_address_mode: 36h -> rotation
  Write_Data_int(0x00);
  write_command(0x3A); // set_pixel_format: 3Ah
  Write_Data_int(0x55); // 16 bits/pixel (65,536 colors)
  write_command(0x29); // set_display_on: 29h
  Write_Data_int(0x00);
  write_command(0x36); // set_address_mode: 36h
  Write_Data_int(0x68);
}


void setup() {
  Serial.begin(115200);
  Serial.println("Start !");
  
  // LCD
  pinMode(16, OUTPUT); // DATA 0
  pinMode(17, OUTPUT); // DATA 1
  pinMode(18, OUTPUT); // DATA 2
  pinMode(19, OUTPUT); // DATA 3
  pinMode(20, OUTPUT); // DATA 4
  pinMode(21, OUTPUT); // DATA 5
  pinMode(22, OUTPUT); // DATA 6
  pinMode(23, OUTPUT); // DATA 7
  pinMode(24, OUTPUT); // DATA 8
  pinMode(25, OUTPUT); // DATA 9
  pinMode(26, OUTPUT); // DATA 10
  pinMode(27, OUTPUT); // DATA 11
  pinMode(28, OUTPUT); // DATA 12
  pinMode(29, OUTPUT); // DATA 13
  pinMode(30, OUTPUT); // DATA 14
  pinMode(31, OUTPUT); // DATA 15
  pinMode(SSD1963_RS, OUTPUT); // RS
  pinMode(SSD1963_WR, OUTPUT); // WR
  // pinMode(SSD1963_RD, OUTPUT); // RD
  // pinMode(SSD1963_CS, OUTPUT); // CS
#ifdef SSD1963_RESET
  pinMode(SSD1963_RESET, OUTPUT); // RESET
  digitalWrite(SSD1963_RESET, 0);
#endif

  // digitalWrite(SSD1963_CS, 0);
  
  digitalWrite(SSD1963_RS, 0);
  digitalWrite(SSD1963_WR, 1);
  // digitalWrite(SSD1963_RD, 1);
  
  SSD1963_Initial();
  Serial.println("Go to loop");
}

void loop() {
  LCD_clear(0xF800);
  LCD_clear(0x07E0);
  LCD_clear(0x001F);
  LCD_clear(0xFFFF);
}
