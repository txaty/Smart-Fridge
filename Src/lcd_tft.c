#include "lcd_tft.h"

// TFT variables
const float COOR_X_K = 0.09f;
const float COOR_Y_K = 0.068f;
const int COOR_X_B = -170;
const int COOR_Y_B = -180;

// LED functions
void Delay(__IO uint32_t nCount)
{
	for (; nCount != 0; nCount--)
		;
}

static inline void LCD_Write_Cmd(uint16_t usCmd)
{
	*(__IO uint16_t *)(FSMC_Addr_LCD_CMD) = usCmd;
}

static inline void LCD_Write_Data(uint16_t usData)
{
	*(__IO uint16_t *)(FSMC_Addr_LCD_DATA) = usData;
}

void LCD_REG_Config(void)
{
	/*  Power control B (CFh)  */
	DEBUG_DELAY();
	LCD_Write_Cmd(0xCF);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x81);
	LCD_Write_Data(0x30);

	/*  Power on sequence control (EDh) */
	DEBUG_DELAY();
	LCD_Write_Cmd(0xED);
	LCD_Write_Data(0x64);
	LCD_Write_Data(0x03);
	LCD_Write_Data(0x12);
	LCD_Write_Data(0x81);

	/*  Driver timing control A (E8h) */
	DEBUG_DELAY();
	LCD_Write_Cmd(0xE8);
	LCD_Write_Data(0x85);
	LCD_Write_Data(0x10);
	LCD_Write_Data(0x78);

	/*  Power control A (CBh) */
	DEBUG_DELAY();
	LCD_Write_Cmd(0xCB);
	LCD_Write_Data(0x39);
	LCD_Write_Data(0x2C);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x34);
	LCD_Write_Data(0x02);

	/* Pump ratio control (F7h) */
	DEBUG_DELAY();
	LCD_Write_Cmd(0xF7);
	LCD_Write_Data(0x20);

	/* Driver timing control B */
	DEBUG_DELAY();
	LCD_Write_Cmd(0xEA);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x00);

	/* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
	DEBUG_DELAY();
	LCD_Write_Cmd(0xB1);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x1B);

	/*  Display Function Control (B6h) */
	DEBUG_DELAY();
	LCD_Write_Cmd(0xB6);
	LCD_Write_Data(0x0A);
	LCD_Write_Data(0xA2);

	/* Power Control 1 (C0h) */
	DEBUG_DELAY();
	LCD_Write_Cmd(0xC0);
	LCD_Write_Data(0x35);

	/* Power Control 2 (C1h) */
	DEBUG_DELAY();
	LCD_Write_Cmd(0xC1);
	LCD_Write_Data(0x11);

	/* VCOM Control 1 (C5h) */
	LCD_Write_Cmd(0xC5);
	LCD_Write_Data(0x45);
	LCD_Write_Data(0x45);

	/*  VCOM Control 2 (C7h)  */
	LCD_Write_Cmd(0xC7);
	LCD_Write_Data(0xA2);

	/* Enable 3G (F2h) */
	LCD_Write_Cmd(0xF2);
	LCD_Write_Data(0x00);

	/* Gamma Set (26h) */
	LCD_Write_Cmd(0x26);
	LCD_Write_Data(0x01);
	DEBUG_DELAY();

	/* Positive Gamma Correction */
	LCD_Write_Cmd(0xE0); //Set Gamma
	LCD_Write_Data(0x0F);
	LCD_Write_Data(0x26);
	LCD_Write_Data(0x24);
	LCD_Write_Data(0x0B);
	LCD_Write_Data(0x0E);
	LCD_Write_Data(0x09);
	LCD_Write_Data(0x54);
	LCD_Write_Data(0xA8);
	LCD_Write_Data(0x46);
	LCD_Write_Data(0x0C);
	LCD_Write_Data(0x17);
	LCD_Write_Data(0x09);
	LCD_Write_Data(0x0F);
	LCD_Write_Data(0x07);
	LCD_Write_Data(0x00);

	/* Negative Gamma Correction (E1h) */
	LCD_Write_Cmd(0XE1); //Set Gamma
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x19);
	LCD_Write_Data(0x1B);
	LCD_Write_Data(0x04);
	LCD_Write_Data(0x10);
	LCD_Write_Data(0x07);
	LCD_Write_Data(0x2A);
	LCD_Write_Data(0x47);
	LCD_Write_Data(0x39);
	LCD_Write_Data(0x03);
	LCD_Write_Data(0x06);
	LCD_Write_Data(0x06);
	LCD_Write_Data(0x30);
	LCD_Write_Data(0x38);
	LCD_Write_Data(0x0F);

	/* memory access control set */
	DEBUG_DELAY();
	LCD_Write_Cmd(0x36);
	LCD_Write_Data(0xC8);
	DEBUG_DELAY();

	/* column address control set */
	LCD_Write_Cmd(CMD_Set_COLUMN);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0xEF);

	/* page address control set */
	DEBUG_DELAY();
	LCD_Write_Cmd(CMD_Set_PAGE);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x01);
	LCD_Write_Data(0x3F);

	/*  Pixel Format Set (3Ah)  */
	DEBUG_DELAY();
	LCD_Write_Cmd(0x3a);
	LCD_Write_Data(0x55);

	/* Sleep Out (11h)  */
	LCD_Write_Cmd(0x11);
	Delay(0xAFFf << 2);
	DEBUG_DELAY();

	/* Display ON (29h) */
	LCD_Write_Cmd(0x29);
}

