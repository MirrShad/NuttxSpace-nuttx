#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <debug.h>

#include <nuttx/board.h>
#include <nuttx/power/pm.h>
#include <arch/board/board.h>

#include "chip.h"
#include "up_arch.h"
#include "up_internal.h"

#include "seerdioboard1_2_6.h"
#include "hal_gpio.h"

void heart_led_init(void)
{
    board_gpio_initialize(HEART_LED_1_2_6);
}

void heart_led_on(void)
{
    board_gpio_high(HEART_LED_1_2_6);
}

void heart_led_off(void)
{
    board_gpio_low(HEART_LED_1_2_6);
}
