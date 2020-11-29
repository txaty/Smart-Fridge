#include "bsp_ov7725.h"
#include "bsp_sccb.h"
#include "gpio.h"
#include "lcd_tft.h"

OV7725_MODE_PARAM cam_mode =
	{

		.QVGA_VGA = 0,
		.cam_sx = 0,
		.cam_sy = 0,

		.cam_width = 320,
		.cam_height = 240,

		.lcd_sx = 0,
		.lcd_sy = 0,
		.lcd_scan = 3,

		.light_mode = 0,
		.saturation = 0,
		.brightness = 0,
		.contrast = 0,
		.effect = 0,
};

typedef struct Reg
{
	uint8_t Address;
	uint8_t Value;
} Reg_Info;

Reg_Info Sensor_Config[] =
	{
		{REG_CLKRC, 0x00},
		{REG_COM7, 0x46},
		{REG_HSTART, 0x3f},
		{REG_HSIZE, 0x50},
		{REG_VSTRT, 0x03},
		{REG_VSIZE, 0x78},
		{REG_HREF, 0x00},
		{REG_HOutSize, 0x50},
		{REG_VOutSize, 0x78},
		{REG_EXHCH, 0x00},

		/*DSP control*/
		{REG_TGT_B, 0x7f},
		{REG_FixGain, 0x09},
		{REG_AWB_Ctrl0, 0xe0},
		{REG_DSP_Ctrl1, 0xff},
		{REG_DSP_Ctrl2, 0x20},
		{REG_DSP_Ctrl3, 0x00},
		{REG_DSP_Ctrl4, 0x00},

		/*AGC AEC AWB*/
		{REG_COM8, 0xf0},
		{REG_COM4, 0x81}, /*Pll AEC CONFIG*/
		{REG_COM6, 0xc5},
		{REG_COM9, 0x21},
		{REG_BDBase, 0xFF},
		{REG_BDMStep, 0x01},
		{REG_AEW, 0x34},
		{REG_AEB, 0x3c},
		{REG_VPT, 0xa1},
		{REG_EXHCL, 0x00},
		{REG_AWBCtrl3, 0xaa},
		{REG_COM8, 0xff},
		{REG_AWBCtrl1, 0x5d},

		{REG_EDGE1, 0x0a},
		{REG_DNSOff, 0x01},
		{REG_EDGE2, 0x01},
		{REG_EDGE3, 0x01},

		{REG_MTX1, 0x5f},
		{REG_MTX2, 0x53},
		{REG_MTX3, 0x11},
		{REG_MTX4, 0x1a},
		{REG_MTX5, 0x3d},
		{REG_MTX6, 0x5a},
		{REG_MTX_Ctrl, 0x1e},

		{REG_BRIGHT, 0x00},
		{REG_CNST, 0x25},
		{REG_USAT, 0x65},
		{REG_VSAT, 0x65},
		{REG_UVADJ0, 0x81},
		//{REG_SDE,		  0x20},
		{REG_SDE, 0x06},

		/*GAMMA config*/
		{REG_GAM1, 0x0c},
		{REG_GAM2, 0x16},
		{REG_GAM3, 0x2a},
		{REG_GAM4, 0x4e},
		{REG_GAM5, 0x61},
		{REG_GAM6, 0x6f},
		{REG_GAM7, 0x7b},
		{REG_GAM8, 0x86},
		{REG_GAM9, 0x8e},
		{REG_GAM10, 0x97},
		{REG_GAM11, 0xa4},
		{REG_GAM12, 0xaf},
		{REG_GAM13, 0xc5},
		{REG_GAM14, 0xd7},
		{REG_GAM15, 0xe8},
		{REG_SLOP, 0x20},

		{REG_HUECOS, 0x80},
		{REG_HUESIN, 0x80},
		{REG_DSPAuto, 0xff},
		{REG_DM_LNL, 0x00},
		{REG_BDBase, 0x99},
		{REG_BDMStep, 0x03},
		{REG_LC_RADI, 0x00},
		{REG_LC_COEF, 0x13},
		{REG_LC_XC, 0x08},
		{REG_LC_COEFB, 0x14},
		{REG_LC_COEFR, 0x17},
		{REG_LC_CTR, 0x05},

		{REG_COM3, 0xd0}, /*Horizontal mirror image*/

		/*night mode auto frame rate control*/
		{REG_COM5, 0xf5},
		//{REG_COM5,		0x31},
};

