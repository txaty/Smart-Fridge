#include "bsp_sccb.h"
#include "gpio.h"

#define DEV_ADR ADDR_OV7725

void SCCB_GPIO_Config(void)
{
	// GPIO_InitTypeDef GPIO_InitStructure;
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	// GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	// GPIO_Init(GPIOC, &GPIO_InitStructure);

	// Configure PC6, PC7 as GPIO_output open drain
}

static void SCCB_delay(void)
{
	uint16_t i = 300;
	while (i)
	{
		i--;
	}
}

static int SCCB_Start(void)
{
	SDA_H;
	SCL_H;
	delay_us(50);
	if (!SDA_read) {
		return DISABLE;
	}
	SDA_L;
	delay_us(50);
	if (SDA_read) {
		return DISABLE;
	}
	SDA_L;
	delay_us(50);
	return ENABLE;
}

static void SCCB_Stop(void)
{
	SCL_L;
	delay_us(50);
	SDA_L;
	delay_us(50);
	SCL_H;
	delay_us(50);
	SDA_H;
	delay_us(50);
}

static void SCCB_Ack(void)
{
	SCL_L;
	delay_us(50);
	SDA_L;
	delay_us(50);
	SCL_H;
	delay_us(50);
	SCL_L;
	delay_us(50);
}

static void SCCB_NoAck(void)
{
	SCL_L;
	delay_us(50);
	SDA_H;
	delay_us(50);
	SCL_H;
	delay_us(50);
	SCL_L;
	delay_us(50);
}

static int SCCB_WaitAck(void)
{
	SCL_L;
	delay_us(50);
	SDA_H;
	delay_us(50);
	SCL_H;
	delay_us(50);
	if (SDA_read)
	{
		SCL_L;
		return DISABLE;
	}
	SCL_L;
	return ENABLE;
}

static void SCCB_SendByte(uint8_t SendByte)
{
	uint8_t i = 8;
	while (i--)
	{
		SCL_L;
		delay_us(50);
		if (SendByte & 0x80)
			SDA_H;
		else
			SDA_L;
		SendByte <<= 1;
		delay_us(50);
		SCL_H;
		delay_us(50);
	}
	SCL_L;
}

static int SCCB_ReceiveByte(void)
{
	uint8_t i = 8;
	uint8_t ReceiveByte = 0;

	SDA_H;
	while (i--)
	{
		ReceiveByte <<= 1;
		SCL_L;
		delay_us(50);
		SCL_H;
		delay_us(50);
		if (SDA_read)
		{
			ReceiveByte |= 0x01;
		}
	}
	SCL_L;
	return ReceiveByte;
}

int SCCB_WriteByte(uint16_t WriteAddress, uint8_t SendByte)
{
	if (!SCCB_Start())
	{
		return DISABLE;
	}
	SCCB_SendByte(DEV_ADR);
	if (!SCCB_WaitAck())
	{
		SCCB_Stop();
		return DISABLE;
	}
	SCCB_SendByte((uint8_t)(WriteAddress & 0x00FF));
	SCCB_WaitAck();
	SCCB_SendByte(SendByte);
	SCCB_WaitAck();
	SCCB_Stop();
	return ENABLE;
}

int SCCB_ReadByte(uint8_t *pBuffer, uint16_t length, uint8_t ReadAddress)
{
	if (!SCCB_Start())
	{
		return DISABLE;
	}
	SCCB_SendByte(DEV_ADR);
	if (!SCCB_WaitAck())
	{
		SCCB_Stop();
		return DISABLE;
	}
	SCCB_SendByte(ReadAddress);
	SCCB_WaitAck();
	SCCB_Stop();

	if (!SCCB_Start())
	{
		return DISABLE;
	}
	SCCB_SendByte(DEV_ADR + 1);
	if (!SCCB_WaitAck())
	{
		SCCB_Stop();
		return DISABLE;
	}
	while (length)
	{
		*pBuffer = SCCB_ReceiveByte();
		if (length == 1)
		{
			SCCB_NoAck();
		}
		else
		{
			SCCB_Ack();
		}
		pBuffer++;
		length--;
	}
	SCCB_Stop();
	return ENABLE;
}
