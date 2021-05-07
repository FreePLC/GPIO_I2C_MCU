/*
* The Clear BSD License
* Copyright (c) 2019 Liang.Yang
* Copyright 2019-2019 Liang.Yang <WeChat:kala4tgo><Email:17389711@qq.com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted (subject to the limitations in the disclaimer below) provided
* that the following conditions are met:
*
* o Redistributions of source code must retain the above copyright notice, this list
*   of conditions and the following disclaimer.
*
* o Redistributions in binary form must reproduce the above copyright notice, this
*   list of conditions and the following disclaimer in the documentation and/or
*   other materials provided with the distribution.
*
* o Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from this
*   software without specific prior written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "I2C_Porting.h"
#include "I2C_UserConfig.h"

/*******************************************************************************
* Definitions
******************************************************************************/



/*******************************************************************************
* Prototypes
******************************************************************************/

static uint32_t I2C_GetACK(uint8_t port);
static void I2C_Start(uint8_t port);
static void I2C_Write(uint8_t dat, uint8_t port);
static uint32_t I2C_SetACK(uint8_t port);
static uint32_t I2C_SetNoACK(uint8_t port);
static uint8_t I2C_Read(uint8_t port);
static uint32_t I2C_GetACK(uint8_t port);
static void I2C_Stop(uint8_t port);

/*******************************************************************************
* Variables
******************************************************************************/

I2C_Port g_I2C_Port[I2C_NUMBER];

/*******************************************************************************
* Code
******************************************************************************/

/*****************************************************************************
** Function name:       GPIO_I2CWriteNByte
** Descriptions:        IIC读多字节
** input parameters:    regAddr:寄存器地址
                        regData:写入寄存器数据指针
                        num:数据长度
                        port: GPIO模拟的I2C模块号
** output parameters:
** Returned value:      TRUE or FALSE
******************************************************************************/

uint8_t GPIO_I2CWriteNByte(uint8_t regAddr,uint8_t *regData,uint8_t length,uint8_t port)
{
	uint8_t ack;
	g_I2C_Port[port].IRQ(0);
	I2C_Start(port);
	I2C_Write(g_I2C_Port[port].slave_addr|IIC_WRITE, port);
	ack = I2C_GetACK(port);
	if ( ack != IIC_ACK )
	{
		I2C_Stop(port);
		return FALSE;
	}
	I2C_Write(regAddr, port);
	ack = I2C_GetACK(port);
	if ( ack != IIC_ACK )
	{
		I2C_Stop(port);
		return FALSE;
	}
	while (length--)
	{
		I2C_Write(*regData, port);
		regData++;
		ack = I2C_GetACK(port);
		if ( ack != IIC_ACK )
		{
			I2C_Stop(port);
			return FALSE;
		}
	}
	I2C_Stop(port);
	g_I2C_Port[port].IRQ(1);
	return TRUE;
}

/*****************************************************************************
** Function name:       GPIO_I2CWriteByte
** Descriptions:        IIC读单字节
** input parameters:    regAddr:寄存器地址
                        regData:写入寄存器数据
                        port: GPIO模拟的I2C模块号
** output parameters:
** Returned value:      TRUE or FALSE
******************************************************************************/

uint8_t GPIO_I2CWriteByte(uint8_t regAddr,uint8_t regData, uint8_t port)
{
	uint8_t ack;
	g_I2C_Port[port].IRQ(0);
	I2C_Start(port);
	I2C_Write(g_I2C_Port[port].slave_addr|IIC_WRITE, port);
	ack = I2C_GetACK(port);
	if ( ack != IIC_ACK )
	{
		I2C_Stop(port);
		return FALSE;
	}
	I2C_Write(regAddr, port);
	ack = I2C_GetACK(port);
	if ( ack != IIC_ACK )
	{
		I2C_Stop(port);
		return FALSE;
	}

	I2C_Write(regData, port);
	regData++;
	ack = I2C_GetACK(port);
	if ( ack != IIC_ACK )
	{
		I2C_Stop(port);
		return FALSE;
	}
	I2C_Stop(port);
	g_I2C_Port[port].IRQ(1);
	return TRUE;
}

