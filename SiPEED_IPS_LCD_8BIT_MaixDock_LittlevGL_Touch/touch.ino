
#define FT5216_ADDR 0x38

void touch_init() {
  Wire.begin();
}

int touch_read(uint16_t *cx, uint16_t *cy) {
  Wire.beginTransmission(FT5216_ADDR);
  Wire.write(0x02); // Set point to TD_STATUS 
  if (Wire.endTransmission(false) != 0) {
    Serial.println("Write error !");
    return 0;
  }
  uint8_t count = Wire.requestFrom(FT5216_ADDR, 5, true);
  if (Wire.available() <= 0) {
    Serial.println(String("Read error !, ") + Wire.available());
    return 0;
  }

  // Process Data
  uint8_t TD_STATUS = Wire.read();
  uint8_t TOUCH1_XH = Wire.read();
  uint8_t TOUCH1_XL = Wire.read();
  uint8_t TOUCH1_YH = Wire.read();
  uint8_t TOUCH1_YL = Wire.read();

  *cx = (((uint16_t)TOUCH1_XH&0x0F)<<8)|TOUCH1_XL;
  *cy = (((uint16_t)TOUCH1_YH&0x0F)<<8)|TOUCH1_YL;

  return TD_STATUS&0x0F;
}
