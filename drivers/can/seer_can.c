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
#include <nuttx/kmalloc.h>
#include <nuttx/wqueue.h>
#include <nuttx/fs/fs.h>

#include <nuttx/semaphore.h>
#include <nuttx/can/seer_can.h>
#include <nuttx/board.h>

///bool bInited = false;
sem_t can_readSem;

struct can_config_s
{
    sem_t bu_exclsem;/* Supports exclusive access to the device */
    int id;
};

static void can_enable(FAR struct can_config_s *priv)
{
  irqstate_t flags;
  
  /* This routine is called both task level and interrupt level, so
   * interrupts must be disabled.
   */

  flags = enter_critical_section();
  //if(!bInited)
    CANInit();
  
  leave_critical_section(flags);
  //bInited = true;
}

static int can_open(FAR struct file *filep)
{
  FAR struct inode *inode;
  FAR struct can_config_s *priv;
  
  DEBUGASSERT(filep && filep->f_inode);
  inode = filep->f_inode;
  DEBUGASSERT(inode->i_private);
  priv = (FAR struct can_config_s *)inode->i_private;

  can_enable(priv);
  /* Get exclusive access to the driver structure */
  int ret = 0;
/*
  ret = digitalinput_takesem(&priv->bu_exclsem);
  if (ret < 0)
    {
      ierr("ERROR: btn_takesem failed: %d\n", ret);
      return ret;
    }
      
  digitalinput_enable(priv);

errout_with_sem:
  digitalinput_givesem(&priv->bu_exclsem);
*/
  return ret;
}

static int canread_interrupt()
{
  int value;
	int status = sem_getvalue(&can_readSem, &value);
	if(status<0){
		printf("Error: chassis new speed cmd can not get semophore value\r\n");
	}
	status = sem_post(&can_readSem);
	if(status!=0){
		printf("Error: chassis new speed cmd can not post semophore value\r\n");
	}
  return 0;
}

static ssize_t can_read(FAR struct file *filep, FAR char *buffer,
                        size_t buflen)
{
  int value;
	int status = sem_getvalue(&can_readSem, &value);
	if(status<0){
		printf("Error: canread_interrupt can not get semophore value\r\n");
    return -1*errno;
	}
  //bug!!!!! just for emergency ignore other interrupt here
  do{
    status = sem_wait(&can_readSem);
    if(errno==EINTR && status!=0){
      ;//printf("temparory Ignore other interrupt\r\n");
    }else if(status!=0){
		  printf("Error: canread_interrupt can not wait semophore value %d\r\n",errno);
      return -1;
    }
    else break;
  }while(true);

  CAN_getMsg((FAR struct can_msg_s*)buffer);
  //syslog(LOG_DEBUG,"get msg 0x%x\r\n",((FAR struct can_msg_s*)buffer)->cm_hdr.ch_id);
  return 0;
}

static ssize_t can_write(FAR struct file *filep,FAR const char *buffer, size_t buflen)
{
  FAR struct can_msg_s *msg = (FAR struct can_msg_s *)buffer;
  sendCANMsg(1,msg);
  return 8;
}

static int can_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
  return 0;
}

static const struct file_operations can_fops =
{
  can_open,  /* open */
  NULL, /* close */
  can_read,  /* read */
  can_write,      /* write */
  NULL,      /* seek */
  can_ioctl  /* ioctl */
#ifndef CONFIG_DISABLE_POLL
  , NULL /* poll */
#endif
};

int can_register(FAR const char *devname)
{
  FAR struct can_config_s *priv;
  int ret;

  DEBUGASSERT(devname);

  /* Allocate a new button driver instance */

  priv = (FAR struct can_config_s *)
    kmm_zalloc(sizeof(struct can_config_s));

  if (!priv)
    {
      ierr("ERROR: Failed to allocate device structure\n");
      return -ENOMEM;
    }

  nxsem_init(&priv->bu_exclsem, 0, 1);
  //memset(priv->inputs,MAX_INPUT_NUM*sizeof(char),1);
  /* And register the button driver */

  ret = register_driver(devname, &can_fops, 0666, priv);
  if (ret < 0)
    {
      ierr("ERROR: register_driver failed: %d\n", ret);
      goto errout_with_priv;
    }

  int value;
	int status = sem_getvalue(&can_readSem, &value);
  if(status<0){
		printf("Error: can_register can not get semophore value\r\n");
	}

  irq_can_init(canread_interrupt);

  return OK;

errout_with_priv:
  nxsem_destroy(&priv->bu_exclsem);
  kmm_free(priv);
  return ret;
}

