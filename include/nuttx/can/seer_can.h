#ifndef SEER_CAN_H_
#define SEER_CAN_H_

#include <nuttx/config.h>
#include <stdint.h>
#include <stdbool.h>


#ifdef CONFIG_SEER_SYSTEM

#ifdef __cplusplus
extern "C" {
#endif

#define CAN_MAXDATALEN 8

begin_packed_struct struct can_hdr_s
{
  uint32_t     ch_id;         /* 11- or 29-bit ID (20- or 3-bits unused) */
  uint8_t      ch_dlc    : 4; /* 4-bit DLC */
  uint8_t      ch_rtr    : 1; /* RTR indication */
#ifdef CONFIG_CAN_ERRORS
  uint8_t      ch_error  : 1; /* 1=ch_id is an error report */
#endif
  uint8_t      ch_extid  : 1; /* Extended ID indication */
  uint8_t      ch_unused : 1; /* Unused */
} end_packed_struct;

begin_packed_struct struct can_msg_s
{
  struct can_hdr_s cm_hdr;                  /* The CAN header */
  uint8_t          cm_data[CAN_MAXDATALEN]; /* CAN message data (0-8 byte) */
} end_packed_struct;

int can_register(FAR const char *devname);

#ifdef __cplusplus
}
#endif

#endif //CONFIG_SEER_SYSTEM

#endif