void LCD_Init(void)
{
	LCD_BackLed_Control(ENABLE);
	LCD_Rst();
	LCD_REG_Config();
}

void LCD_Rst(void)
{
	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET);
	Delay(0xAFFf << 2);
	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);
	Delay(0xAFFf << 2);
}

void LCD_BackLed_Control(FunctionalState enumState)
{
	if (enumState)
		HAL_GPIO_WritePin(LCD_BK_PORT, LCD_BK_PIN, GPIO_PIN_RESET);
	else
		HAL_GPIO_WritePin(LCD_BK_PORT, LCD_BK_PIN, GPIO_PIN_SET);
}

uint16_t LCD_Read_Data(void)
{
	return (*(__IO uint16_t *)(FSMC_Addr_LCD_DATA));
}

static inline void LCD_OpenWindow ( uint16_t usCOLUMN, uint16_t usPAGE, uint16_t usWidth, uint16_t usHeight )
{	
	LCD_Write_Cmd ( CMD_Set_COLUMN ); 				
	LCD_Write_Data ( usCOLUMN >> 8  );	 
	LCD_Write_Data ( usCOLUMN & 0xff  );	 
	LCD_Write_Data ( ( usCOLUMN + usWidth - 1 ) >> 8  );
	LCD_Write_Data ( ( usCOLUMN + usWidth - 1 ) & 0xff  );

	LCD_Write_Cmd ( CMD_Set_PAGE ); 			     
	LCD_Write_Data ( usPAGE >> 8  );
	LCD_Write_Data ( usPAGE & 0xff  );
	LCD_Write_Data ( ( usPAGE + usHeight - 1 ) >> 8 );
	LCD_Write_Data ( ( usPAGE + usHeight - 1) & 0xff );
	
}

static inline void LCD_FillColor ( uint32_t usPoint, uint16_t usColor )
{
	uint32_t i = 0;
	
	/* memory write */
	LCD_Write_Cmd ( CMD_SetPixel );	
		
	for ( i = 0; i < usPoint; i ++ )
		LCD_Write_Data ( usColor );
		
}

static inline void LCD_Clear(uint16_t usCOLUMN, uint16_t usPAGE, uint16_t usWidth, uint16_t usHeight, uint16_t usColor)
{
	LCD_OpenWindow(usCOLUMN, usPAGE, usWidth, usHeight);

	LCD_FillColor(usWidth * usHeight, usColor);
}

uint16_t LCD_Read_PixelData(void)
{
	uint16_t usR = 0, usG = 0, usB = 0;

	LCD_Write_Cmd(0x2E);

	usR = LCD_Read_Data(); /*FIRST READ OUT DUMMY DATA*/
	usR = LCD_Read_Data(); /*READ OUT RED DATA  */
	usB = LCD_Read_Data(); /*READ OUT BLUE DATA*/
	usG = LCD_Read_Data(); /*READ OUT GREEN DATA*/

	return (((usR >> 11) << 11) | ((usG >> 10) << 5) | (usB >> 11));
}

static inline uint16_t LCD_GetPointPixel(uint16_t usCOLUMN, uint16_t usPAGE)
{
	uint16_t usPixelData;

	LCD_OpenWindow(usCOLUMN, usPAGE, 1, 1);

	usPixelData = LCD_Read_PixelData();

	return usPixelData;
}

static inline void LCD_DrawDot(uint16_t usCOLUMN, uint16_t usPAGE, uint16_t usColor)
{
	LCD_Write_Cmd(CMD_Set_COLUMN);
	LCD_Write_Data(usCOLUMN >> 8);
	LCD_Write_Data(usCOLUMN & 0xff);
	LCD_Write_Cmd(CMD_Set_PAGE);
	LCD_Write_Data(usPAGE >> 8);
	LCD_Write_Data(usPAGE & 0xff);

	LCD_Write_Cmd(CMD_SetPixel);
	LCD_Write_Data(usColor);
}