uint8_t OV7725_REG_NUM = sizeof(Sensor_Config) / sizeof(Sensor_Config[0]);

// volatile uint8_t Ov7725_vsync;
volatile uint8_t Ov7725_vsync;

static void FIFO_GPIO_Config(void)
{
	// GPIO_InitTypeDef GPIO_InitStructure;

	// RCC_APB2PeriphClockCmd(OV7725_OE_GPIO_CLK | OV7725_WRST_GPIO_CLK |
	// 						   OV7725_RRST_GPIO_CLK | OV7725_RCLK_GPIO_CLK |
	// 						   OV7725_WE_GPIO_CLK | OV7725_DATA_GPIO_CLK,
	// 					   ENABLE);

	// GPIO_InitStructure.Mode = GPIO_Mode_Out_PP;
	// GPIO_InitStructure.Speed = GPIO_Speed_50MHz;
	// GPIO_InitStructure.Pin = OV7725_OE_GPIO_PIN;
	// GPIO_Init(OV7725_OE_GPIO_PORT, &GPIO_InitStructure);

	// GPIO_InitStructure.GPIO_Pin = OV7725_WRST_GPIO_PIN;
	// GPIO_Init(OV7725_WRST_GPIO_PORT, &GPIO_InitStructure);

	// GPIO_InitStructure.GPIO_Pin = OV7725_RRST_GPIO_PIN;
	// GPIO_Init(OV7725_RRST_GPIO_PORT, &GPIO_InitStructure);

	// GPIO_InitStructure.GPIO_Pin = OV7725_RCLK_GPIO_PIN;
	// GPIO_Init(OV7725_RCLK_GPIO_PORT, &GPIO_InitStructure);

	// GPIO_InitStructure.GPIO_Pin = OV7725_WE_GPIO_PIN;
	// GPIO_Init(OV7725_WE_GPIO_PORT, &GPIO_InitStructure);

	// GPIO_InitStructure.GPIO_Pin = OV7725_DATA_0_GPIO_PIN | OV7725_DATA_1_GPIO_PIN |
	// 							  OV7725_DATA_2_GPIO_PIN | OV7725_DATA_3_GPIO_PIN |
	// 							  OV7725_DATA_4_GPIO_PIN | OV7725_DATA_5_GPIO_PIN |
	// 							  OV7725_DATA_6_GPIO_PIN | OV7725_DATA_7_GPIO_PIN;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_Init(OV7725_DATA_GPIO_PORT, &GPIO_InitStructure);

	// Recover from USART3
	switch_pin_for_camera();

	FIFO_OE_L();
	FIFO_WE_H();
}

static void VSYNC_GPIO_Config(void)
{
	// GPIO_InitTypeDef GPIO_InitStructure;
	// EXTI_InitTypeDef EXTI_InitStructure;
	// NVIC_InitTypeDef NVIC_InitStructure;

	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | OV7725_VSYNC_GPIO_CLK, ENABLE);

	// GPIO_InitStructure.Pin = OV7725_VSYNC_GPIO_PIN;
	// GPIO_InitStructure.Mode = GPIO_Mode_IN_FLOATING;
	// GPIO_InitStructure.Speed = GPIO_Speed_50MHz;
	// GPIO_Init(OV7725_VSYNC_GPIO_PORT, &GPIO_InitStructure);

	// GPIO_EXTILineConfig(OV7725_VSYNC_EXTI_SOURCE_PORT, OV7725_VSYNC_EXTI_SOURCE_PIN);
	// EXTI_InitStructure.EXTI_Line = OV7725_VSYNC_EXTI_LINE;
	// EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	// EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	// EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	// EXTI_Init(&EXTI_InitStructure);
	// EXTI_GenerateSWInterrupt(OV7725_VSYNC_EXTI_LINE);

	// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	// NVIC_InitStructure.NVIC_IRQChannel = OV7725_VSYNC_EXTI_IRQ;
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// NVIC_Init(&NVIC_InitStructure);
}

