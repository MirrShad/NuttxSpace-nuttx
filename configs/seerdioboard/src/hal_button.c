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

void irq_button_init(int button_id,int(*pf)(int,bool))
{
  if(0 == button_id)
  {
    board_gpio_initialize(BUTTON_1);
    board_gpio_exti_init(BUTTON_1,pf);
  }

    
}