inline void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
	int32_t x, y;
	
	for (y = area->y1; y <= area->y2; y++)
	{
		for (x = area->x1; x <= area->x2; x++)
		{
			LCD_DrawDot(y, 320 - x, color_p->full);
			color_p++;
		}
	}
	lv_disp_flush_ready(disp);
}


// TFT functions
void XPT2046_Init(void)
{
	// GPIO_InitTypeDef GPIO_InitStructure;
	__HAL_RCC_GPIOE_CLK_ENABLE();
}

static inline void XPT2046_DelayUS(__IO uint32_t ulCount)
{
	uint32_t i;
	for (i = 0; i < ulCount; i++)
	{
		uint8_t uc = 12;

		while (uc--)
			;
	}
}

static inline void XPT2046_SendCMD(uint8_t cmd)
{
	uint8_t i;
	XPT2046_CS_ENABLE();
	XPT2046_CLK_LOW();
	XPT2046_MOSI_0();
	XPT2046_DelayUS(10);
	XPT2046_CS_DISABLE();
	for (i = 0; i < 8; i++)
	{
		if ((cmd & (0x80 >> i)) == 0)
		{
			XPT2046_MOSI_0();
		}
		else
		{
			XPT2046_MOSI_1();
		}
		XPT2046_CLK_HIGH();
		XPT2046_DelayUS(5);
		XPT2046_CLK_LOW();
		XPT2046_DelayUS(5);
	}
}

static inline uint16_t XPT2046_ReceiveData(void)
{
	uint8_t i;
	uint16_t receive_temp = 0;
	XPT2046_CLK_HIGH();
	XPT2046_DelayUS(5);
	XPT2046_CLK_LOW();
	XPT2046_DelayUS(5);
	for (i = 0; i < 12; i++)
	{
		receive_temp = receive_temp << 1;
		XPT2046_CLK_HIGH();
		XPT2046_DelayUS(5);
		if (XPT2046_MISO() == GPIO_PIN_SET)
		{
			receive_temp |= 0x01;
		}
		else
		{
			receive_temp |= 0x00;
		}
		XPT2046_CLK_LOW();
		XPT2046_DelayUS(5);
	}

	XPT2046_CS_ENABLE();
	return receive_temp;
}

static inline uint16_t XPT2046_ReadAdc(uint8_t ucChannel)
{
	XPT2046_SendCMD(ucChannel);
	return XPT2046_ReceiveData();
}

static inline void XPT2046_ReadAdc_XY(int16_t *sX_Ad, int16_t *sY_Ad)
{
	int16_t sX_Ad_Temp, sY_Ad_Temp;
	sX_Ad_Temp = XPT2046_ReadAdc(XPT2046_CHANNEL_X);
	XPT2046_DelayUS(1);
	sY_Ad_Temp = XPT2046_ReadAdc(XPT2046_CHANNEL_Y);
	*sX_Ad = (int16_t)(COOR_X_K * (sX_Ad_Temp + COOR_X_B));
	*sY_Ad = (int16_t)(COOR_Y_K * (sY_Ad_Temp + COOR_Y_B));
}

static inline uint8_t touch_detect(void)
{
	static Touch_State touch_state = XPT2046_STATE_RELEASE;
	static uint8_t i;
	uint8_t result;
	switch (touch_state)
	{
	case XPT2046_STATE_RELEASE:
		if (XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel)
		{
			touch_state = XPT2046_STATE_WAITING;
			result = TOUCH_NOT_PRESSED;
		}
		else
		{
			touch_state = XPT2046_STATE_RELEASE;
			result = TOUCH_NOT_PRESSED;
		}
		break;
	case XPT2046_STATE_WAITING:
		if (XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel)
		{
			i++;

			if (i > 10)
			{
				touch_state = XPT2046_STATE_PRESSED;
				result = TOUCH_PRESSED;
			}
			else
			{
				touch_state = XPT2046_STATE_WAITING;
				result = TOUCH_NOT_PRESSED;
			}
		}
		else
		{
			i = 0;
			touch_state = XPT2046_STATE_RELEASE;
			result = TOUCH_NOT_PRESSED;
		}
		break;
	case XPT2046_STATE_PRESSED:
		if (XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel)
		{
			touch_state = XPT2046_STATE_PRESSED;
			result = TOUCH_PRESSED;
		}
		else
		{
			touch_state = XPT2046_STATE_RELEASE;
			result = TOUCH_NOT_PRESSED;
		}
		break;
	}
	return result;
}

inline bool my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data)
{
	data->state = touch_detect() ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
	if (data->state == LV_INDEV_STATE_PR)
		XPT2046_ReadAdc_XY(&data->point.x, &data->point.y);
	return false;
}