void OV7725_GPIO_Config(void)
{
	SCCB_GPIO_Config();
	FIFO_GPIO_Config();
	VSYNC_GPIO_Config();
}

ErrorStatus OV7725_Init(void)
{
	uint16_t i = 0;
	uint8_t Sensor_IDCode = 0;

	printf("ov7725 Register Config Start......\r\n");

	if (0 == SCCB_WriteByte(0x12, 0x80))
	{
		printf("sccb write data error\r\n");
		return ERROR;
	}

	if (0 == SCCB_ReadByte(&Sensor_IDCode, 1, 0x0b))
	{
		printf("read id failed\r\n");
		return ERROR;
	}
	printf("Sensor ID is 0x%x\r\n", Sensor_IDCode);

	if (Sensor_IDCode == OV7725_ID)
	{
		for (i = 0; i < OV7725_REG_NUM; i++)
		{
			if (0 == SCCB_WriteByte(Sensor_Config[i].Address, Sensor_Config[i].Value))
			{
				printf("write reg failed\r\n");
				return ERROR;
			}
		}
	}
	else
	{
		printf("Sensor ID not match\r\n");
		return ERROR;
	}
	printf("ov7725 Register Config Success\r\n");

	return SUCCESS;
}

void OV7725_Light_Mode(uint8_t mode)
{
	switch (mode)
	{
	case 0:							//Auto
		SCCB_WriteByte(0x13, 0xff); //AWB on
		SCCB_WriteByte(0x0e, 0x65);
		SCCB_WriteByte(0x2d, 0x00);
		SCCB_WriteByte(0x2e, 0x00);
		break;
	case 1:							//sunny
		SCCB_WriteByte(0x13, 0xfd); //AWB off
		SCCB_WriteByte(0x01, 0x5a);
		SCCB_WriteByte(0x02, 0x5c);
		SCCB_WriteByte(0x0e, 0x65);
		SCCB_WriteByte(0x2d, 0x00);
		SCCB_WriteByte(0x2e, 0x00);
		break;
	case 2:							//cloudy
		SCCB_WriteByte(0x13, 0xfd); //AWB off
		SCCB_WriteByte(0x01, 0x58);
		SCCB_WriteByte(0x02, 0x60);
		SCCB_WriteByte(0x0e, 0x65);
		SCCB_WriteByte(0x2d, 0x00);
		SCCB_WriteByte(0x2e, 0x00);
		break;
	case 3:							//office
		SCCB_WriteByte(0x13, 0xfd); //AWB off
		SCCB_WriteByte(0x01, 0x84);
		SCCB_WriteByte(0x02, 0x4c);
		SCCB_WriteByte(0x0e, 0x65);
		SCCB_WriteByte(0x2d, 0x00);
		SCCB_WriteByte(0x2e, 0x00);
		break;
	case 4:							//home
		SCCB_WriteByte(0x13, 0xfd); //AWB off
		SCCB_WriteByte(0x01, 0x96);
		SCCB_WriteByte(0x02, 0x40);
		SCCB_WriteByte(0x0e, 0x65);
		SCCB_WriteByte(0x2d, 0x00);
		SCCB_WriteByte(0x2e, 0x00);
		break;

	case 5:							//night
		SCCB_WriteByte(0x13, 0xff); //AWB on
		SCCB_WriteByte(0x0e, 0xe5);
		break;

	default:
		OV7725_DEBUG("Light Mode parameter error!");

		break;
	}
}

