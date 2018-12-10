#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <debug.h>

#include <nuttx/board.h>
#include <arch/board/board.h>

#include "stm32f4xx.h"
#include "hal_gpio.h"

GPIO_TypeDef* getPort(uint32_t cfgset)
{
    int port =  (cfgset & GPIO_PORT_MASK) >> GPIO_PORT_SHIFT;
    if(port == 0)
        return GPIOA;
    else if(port == 1)
        return GPIOB;
    else if(port == 2)
        return GPIOC;
    else if(port == 3)
        return GPIOD;
    else if(port == 4)
        return GPIOE;
    else if(port == 5)
        return GPIOF;
    else if(port == 6)
        return GPIOG;
    else if(port == 7)
        return GPIOH;
    else if(port == 8)
        return GPIOI;
}

uint16_t getPin(uint32_t cfgset)
{
    int pin = (cfgset & GPIO_PIN_MASK) >> GPIO_PIN_SHIFT;
    return GPIO_Pin_0 + (1 << pin );
}

GPIOSpeed_TypeDef getSpeed(uint32_t cfgset)
{
    int speed = (cfgset & GPIO_SPEED_MASK) >> GPIO_SPEED_SHIFT;
    return (GPIOSpeed_TypeDef)speed;
}

GPIOMode_TypeDef getMode(uint32_t cfgset)
{
    int mode = (cfgset & GPIO_MODE_MASK) >> GPIO_MODE_SHIFT;
    return (GPIOMode_TypeDef)mode;
}

GPIOOType_TypeDef getOType(uint32_t cfgset)
{
    if((cfgset & GPIO_OPENDRAIN) != 0)
        return GPIO_OType_OD;
    else
        return GPIO_OType_PP;
}

GPIOPuPd_TypeDef getPuPd(uint32_t cfgset)
{
    int pupd = (cfgset & GPIO_PUPD_MASK) >> GPIO_PUPD_SHIFT;
    return (GPIOPuPd_TypeDef)pupd;
}

void board_gpio_initialize(uint32_t cfgset)
{
    uint32_t RCC_AHB1Periphx;
	GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_TypeDef* port = getPort(cfgset);

    /* DI config */
	RCC_AHB1Periphx	= RCC_AHB1Periph_GPIOA<< (((uint32_t)port-(uint32_t)GPIOA)/0x400);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periphx, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = getPin(cfgset);
	GPIO_InitStructure.GPIO_Speed = getSpeed(cfgset); 
	GPIO_InitStructure.GPIO_Mode = getMode(cfgset);
	GPIO_InitStructure.GPIO_PuPd = getPuPd(cfgset);
    GPIO_InitStructure.GPIO_OType = getOType(cfgset);
    GPIO_InitStructure.GPIO_PuPd = getPuPd(cfgset);
    GPIO_Init(port, &GPIO_InitStructure);
}

void board_gpio_high(uint32_t cfgset)
{
    GPIO_SetBits(getPort(cfgset),getPin(cfgset));
}

void board_gpio_low(uint32_t cfgset)
{
    GPIO_ResetBits(getPort(cfgset),getPin(cfgset));
}
