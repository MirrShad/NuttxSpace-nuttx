#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <debug.h>

#include <nuttx/board.h>
#include <arch/board/board.h>

#include "stm32f4xx.h"
#include "hal_gpio.h"

#include <arch/irq.h>
#include <nuttx/arch.h>

int(*buttonUpperInt)(int,bool);

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

    //should not reach here
    return GPIOA;
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

bool haveEXTI(uint32_t cfgset)
{
    if(getMode(cfgset) != GPIO_Mode_IN)
      return false;
    return (cfgset & GPIO_EXTI) > 0;
}

void board_gpio_initialize(uint32_t cfgset)
{
    uint32_t RCC_AHB1Periphx;
	GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_TypeDef* port = getPort(cfgset);

    RCC_AHB1Periphx	= RCC_AHB1Periph_GPIOA<< (((uint32_t)port-(uint32_t)GPIOA)/0x400);
    //RCC_AHB1PeriphClockCmd(RCC_AHB1Periphx, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin = getPin(cfgset);
    GPIO_InitStructure.GPIO_Speed = getSpeed(cfgset); 
    GPIO_InitStructure.GPIO_Mode = getMode(cfgset);
    GPIO_InitStructure.GPIO_PuPd = getPuPd(cfgset);
//    GPIO_InitStructure.GPIO_OType = getOType(cfgset);
    GPIO_Init(port, &GPIO_InitStructure);

    //if(haveEXTI(cfgset))
     //board_gpio_exti_init(cfgset); 
}

int buttonIRQ(int irq, FAR void *context, FAR void *arg)
{
    uint16_t temp = GPIO_ReadInputData(GPIOD);
    syslog(LOG_DEBUG,"In button IRQ %d\r\n",(temp>>10)&1);
    int buttonid = 0;
    bool isPressed = (temp>>10)&1;
    (*buttonUpperInt)(buttonid,isPressed);
    EXTI_ClearITPendingBit(EXTI_Line10);
}

void board_gpio_exti_init(uint32_t cfgset,int(*pf)(int,bool))
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
    uint8_t EXTI_PortSourceX = ((uint8_t)getPort(cfgset) - AHB1PERIPH_BASE)/0x0400;
    //print("PortSource is %d",EXTI_PortSourceX);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD,EXTI_PinSource10);
    
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line10;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);    

    irq_attach(STM32_IRQ_EXTI1510,buttonIRQ,(void *)0);
    up_enable_irq(STM32_IRQ_EXTI1510);

    buttonUpperInt = pf;
}

void board_gpio_high(uint32_t cfgset)
{
    GPIO_SetBits(getPort(cfgset),getPin(cfgset));
}

void board_gpio_low(uint32_t cfgset)
{
    GPIO_ResetBits(getPort(cfgset),getPin(cfgset));
}
