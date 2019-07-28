// Pin Control
#define LCD_RS     26    // Data or Command
#define LCD_WR     27    // Write
// #define LCD_RD     34    // Read
// #define LCD_CS     35    // Chip select
// #define LCD_RESET  31    // Reset

/*
 * ==== Wireing ====
 *  LCD  | K210(MCU)
 *   D0 <-> IO18
 *   D1 <-> IO19
 *   D2 <-> IO20
 *   D3 <-> IO21
 *   D4 <-> IO22
 *   D5 <-> IO23
 *   D6 <-> IO24
 *   D7 <-> IO25
 *   RS <-> IO26
 *   WR <-> IO27
 *   RD <-> 3V3
 *   CS <-> GND
 *   RESET <-> RESET
 */

#ifdef LCD_RESET
#define SET_LCD_RESET() { digitalWrite(LCD_RESET, 1); \
    delay(50); \
    digitalWrite(LCD_RESET, 0); \
    delay(100); \
    digitalWrite(LCD_RESET, 1); \
    delay(50); }
#endif

// #define SET_LCD_CS(a) digitalWrite(LCD_CS, a)
#define SET_LCD_RS(a) digitalWrite(LCD_RS, a)
#define SET_LCD_WR(a) digitalWrite(LCD_WR, a)

#define SET_LCD_WR_ACTIVE() *gpiohs->output_val.u32 |= (1<<(LCD_WR-16));
#define SET_LCD_WR_NON_ACTIVE() *gpiohs->output_val.u32 &= ~(1<<(LCD_WR-16));

#define SET_LCD_RS_DATA() *gpiohs->output_val.u32 |= (1<<(LCD_RS-16));
#define SET_LCD_RS_COMMAND() *gpiohs->output_val.u32 &= ~(1<<(LCD_RS-16));

#define SET_LCD_DATA(a) *gpiohs->output_val.u32=(*gpiohs->output_val.u32&0xFFFFFC03)|((uint32_t)(a)<<2)

#define SET_LCD_WRITE() { SET_LCD_WR_NON_ACTIVE(); \
                          SET_LCD_WR_ACTIVE(); }

// Config LCD
#define LCD_WIDTH  800
#define LCD_HEIGHT 480

void setup() {
  Serial.begin(115200);
  Serial.println("Start !");
  
  // LCD
  LCD_Initial();
  Serial.println("Go to loop");
}

void loop() {
  LCD_clear(0xF800);
  LCD_clear(0x07E0);
  LCD_clear(0x001F);
  LCD_clear(0xFFFF);
}
