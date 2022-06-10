#ifndef _FT6336_H
#define _FT6336_H

#include <stm32f4xx_hal.h>  //change the header file for your own STM32 devices

#define FT6336_PORT GPIOB
#define FT6336_SDA GPIO_PIN_3
#define FT6336_SCL GPIO_PIN_4  //change the GPIO port for your own IO connection

#define IIC_SCL(n)  (n?HAL_GPIO_WritePin(FT6336_PORT,FT6336_SCL,GPIO_PIN_SET):HAL_GPIO_WritePin(FT6336_PORT,FT6336_SCL,GPIO_PIN_RESET)) 
#define IIC_SDA(n)  (n?HAL_GPIO_WritePin(FT6336_PORT,FT6336_SDA,GPIO_PIN_SET):HAL_GPIO_WritePin(FT6336_PORT,FT6336_SDA,GPIO_PIN_RESET)) 
#define READ_SDA    HAL_GPIO_ReadPin(FT6336_PORT,FT6336_SDA)  

#define FT6336_ADDR               0x38

#define FT6336_ADDR_DEVICE_MODE   0x00
#define FT6336_ADDR_TD_STATUS     0x02

#define FT6336_ADDR_TOUCH1_EVENT  0x03
#define FT6336_ADDR_TOUCH1_ID     0x05
#define FT6336_ADDR_TOUCH1_XH     0x03
#define FT6336_ADDR_TOUCH1_XL     0x04
#define FT6336_ADDR_TOUCH1_YH     0x05
#define FT6336_ADDR_TOUCH1_YL     0x06

#define FT6336_ADDR_TOUCH2_EVENT  0x09
#define FT6336_ADDR_TOUCH2_ID     0x0B
#define FT6336_ADDR_TOUCH2_XH     0x09
#define FT6336_ADDR_TOUCH2_XL     0x0A
#define FT6336_ADDR_TOUCH2_YH     0x0B
#define FT6336_ADDR_TOUCH2_YL     0x0C

#define FT6336_ADDR_FIRMARE_ID    0xA6

void IIC_Init(void);
unsigned char ft6336_write_byte(unsigned char addr,unsigned char reg,unsigned char data);
unsigned char ft6336_read_byte(unsigned char addr,unsigned char reg);
unsigned char ft6336_get_td_status(void);
void ft6336_get_touch1_position(unsigned int *x,unsigned int *y);
void ft6336_get_touch2_position(unsigned int *x,unsigned int *y);

#endif