/*****************************************************************************
** Function name:       I2CReadNByte
** Descriptions:        IIC读多字节
** input parameters:    regAddr:寄存器地址
                        regData:寄存器数据
                        num:数据长度
                        port: GPIO模拟I2C模块号
** output parameters:
** Returned value:      TRUE or FALSE
*****************************************************************************/
uint8_t GPIO_I2CReadNByte(uint8_t regAddr,uint8_t *regData,uint8_t num, uint8_t port)
{
	uint8_t ack;
	g_I2C_Port[port].IRQ(0);
	I2C_Start(port);
	I2C_Write(g_I2C_Port[port].slave_addr|IIC_WRITE, port);
	ack = I2C_GetACK(port);
	if ( ack != IIC_ACK )
	{
		I2C_Stop(port);
		return FALSE;
	}

	I2C_Write(regAddr, port);
	ack = I2C_GetACK(port);
	if ( ack != IIC_ACK )
	{
		I2C_Stop(port);
		return FALSE;
	}
	//I2C_Stop();
	I2C_Start(port);
	I2C_Write(g_I2C_Port[port].slave_addr|IIC_READ, port);
	ack = I2C_GetACK(port);
	if ( ack != IIC_ACK )
	{
		I2C_Stop(port);
		return FALSE;
	}
	g_I2C_Port[port].Delay(25);

	while (num)
	{
		*regData++ =I2C_Read(port);
		if (num>1)
		{
			I2C_SetACK(port);
		}
		else
		{
			I2C_SetNoACK(port);
		}
		num--;
	}
	I2C_Stop(port);
	g_I2C_Port[port].IRQ(1);
	return TRUE;
}

/***********************************************************************
** Function name:       I2CReadByte
** Descriptions:        IIC读单字节
** input parameters:    regAddr:寄存器地址
                        regData:寄存器数据
                        port:GPIO模拟I2C模块号
** output parameters:
** Returned value:      TRUE or FALSE
*************************************************************************/
uint8_t GPIO_I2CReadByte(uint8_t regAddr,uint8_t *regData, uint8_t port)
{
	uint32_t ack;
	g_I2C_Port[port].IRQ(0);
	I2C_Start(port);
	I2C_Write(g_I2C_Port[port].slave_addr|IIC_WRITE, port);
	ack = I2C_GetACK(port);
	if ( ack != IIC_ACK )
	{
		I2C_Stop(port);
		return FALSE;
	}
	I2C_Write(regAddr, port);
	ack = I2C_GetACK(port);
	if ( ack != IIC_ACK )
	{
		I2C_Stop(port);
		return FALSE;
	}
	I2C_Stop(port);
	I2C_Start(port);
	I2C_Write(g_I2C_Port[port].slave_addr|IIC_READ,port);
	ack = I2C_GetACK(port);
	if ( ack != IIC_ACK )
	{
		I2C_Stop(port);
		return FALSE;
	}
	*regData =I2C_Read(port);
	I2C_SetNoACK(port);
	I2C_Stop(port);
	g_I2C_Port[port].IRQ(1);
	return TRUE;
}

void I2C_Recovery(uint8_t port)
{
	uint32_t num;

	g_I2C_Port[port].IIC_SDA_HIGH();
	g_I2C_Port[port].IIC_SCL_HIGH();
	g_I2C_Port[port].IIC_SDA_OUTPUT();
	g_I2C_Port[port].IIC_SCL_OUTPUT();
	g_I2C_Port[port].Delay(1);
	g_I2C_Port[port].IIC_SDA_INPUT();
	g_I2C_Port[port].Delay(1);

	num = 9;

	while (num--)
	{
		//总线已经释放
		if (g_I2C_Port[port].IIC_SDA_IO())
		{
			break;
		}
		//总线拉死状态
		else
		{
			g_I2C_Port[port].IIC_SCL_LOW();
			g_I2C_Port[port].Delay(1);
			g_I2C_Port[port].IIC_SCL_HIGH();
			g_I2C_Port[port].Delay(1);
		}
	}


	g_I2C_Port[port].IIC_SDA_HIGH();
	g_I2C_Port[port].IIC_SDA_OUTPUT();

	if (num < 8)
	{
		I2C_Stop(port);
	}


}


