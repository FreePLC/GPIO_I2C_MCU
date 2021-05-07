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
#include "I2C.h"
#include "I2C_Porting.h"
#include "I2C_UserConfig.h"



/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


extern void Systick_DelayUS(uint32_t us);

/*******************************************************************************
 * Variables
 ******************************************************************************/

extern I2C_Port g_I2C_Port[I2C_NUMBER];


/*******************************************************************************
 * Code
 ******************************************************************************/


void i2c0_scl_high()
{
	I2C0_SCL_PORT->PSOR |=  I2C0_SCL;
}

void i2c0_scl_low()
{
	I2C0_SCL_PORT->PCOR |=  I2C0_SCL;
}

void i2c0_scl_output()
{
	I2C0_SCL_PORT->PDDR |=  I2C0_SCL;

}

void i2c0_scl_input()
{
	I2C0_SCL_PORT->PDDR &=  ~I2C0_SCL;
}

uint8_t i2c0_scl_io()
{
	uint8_t ret = 0;
	ret = (I2C0_SCL_PORT->PDIR & I2C0_SCL)>>I2C0_SCL_PIN;
	return ret;
}


void i2c0_sda_high()
{
	I2C0_SDA_PORT->PSOR |=  I2C0_SDA;
}

void i2c0_sda_low()
{
	I2C0_SDA_PORT->PCOR |=  I2C0_SDA;
}

void i2c0_sda_output()
{
	I2C0_SDA_PORT->PDDR |=  I2C0_SDA;
}

void i2c0_sda_input()
{
	I2C0_SDA_PORT->PDDR &=  ~I2C0_SDA;
}

uint8_t i2c0_sda_io()
{
	uint8_t ret = 0;
	ret = (I2C0_SDA_PORT->PDIR & I2C0_SDA)>>I2C0_SDA_PIN;
	return ret;
}




void i2c1_scl_high()
{
	I2C1_SCL_PORT->PSOR |=  I2C1_SCL;
}

void i2c1_scl_low()
{
	I2C1_SCL_PORT->PCOR |=  I2C1_SCL;
}

void i2c1_scl_output()
{
	I2C1_SCL_PORT->PDDR |=  I2C1_SCL;

}

void i2c1_scl_input()
{
	I2C1_SCL_PORT->PDDR &=  ~I2C1_SCL;
}

uint8_t i2c1_scl_io()
{
	return I2C1_SCL_PORT->PDIR & I2C1_SCL;
}


void i2c1_sda_high()
{
	I2C1_SDA_PORT->PSOR |=  I2C1_SDA;
}

void i2c1_sda_low()
{
	I2C1_SDA_PORT->PCOR |=  I2C1_SDA;
}

void i2c1_sda_output()
{
	I2C1_SDA_PORT->PDDR |=  I2C1_SDA;
}

void i2c1_sda_input()
{
	I2C1_SDA_PORT->PDDR &=  ~I2C1_SDA;
}

uint8_t i2c1_sda_io()
{
	return I2C1_SDA_PORT->PDIR & I2C1_SDA;
}

void IRQ_ENABLE(uint8_t en)
{
	if (en)
	{
		ENABLE_IRQ();
	}
	else
	{
		DISABLE_IRQ();
	}
}

void GPIO_I2C_InitPort(uint8_t port)
{
	if (port == 0)
	{
		g_I2C_Port[port].IIC_SCL_HIGH = i2c0_scl_high;
		g_I2C_Port[port].IIC_SCL_LOW = i2c0_scl_low;
		g_I2C_Port[port].IIC_SCL_OUTPUT = i2c0_scl_output;
		g_I2C_Port[port].IIC_SCL_INPUT = i2c0_scl_input;
		g_I2C_Port[port].IIC_SCL_IO = i2c0_scl_io;

		g_I2C_Port[port].IIC_SDA_HIGH = i2c0_sda_high;
		g_I2C_Port[port].IIC_SDA_LOW = i2c0_sda_low;
		g_I2C_Port[port].IIC_SDA_OUTPUT = i2c0_sda_output;
		g_I2C_Port[port].IIC_SDA_INPUT = i2c0_sda_input;
		g_I2C_Port[port].IIC_SDA_IO = i2c0_sda_io;

		g_I2C_Port[port].slave_addr = (I2C0_SLAVE_ADDR_7BIT << 1);

		//SENSOR I2C
		/* PORTE24 (pin 20) is configured as I2C0_SCL */
		PORT_SetPinMux(PORTE, 24U, kPORT_MuxAsGpio);
		/* PORTE25 (pin 21) is configured as I2C0_SDA */
		PORT_SetPinMux(PORTE, 25U, kPORT_MuxAsGpio);

	}
	else if (port == 1)
	{
		g_I2C_Port[port].IIC_SCL_HIGH = i2c1_scl_high;
		g_I2C_Port[port].IIC_SCL_LOW = i2c1_scl_low;
		g_I2C_Port[port].IIC_SCL_OUTPUT = i2c1_scl_output;
		g_I2C_Port[port].IIC_SCL_INPUT = i2c1_scl_input;
		g_I2C_Port[port].IIC_SCL_IO = i2c1_scl_io;

		g_I2C_Port[port].IIC_SDA_HIGH = i2c1_sda_high;
		g_I2C_Port[port].IIC_SDA_LOW = i2c1_sda_low;
		g_I2C_Port[port].IIC_SDA_OUTPUT = i2c1_sda_output;
		g_I2C_Port[port].IIC_SDA_INPUT = i2c1_sda_input;
		g_I2C_Port[port].IIC_SDA_IO = i2c1_sda_io;

		g_I2C_Port[port].slave_addr = (I2C1_SLAVE_ADDR_7BIT << 1);

	}

	g_I2C_Port[port].IRQ = IRQ_ENABLE;
	g_I2C_Port[port].IIC_INIT = GPIO_I2C_Init;
	g_I2C_Port[port].Delay = Systick_DelayUS;

	g_I2C_Port[port].IIC_INIT(port);



}


void GPIO_I2C_Init(uint8_t port)
{
	g_I2C_Port[port].IIC_SDA_HIGH();
	g_I2C_Port[port].IIC_SCL_HIGH();
	g_I2C_Port[port].IIC_SDA_OUTPUT();
	g_I2C_Port[port].IIC_SCL_OUTPUT();
	g_I2C_Port[port].Delay(1);
	I2C_Recovery(port);
}













