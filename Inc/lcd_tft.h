#ifndef __LCD_H
#define __LCD_H

#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "lvgl.h"

// LED definitions
#define FSMC_Addr_LCD_CMD ((uint32_t)0x60000000)
#define FSMC_Addr_LCD_DATA ((uint32_t)0x60020000)

#define FSMC_Bank1_NORSRAMx FSMC_Bank1_NORSRAM1

#define LCD_RST_APBxClock_FUN RCC_APB2PeriphClockCmd
#define LCD_RST_CLK RCC_APB2Periph_GPIOE
#define LCD_RST_PORT GPIOE
#define LCD_RST_PIN GPIO_PIN_1

#define LCD_BK_APBxClock_FUN RCC_APB2PeriphClockCmd
#define LCD_BK_CLK RCC_APB2Periph_GPIOD
#define LCD_BK_PORT GPIOD
#define LCD_BK_PIN GPIO_PIN_12

#define DEBUG_DELAY()

#define LCD_Default_Max_COLUMN 240
#define LCD_Default_Max_PAGE 320

#define LCD_DispWindow_Start_COLUMN 0
#define LCD_DispWindow_Start_PAGE 0

#define LCD_DispWindow_COLUMN 240
#define LCD_DispWindow_PAGE 320

#define WIDTH_EN_CHAR 8
#define HEIGHT_EN_CHAR 16

#define GetGBKCode(ucBuffer, usChar)

#define CMD_Set_COLUMN 0x2A
#define CMD_Set_PAGE 0x2B
#define CMD_SetPixel 0x2C

// TFT definitions
#define RCC_APB2Periph_GPIOD   ((uint32_t)0x00000020)
#define RCC_APB2Periph_GPIOE   ((uint32_t)0x00000040)

#define XPT2046_PENIRQ_GPIO_CLK RCC_APB2Periph_GPIOE
#define XPT2046_PENIRQ_GPIO_PORT GPIOE
#define XPT2046_PENIRQ_GPIO_PIN GPIO_PIN_4

#define XPT2046_PENIRQ_ActiveLevel 0
#define XPT2046_PENIRQ_Read() HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)
#define XPT2046_SPI_GPIO_CLK RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOD

#define XPT2046_SPI_CS_PIN GPIO_PIN_13
#define XPT2046_SPI_CS_PORT GPIOD

#define XPT2046_SPI_CLK_PIN GPIO_PIN_0
#define XPT2046_SPI_CLK_PORT GPIOE

#define XPT2046_SPI_MOSI_PIN GPIO_PIN_2
#define XPT2046_SPI_MOSI_PORT GPIOE

#define XPT2046_SPI_MISO_PIN GPIO_PIN_3
#define XPT2046_SPI_MISO_PORT GPIOE

#define XPT2046_CS_ENABLE() HAL_GPIO_WritePin(XPT2046_SPI_CS_PORT, XPT2046_SPI_CS_PIN,GPIO_PIN_SET)
#define XPT2046_CS_DISABLE() HAL_GPIO_WritePin(XPT2046_SPI_CS_PORT, XPT2046_SPI_CS_PIN,GPIO_PIN_RESET)

#define XPT2046_CLK_HIGH() HAL_GPIO_WritePin(XPT2046_SPI_CLK_PORT, XPT2046_SPI_CLK_PIN,GPIO_PIN_SET)
#define XPT2046_CLK_LOW() HAL_GPIO_WritePin(XPT2046_SPI_CLK_PORT, XPT2046_SPI_CLK_PIN,GPIO_PIN_RESET)

#define XPT2046_MOSI_1() HAL_GPIO_WritePin(XPT2046_SPI_MOSI_PORT, XPT2046_SPI_MOSI_PIN,GPIO_PIN_SET)
#define XPT2046_MOSI_0() HAL_GPIO_WritePin(XPT2046_SPI_MOSI_PORT, XPT2046_SPI_MOSI_PIN,GPIO_PIN_RESET)

#define XPT2046_MISO() HAL_GPIO_ReadPin(XPT2046_SPI_MISO_PORT, XPT2046_SPI_MISO_PIN)

#define TOUCH_NOT_PRESSED 0
#define TOUCH_PRESSED 1

#define  XPT2046_CHANNEL_X    0x90            
#define  XPT2046_CHANNEL_Y    0xd0  

typedef enum
{
    XPT2046_STATE_RELEASE = 0,
    XPT2046_STATE_WAITING,
    XPT2046_STATE_PRESSED,
} Touch_State;

// LCD functions
void LCD_Init(void);
void LCD_Rst(void);
void LCD_REG_Config(void);
void LCD_FillColor(uint32_t ulAmout_Point, uint16_t usColor);
uint16_t LCD_Read_PixelData(void);
void LCD_BackLed_Control(FunctionalState enumState);
void LCD_Write_Cmd(uint16_t usCmd);
void LCD_Write_Data(uint16_t usData);
uint16_t LCD_Read_Data(void);
void LCD_Clear(uint16_t usC, uint16_t usP, uint16_t usWidth, uint16_t usHeight, uint16_t usColor);
uint16_t LCD_GetPointPixel(uint16_t usC, uint16_t usP);
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void XPT2046_ReadAdc_XY ( int16_t * sX_Ad, int16_t * sY_Ad );
// TFT functions
void XPT2046_Init(void);
void XPT2046_SendCMD(uint8_t cmd);
void XPT2046_DelayUS(__IO uint32_t ulCount);
uint16_t XPT2046_ReceiveData(void)	;
uint8_t touch_detect(void);
#endif