void OV7725_Color_Saturation(int8_t sat)
{

	if (sat >= -4 && sat <= 4)
	{
		SCCB_WriteByte(REG_USAT, (sat + 4) << 4);
		SCCB_WriteByte(REG_VSAT, (sat + 4) << 4);
	}
	else
	{
		OV7725_DEBUG("Color Saturation parameter error!");
	}
}

void OV7725_Brightness(int8_t bri)
{
	uint8_t BRIGHT_Value, SIGN_Value;

	switch (bri)
	{
	case 4:
		BRIGHT_Value = 0x48;
		SIGN_Value = 0x06;
		break;

	case 3:
		BRIGHT_Value = 0x38;
		SIGN_Value = 0x06;
		break;

	case 2:
		BRIGHT_Value = 0x28;
		SIGN_Value = 0x06;
		break;

	case 1:
		BRIGHT_Value = 0x18;
		SIGN_Value = 0x06;
		break;

	case 0:
		BRIGHT_Value = 0x08;
		SIGN_Value = 0x06;
		break;

	case -1:
		BRIGHT_Value = 0x08;
		SIGN_Value = 0x0e;
		break;

	case -2:
		BRIGHT_Value = 0x18;
		SIGN_Value = 0x0e;
		break;

	case -3:
		BRIGHT_Value = 0x28;
		SIGN_Value = 0x0e;
		break;

	case -4:
		BRIGHT_Value = 0x38;
		SIGN_Value = 0x0e;
		break;

	default:
		OV7725_DEBUG("Brightness parameter error!");
		break;
	}

	SCCB_WriteByte(REG_BRIGHT, BRIGHT_Value); //AWB on
	SCCB_WriteByte(REG_SIGN, SIGN_Value);
}

void OV7725_Contrast(int8_t cnst)
{
	if (cnst >= -4 && cnst <= 4)
	{
		SCCB_WriteByte(REG_CNST, (0x30 - (4 - cnst) * 4));
	}
	else
	{
		OV7725_DEBUG("Contrast parameter error!");
	}
}

void OV7725_Special_Effect(uint8_t eff)
{
	switch (eff)
	{
	case 0:
		SCCB_WriteByte(0xa6, 0x06);
		SCCB_WriteByte(0x60, 0x80);
		SCCB_WriteByte(0x61, 0x80);
		break;

	case 1:
		SCCB_WriteByte(0xa6, 0x26);
		SCCB_WriteByte(0x60, 0x80);
		SCCB_WriteByte(0x61, 0x80);
		break;

	case 2:
		SCCB_WriteByte(0xa6, 0x1e);
		SCCB_WriteByte(0x60, 0xa0);
		SCCB_WriteByte(0x61, 0x40);
		break;

	case 3:
		SCCB_WriteByte(0xa6, 0x1e);
		SCCB_WriteByte(0x60, 0x40);
		SCCB_WriteByte(0x61, 0xa0);
		break;

	case 4:
		SCCB_WriteByte(0xa6, 0x1e);
		SCCB_WriteByte(0x60, 0x80);
		SCCB_WriteByte(0x61, 0xc0);
		break;

	case 5:
		SCCB_WriteByte(0xa6, 0x1e);
		SCCB_WriteByte(0x60, 0x60);
		SCCB_WriteByte(0x61, 0x60);
		break;

	case 6:
		SCCB_WriteByte(0xa6, 0x46);
		break;

	default:
		OV7725_DEBUG("Special Effect error!");
		break;
	}
}