/*************************************************************
Static Function
*************************************************************/
static void I2C_Start(uint8_t port)
{
	uint32_t num;

	g_I2C_Port[port].IIC_SDA_OUTPUT();
	g_I2C_Port[port].IIC_SCL_OUTPUT();
	g_I2C_Port[port].IIC_SDA_HIGH();
	g_I2C_Port[port].IIC_SCL_HIGH();
	g_I2C_Port[port].Delay(1);
	num=2000;                                                           /* 判断IIC从机是否空闲          */
	while (num--)
	{
		if (g_I2C_Port[port].IIC_SCL_IO() )                          /* 根据IIC协议，时钟线拉高空闲 */
		{
			break;
		}
	}
	g_I2C_Port[port].Delay(1);
	g_I2C_Port[port].IIC_SDA_LOW();
	g_I2C_Port[port].Delay(2);
	g_I2C_Port[port].IIC_SCL_LOW();
	g_I2C_Port[port].Delay(1);
}

/*********************************************************************************************************
** Function name:       I2C_Write
** Descriptions:        IIC总线写字节
** input parameters:    需要写入的数据
** output parameters:
** Returned value:      void
*********************************************************************************************************/
static void I2C_Write(uint8_t dat, uint8_t port)
{
	uint8_t t = 8;
	uint32_t num;

	g_I2C_Port[port].IIC_SDA_LOW();
	g_I2C_Port[port].IIC_SDA_OUTPUT();
	g_I2C_Port[port].Delay(1);
	while (t--)
	{
		if (dat & 0x80)
		{
			g_I2C_Port[port].IIC_SDA_HIGH();
		}
		else
		{
			g_I2C_Port[port].IIC_SDA_LOW();
		}
		dat <<= 1;
		g_I2C_Port[port].Delay(1);
		g_I2C_Port[port].IIC_SCL_HIGH();
		g_I2C_Port[port].Delay(1);
		num=2000;                                                       /* 需要判断IIC时钟是否拉高      */
		while (num--)
		{
			if (g_I2C_Port[port].IIC_SCL_IO() )
			{
				break;
			}
		}
		g_I2C_Port[port].Delay(1);
		g_I2C_Port[port].IIC_SCL_LOW();
		g_I2C_Port[port].Delay(1);
	}
	g_I2C_Port[port].Delay(1);
}

/*********************************************************************************************************
** Function name:       I2C_SetACK
** Descriptions:        IIC总线应答
** input parameters:    void
** output parameters:
** Returned value:      void
*********************************************************************************************************/
static uint32_t I2C_SetACK(uint8_t port)
{
	uint32_t ack = 0;
	uint32_t num;

	g_I2C_Port[port].IIC_SCL_LOW();
	g_I2C_Port[port].Delay(1);
	g_I2C_Port[port].IIC_SDA_HIGH();
	g_I2C_Port[port].IIC_SDA_OUTPUT();
	g_I2C_Port[port].Delay(1);
	g_I2C_Port[port].IIC_SDA_LOW();
	g_I2C_Port[port].Delay(1);
	g_I2C_Port[port].IIC_SCL_HIGH();
	num=2000;
	while (num--)                                                       /* 需要判断IIC时钟是否拉高      */
	{
		if (g_I2C_Port[port].IIC_SCL_IO() )
		{
			break;
		}
	}
	g_I2C_Port[port].Delay(1);
	g_I2C_Port[port].IIC_SCL_LOW();
	g_I2C_Port[port].Delay(1);
	return ack;
}

