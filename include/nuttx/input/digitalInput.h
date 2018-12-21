
#ifndef __INCLUDE_SEER_DIGITAL_INPUT_H
#define __INCLUDE_SEER_DIGITAL_INPUT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/compiler.h>

#include <nuttx/fs/ioctl.h>
#include <nuttx/semaphore.h>

#ifdef CONFIG_SEER_SYSTEM

#ifdef __cplusplus
extern "C" {
#endif

int digitalinput_register(FAR const char *devname);

#ifdef __cplusplus
}
#endif


#endif //SEER_SYSTEM

#endif //__INCLUDE_SEER_LEDS_HEART_H
