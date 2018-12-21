#include <nuttx/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/kmalloc.h>
#include <nuttx/fs/fs.h>
#include <nuttx/arch.h>
#include <nuttx/leds/heartled.h>

#include <arch/irq.h>
#include <nuttx/board.h>

#ifdef CONFIG_SEER_SYSTEM

static int heartled_open(FAR struct file *filep)
{
    return 0;
}

static int heartled_ioctl(FAR struct file *filep, int cmd,unsigned long arg)
{
    int ret = OK;
    switch (cmd)
    {
        case HEARTLEDIOC_SETON:
            heart_led_on();
            break;
        case HEARTLEDIOC_SETOFF:
            heart_led_off();
            break;
        default:
            return !OK;
        }
        return ret;
}

static ssize_t heartled_write(FAR struct file *filep, FAR const char *buffer,
                             size_t len)
{
    return OK;
}

static const struct file_operations g_heartledops =
{
  heartled_open,  /* open */
  0, /* close */
  0,  /* read */
  heartled_write, /* write */
  0,            /* seek */
  heartled_ioctl             /* ioctl */
#ifndef CONFIG_DISABLE_POLL
  , 0           /* poll */
#endif
#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS
  , 0           /* unlink */
#endif
};

int heartled_register(FAR const char *path)
{
  heart_led_init();
  return register_driver(path, &g_heartledops, 0666, NULL);
}

#endif