void OV7725_Window_Set(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height, uint8_t QVGA_VGA)
{
	uint8_t reg_raw, cal_temp;

	/***********QVGA or VGA *************/
	if (QVGA_VGA == 0)
	{
		/*QVGA RGB565 */
		SCCB_WriteByte(REG_COM7, 0x46);
	}
	else
	{
		/*VGA RGB565 */
		SCCB_WriteByte(REG_COM7, 0x06);
	}

	/***************HSTART*********************/
	SCCB_ReadByte(&reg_raw, 1, REG_HSTART);

	cal_temp = (reg_raw + (sx >> 2));
	SCCB_WriteByte(REG_HSTART, cal_temp);

	/***************HSIZE*********************/
	SCCB_WriteByte(REG_HSIZE, width >> 2); //HSIZE������λ

	/***************VSTART*********************/
	SCCB_ReadByte(&reg_raw, 1, REG_VSTRT);
	cal_temp = (reg_raw + (sy >> 1));

	SCCB_WriteByte(REG_VSTRT, cal_temp);

	/***************VSIZE*********************/
	SCCB_WriteByte(REG_VSIZE, height >> 1); //VSIZE����һλ

	/***************VSTART*********************/
	SCCB_ReadByte(&reg_raw, 1, REG_HREF);
	cal_temp = (reg_raw | (width & 0x03) | ((height & 0x01) << 2) | ((sx & 0x03) << 4) | ((sy & 0x01) << 6));

	SCCB_WriteByte(REG_HREF, cal_temp);

	/***************HOUTSIZIE /VOUTSIZE*********************/
	SCCB_WriteByte(REG_HOutSize, width >> 2);
	SCCB_WriteByte(REG_VOutSize, height >> 1);

	SCCB_ReadByte(&reg_raw, 1, REG_EXHCH);
	cal_temp = (reg_raw | (width & 0x03) | ((height & 0x01) << 2));

	SCCB_WriteByte(REG_EXHCH, cal_temp);
}

void OV7725_Window_VGA_Set(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{

	uint8_t reg_raw, cal_temp;

	/***********QVGA or VGA *************/
	/*VGA RGB565 */
	SCCB_WriteByte(REG_COM7, 0x06);

	/***************HSTART*********************/
	SCCB_ReadByte(&reg_raw, 1, REG_HSTART);

	cal_temp = (reg_raw + (sx >> 2));
	SCCB_WriteByte(REG_HSTART, cal_temp);

	/***************HSIZE*********************/
	SCCB_WriteByte(REG_HSIZE, width >> 2); //HSIZE������λ 320

	/***************VSTART*********************/
	SCCB_ReadByte(&reg_raw, 1, REG_VSTRT);
	cal_temp = (reg_raw + (sy >> 1));

	SCCB_WriteByte(REG_VSTRT, cal_temp);

	/***************VSIZE*********************/
	SCCB_WriteByte(REG_VSIZE, height >> 1); //VSIZE����һλ 240

	/***************VSTART*********************/
	SCCB_ReadByte(&reg_raw, 1, REG_HREF);
	cal_temp = (reg_raw | (width & 0x03) | ((height & 0x01) << 2) | ((sx & 0x03) << 4) | ((sy & 0x01) << 6));

	SCCB_WriteByte(REG_VSTRT, cal_temp);

	/***************HOUTSIZIE /VOUTSIZE*********************/
	SCCB_WriteByte(REG_HOutSize, width >> 2);
	SCCB_WriteByte(REG_VOutSize, height >> 1);

	SCCB_ReadByte(&reg_raw, 1, REG_EXHCH);

	cal_temp = (reg_raw | (width & 0x03) | ((height & 0x01) << 2));

	SCCB_WriteByte(REG_EXHCH, cal_temp);
}

void OV7725_VSYNC_EXTI_INT_FUNCTION ( void )
{
    if ( __HAL_GPIO_EXTI_GET_IT(OV7725_VSYNC_GPIO_PIN) != RESET ) {
        if ( Ov7725_vsync == 0 ) {
            FIFO_WRST_L();
            FIFO_WE_H();  

            Ov7725_vsync = 1;
            FIFO_WE_H();    
            FIFO_WRST_H(); 
        } else if ( Ov7725_vsync == 1 ) {
            FIFO_WE_L();  
            Ov7725_vsync = 2;
        }
        __HAL_GPIO_EXTI_CLEAR_IT(OV7725_VSYNC_GPIO_PIN);
		HAL_GPIO_EXTI_Callback(OV7725_VSYNC_GPIO_PIN);
    }
}