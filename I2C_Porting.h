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

#ifndef __I2C_PORTING_H
#define __I2C_PORTING_H

#include <stdint.h>
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_common.h"


#define I2C0_SDA_PORT	   GPIOE
#define I2C0_SCL_PORT	   GPIOE

#define I2C0_SCL_PIN		 24
#define I2C0_SDA_PIN       25

#define I2C0_SCL           (1<<I2C0_SCL_PIN)
#define I2C0_SDA           (1<<I2C0_SDA_PIN)


#define I2C1_SDA_PORT	   GPIOC
#define I2C1_SCL_PORT	   GPIOC

#define I2C1_SCL_PIN		 1
#define I2C1_SDA_PIN       2

#define I2C1_SCL           (1<<I2C1_SCL_PIN)
#define I2C1_SDA           (1<<I2C1_SDA_PIN)

#define DISABLE_IRQ()       __set_PRIMASK(1);
#define ENABLE_IRQ()        __set_PRIMASK(0); 


#define IIC_WRITE               0x00
#define IIC_READ                0x01
#define IIC_ACK                  0

#ifndef TRUE
#define    TRUE    1
#endif

#ifndef FALSE
    #define FALSE    0
#endif


typedef struct _i2c_port
{
  uint8_t slave_addr;
  
  void	(*IIC_SDA_OUTPUT)(void);
  void	(*IIC_SDA_HIGH)(void);
  void	(*IIC_SDA_LOW)(void);
  void	(*IIC_SDA_INPUT)(void);
  uint8_t	(*IIC_SDA_IO)(void);
  
  void	(*IIC_SCL_OUTPUT)(void);
  void	(*IIC_SCL_HIGH)(void);
  void	(*IIC_SCL_LOW)(void);
  void	(*IIC_SCL_INPUT)(void);
  uint8_t	(*IIC_SCL_IO)(void);

  void	(*Delay)(uint32_t us);

  void	(*IRQ)(uint8_t en);

  void	(*IIC_INIT)(uint8_t port);
  
}I2C_Port;














void GPIO_I2C_InitPort(uint8_t port);
void GPIO_I2C_Init(uint8_t port);












#endif

