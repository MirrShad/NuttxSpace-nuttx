
#ifndef __INCLUDE_SEER_LEDS_HEART_H
#define __INCLUDE_SEER_LEDS_HEART_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/compiler.h>

#include <nuttx/fs/ioctl.h>

#ifdef CONFIG_SEER_SYSTEM

#ifdef __cplusplus
extern "C" {
#endif

#define HEARTLEDIOC_SETON _ULEDIOC(1)
#define HEARTLEDIOC_SETOFF _ULEDIOC(2)


int heartled_register(FAR const char *path);

#ifdef __cplusplus
}
#endif


#endif //SEER_SYSTEM

#endif //__INCLUDE_SEER_LEDS_HEART_H