/*********************************************************************************************************
** Function name:       I2C_SetNoACK
** Descriptions:        IIC总线不应答
** input parameters:    void
** output parameters:
** Returned value:      void
*********************************************************************************************************/
static uint32_t I2C_SetNoACK(uint8_t port)
{
	uint32_t ack,num = 0;

	g_I2C_Port[port].IIC_SCL_LOW();
	g_I2C_Port[port].Delay(1);
	g_I2C_Port[port].IIC_SDA_HIGH();
	g_I2C_Port[port].IIC_SDA_OUTPUT();
	g_I2C_Port[port].Delay(1);
	g_I2C_Port[port].IIC_SCL_HIGH();
	num=2000;
	while (num--)
	{
		if (g_I2C_Port[port].IIC_SCL_IO() )                                              /* 需要判断IIC时钟是否拉高      */
		{
			break;
		}
	}
	g_I2C_Port[port].Delay(1);
	g_I2C_Port[port].IIC_SCL_LOW();
	g_I2C_Port[port].Delay(1);
	return ack;
}

/*********************************************************************************************************
** Function name:       I2C_Read
** Descriptions:        IIC读取字节
** input parameters:    void
** output parameters:
** Returned value:      IIC读取的数据
*********************************************************************************************************/
static uint8_t I2C_Read(uint8_t port)
{
	uint8_t t = 8;
	uint8_t dat = 0;
	uint32_t num;
	g_I2C_Port[port].IIC_SCL_LOW();
	//IIC_SDA_HIGH();	//YANGLIANG
	g_I2C_Port[port].IIC_SDA_INPUT();
	while (t--)
	{
		g_I2C_Port[port].Delay(2);
		g_I2C_Port[port].IIC_SCL_HIGH();
		num = 2000;
		while (num--)                                                   /* 需要判断IIC时钟是否拉高      */
		{
			if (g_I2C_Port[port].IIC_SCL_IO())
				break;
		}
		g_I2C_Port[port].IIC_SCL_HIGH();
		g_I2C_Port[port].Delay(1);
		dat <<= 1;

		if ( g_I2C_Port[port].IIC_SDA_IO() )
		{
			dat++;
		}
		g_I2C_Port[port].IIC_SCL_LOW();
	}
	g_I2C_Port[port].Delay(1);
	return dat;
}


/*********************************************************************************************************
** Function name:       I2C_GetACK
** Descriptions:        IIC获取ACK应答
** input parameters:    void
** output parameters:
** Returned value:      是否应答 ACK=1为不应答 ACK=0为应答
*********************************************************************************************************/
static uint32_t I2C_GetACK(uint8_t port)
{
	uint32_t ack, timeout = 0;

	g_I2C_Port[port].IIC_SCL_LOW();

	g_I2C_Port[port].Delay(1);

	g_I2C_Port[port].IIC_SDA_HIGH();

	g_I2C_Port[port].IIC_SDA_INPUT();

	while (1)
	{
		if (g_I2C_Port[port].IIC_SDA_IO())
		{
			timeout++;
		}
		else
		{
			break;
		}

		if (timeout > TIMEOUT_NUMBER)
		{
			break;
		}
	}

	g_I2C_Port[port].IIC_SCL_HIGH();
	g_I2C_Port[port].Delay(1);
	if (timeout > TIMEOUT_NUMBER)
	{
		ack = 1;
	}
	else
	{
		ack = 0;
	}
	g_I2C_Port[port].Delay(1);
	g_I2C_Port[port].IIC_SCL_LOW();
	g_I2C_Port[port].IIC_SDA_LOW();
	g_I2C_Port[port].IIC_SDA_OUTPUT();
	g_I2C_Port[port].Delay(1);

	return ack;
}

/*********************************************************************************************************
** Function name:       I2C_Stop
** Descriptions:        IIC停止发送数据
** input parameters:    void
** output parameters:
** Returned value:      void
*********************************************************************************************************/
static void I2C_Stop(uint8_t port)
{
	uint32_t num;

	g_I2C_Port[port].IIC_SDA_LOW();
	g_I2C_Port[port].Delay(1);
	g_I2C_Port[port].IIC_SCL_LOW();
	g_I2C_Port[port].Delay(1);
	g_I2C_Port[port].IIC_SCL_HIGH();
	g_I2C_Port[port].Delay(1);
	num=2000;
	while (num--)
	{
		if (g_I2C_Port[port].IIC_SCL_IO() )
		{
			break;
		}
	}
	g_I2C_Port[port].IIC_SDA_HIGH();
	g_I2C_Port[port].Delay(1);
}




