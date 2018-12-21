#include <nuttx/config.h>

#include <sys/types.h>

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <poll.h>
#include <errno.h>
#include <assert.h>
#include <debug.h>

#include <nuttx/irq.h>
#include <nuttx/arch.h>
#include <nuttx/wdog.h>
#include <nuttx/kmalloc.h>
#include <nuttx/wqueue.h>
#include <nuttx/fs/fs.h>
#include <nuttx/spi/spi.h>
#include <nuttx/random.h>

#include <nuttx/semaphore.h>
#include <nuttx/input/digitalInput.h>

const int MAX_INPUT_NUM = 16;

struct digitalinput_config_s
{
    //bool inputs[MAX_INPUT_NUM];
    sem_t bu_exclsem;/* Supports exclusive access to the device */
    int (*pInt)(FAR const struct digitalinput_a *input);
};

struct digitalinput_a
{
    int id;
    bool bHigh;
    //todo:: timestamp
};


//static digitalinput_config_s digitalinputs;

/* Semaphore helpers */
static inline int btn_takesem(sem_t *sem);
#define digitalinput_givesem(s) nxsem_post(s);

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: btn_takesem
 ****************************************************************************/

static int digitalinput_interrupt(int button_id, bool bTriggered)
{
    DEBUGASSERT(input);
    syslog(LOG_DEBUG,"buttonId %d is Triggerd: %d\r\n",button_id,bTriggered);
}

static inline int digitalinput_takesem(sem_t *sem)
{
  int ret;

  /* Take a count from the semaphore, possibly waiting */

  ret = nxsem_wait(sem);

  /* The only case that an error should occur here is if the wait
   * was awakened by a signal
   */

  DEBUGASSERT(ret == OK || ret == -EINTR);
  return ret;
}

static void digitalinput_enable(FAR struct digitalinput_config_s *priv)
{
  irqstate_t flags;
  
  /* This routine is called both task level and interrupt level, so
   * interrupts must be disabled.
   */

  flags = enter_critical_section();
  (void)irq_button_init(0,digitalinput_interrupt);
  
  leave_critical_section(flags);
}

static int digitalinput_open(FAR struct file *filep)
{
  FAR struct inode *inode;
  FAR struct digitalinput_config_s *priv;
  
  DEBUGASSERT(filep && filep->f_inode);
  inode = filep->f_inode;
  DEBUGASSERT(inode->i_private);
  priv = (FAR struct digitalinput_config_s *)inode->i_private;

  /* Get exclusive access to the driver structure */

  int ret = digitalinput_takesem(&priv->bu_exclsem);
  if (ret < 0)
    {
      ierr("ERROR: btn_takesem failed: %d\n", ret);
      return ret;
    }
      
  digitalinput_enable(priv);

errout_with_sem:
  digitalinput_givesem(&priv->bu_exclsem);
  return ret;
}

static ssize_t digitalinput_read(FAR struct file *filep, FAR char *buffer,
                        size_t buflen)
{
    return OK;
}

static const struct file_operations digitalinput_fops =
{
  digitalinput_open,  /* open */
  NULL, /* close */
  digitalinput_read,  /* read */
  NULL,      /* write */
  NULL,      /* seek */
  NULL  /* ioctl */
#ifndef CONFIG_DISABLE_POLL
  , NULL /* poll */
#endif
};

int digitalinput_register(FAR const char *devname)
{
  FAR struct digitalinput_config_s *priv;
  int ret;

  DEBUGASSERT(devname);

  /* Allocate a new button driver instance */

  priv = (FAR struct digitalinput_config_s *)
    kmm_zalloc(sizeof(struct digitalinput_config_s));

  if (!priv)
    {
      ierr("ERROR: Failed to allocate device structure\n");
      return -ENOMEM;
    }

  nxsem_init(&priv->bu_exclsem, 0, 1);
  //memset(priv->inputs,MAX_INPUT_NUM*sizeof(char),1);
  /* And register the button driver */

  ret = register_driver(devname, &digitalinput_fops, 0666, priv);
  if (ret < 0)
    {
      ierr("ERROR: register_driver failed: %d\n", ret);
      goto errout_with_priv;
    }

  return OK;

errout_with_priv:
  nxsem_destroy(&priv->bu_exclsem);
  kmm_free(priv);
  return ret;
}
