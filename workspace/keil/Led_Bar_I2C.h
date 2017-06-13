
#ifndef Led_Bar_I2C_H
#define Led_Bar_I2C_H
#define addr 0x00  // Work on 8-bit mode
#define GLB_ON      0xff  // 8-bit 1 data
#define GLB_OFF     0x00  // 8-bit 0 data

void setI2CLed(I2C_HandleTypeDef* hi2c, uint8_t* data,int led, float brightness);
void sendI2C(I2C_HandleTypeDef* hi2c, uint8_t* data, uint16_t size);

#endif
