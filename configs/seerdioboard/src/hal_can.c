#include <arch/irq.h>
#include <nuttx/arch.h>
#include <queue.h>
#include <string.h>

#include <nuttx/can/seer_can.h>
#include <nuttx/irq.h>
#ifdef ERROR
#undef ERROR
#endif

#include "stm32f4xx_can.h"

struct can_msg_que_s
{
	struct dq_entry_s dq_entry;
	FAR struct can_msg_s msg;
};

static dq_queue_t can1_tx_q;
#define MAX_CAN_MSG_BUF 30
static int can1_tx_cnt;
static FAR struct can_msg_que_s* last_can_msg;
static FAR struct can_msg_que_s can1_tx_buf[MAX_CAN_MSG_BUF];

void InitCANGPIO(CAN_TypeDef* CANx)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	uint32_t RCC_AHB1Periph_GPIOx;
	uint8_t GPIO_PinSource_BASE;
	uint8_t GPIO_AF_CANx;
	GPIO_TypeDef *GPIOx;
	
	if(CAN2 == CANx)
	{
		RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOB;
		GPIOx = GPIOB;
		GPIO_PinSource_BASE = GPIO_PinSource12;
	}	
	else if(CAN1 == CANx)
	{
		RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOA;
		GPIOx = GPIOA;
		GPIO_PinSource_BASE = GPIO_PinSource11;
	}
	//else seer_assert(false); //undefined!
	
	if(CANx == CAN1)	GPIO_AF_CANx = GPIO_AF_CAN1;
	else if(CANx == CAN2)	GPIO_AF_CANx = GPIO_AF_CAN2;
	//else seer_assert(false); //undefined!
	
	/* open clock of MOSI MISO SCK nCS */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx, ENABLE);
	
	/* Config Pin: MOSI MISO SCK */
	GPIO_PinAFConfig(GPIOx, GPIO_PinSource_BASE, GPIO_AF_CANx);
	GPIO_PinAFConfig(GPIOx, GPIO_PinSource_BASE + 1, GPIO_AF_CANx);	
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0|GPIO_Pin_1) << GPIO_PinSource_BASE;
	GPIO_Init(GPIOx, &GPIO_InitStructure);

	dq_init(&can1_tx_q);
	can1_tx_cnt = 0;
	int i = 0;
	last_can_msg = can1_tx_buf;
	for(i = 0; i<MAX_CAN_MSG_BUF ; i++)
	{
		dq_addlast(&(can1_tx_buf[i].dq_entry),&can1_tx_q);
	}
}

void transmitCANmsg(int can_x)//,FAR struct can_msg_s *uppermsg)
{
	FAR struct can_msg_que_s* mpb = (FAR struct can_msg_que_s*)dq_remfirst(&can1_tx_q);
	FAR struct can_msg_s *uppermsg = &(mpb->msg);
	dq_addfirst(&mpb->dq_entry,&can1_tx_q);
	CAN_TypeDef* CANx;
	if(1==can_x)
		CANx = CAN1;
	else if(2==can_x)
		CANx = CAN2;
	CanTxMsg msg;
	msg.StdId = uppermsg->cm_hdr.ch_id;
	msg.ExtId = 0;

	if(1 == uppermsg->cm_hdr.ch_rtr)
		msg.RTR = CAN_RTR_Data;
	else
		msg.RTR = CAN_RTR_Remote;

	if(1 == uppermsg->cm_hdr.ch_extid)	
		msg.IDE = CAN_Id_Standard;
	else
		msg.IDE = CAN_Id_Extended;
	
	msg.DLC = uppermsg->cm_hdr.ch_dlc;
	int i = 0;
	for(i=0;i<msg.DLC;i++)
	{msg.Data[i] = uppermsg->cm_data[i];}

	uint8_t temp_mbox = CAN_Transmit(CANx, &msg);
	while(temp_mbox != CAN_TxStatus_NoMailBox)
	{
		//syslog("no mail box\r\n");
		temp_mbox = CAN_Transmit(CANx, &msg);
	}
}

int CAN_RxIRQhandler(int irq, FAR void *context, FAR void *arg)
{
	CanRxMsg RxMessage;
	while(CAN_MessagePending(CAN1, CAN_Filter_FIFO0) != 0)
	{
		CAN_Receive(CAN1, CAN_Filter_FIFO0, &RxMessage);
	}
	CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
}

int CAN_TxIRQhandler(int irq, FAR void *context, FAR void *arg)
{
	FAR struct can_msg_que_s* mpb = (FAR struct can_msg_que_s*)dq_remfirst(&can1_tx_q);
	mpb->msg.cm_hdr.ch_id = 0x555;//this is an error code, when we see this, this means an error
	dq_addlast(&mpb->dq_entry,&can1_tx_q);
	can1_tx_cnt--;
	if(can1_tx_cnt!=0)
		transmitCANmsg(1);
	CAN_ClearITPendingBit(CAN1, CAN_IT_TME);
}

void InitCANIRQ(CAN_TypeDef* CANx)
{
	if(CANx == CAN1) 
	{
		irq_attach(STM32_IRQ_CAN1RX0,CAN_RxIRQhandler,(void*)0);
		up_enable_irq(STM32_IRQ_CAN1RX0); 
		CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
	}/*else if(CANx == CAN2) 
	{
		irq_attach(STM32_IRQ_CAN2RX1,CAN_IRQRxhandler,(void*)0);
		up_enable_irq(STM32_IRQ_CAN2RX1); 
		CAN_ITConfig(CAN2, CAN_IT_FMP1, ENABLE);
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP1);
	}*/

	//TX
	if(CANx == CAN1) 
	{
		irq_attach(STM32_IRQ_CAN1TX,CAN_TxIRQhandler,(void*)0);
		up_enable_irq(STM32_IRQ_CAN1TX); 
		CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
		CAN_ClearITPendingBit(CAN1, CAN_IT_TME);
	}

}

void InitCAN(CAN_TypeDef* CANx)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//CAN1 must be open when CAN2 used
	if(CANx == CAN2) 
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
	}else if(CANx == CAN1)
	{}
	
	//CAN
	CAN_InitTypeDef CAN_InitStructure;
	CAN_InitStructure.CAN_TTCM = DISABLE;	
	CAN_InitStructure.CAN_ABOM = ENABLE;	
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = ENABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	

	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;		
	
	CAN_InitStructure.CAN_Prescaler = 12;
	CAN_Init(CANx, &CAN_InitStructure);
	
	//config the filter
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask; //mask mode
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;//32bits
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	
	uint8_t CAN_Filter_FIFO;
	if(CAN1 == CANx)
	{
		CAN_Filter_FIFO  = CAN_Filter_FIFO0;
		CAN_FilterInitStructure.CAN_FilterNumber = 0;	  //must < 14
	}
	else if(CAN2 == CANx)
	{
		CAN_Filter_FIFO  = CAN_Filter_FIFO1;
		CAN_FilterInitStructure.CAN_FilterNumber = 15;	  //must >= 14
	}
	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x00<<5);								
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_RTR_DATA|CAN_ID_STD;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO;
	CAN_FilterInit(&CAN_FilterInitStructure);
	int i = 0;
	for(i = 0; i < 3; i++)
		CAN_FIFORelease(CANx, CAN_Filter_FIFO);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_5);
	GPIO_SetBits(GPIOA, GPIO_Pin_6);
}

void CANInit()
{
  	InitCANGPIO(CAN1);
	InitCAN(CAN1);
	//InitCANIRQ(CAN1);
/*
	FAR struct can_msg_s testMsg;
	testMsg.cm_hdr.ch_id = 0x666;
	testMsg.cm_hdr.ch_rtr = 1;
	testMsg.cm_hdr.ch_extid = 1;
	testMsg.cm_hdr.ch_dlc = 4;
	testMsg.cm_data[0] = 0x11;
	testMsg.cm_data[1] = 0x22;
	testMsg.cm_data[2] = 0x33;
	testMsg.cm_data[3] = 0x44;
	sendCANMsg(1,&testMsg);
*/
}

void anothertransmitCANmsg(int can_x ,FAR struct can_msg_s uppermsg)
{
	CAN_TypeDef* CANx;
	if(1==can_x)
		CANx = CAN1;
	else if(2==can_x)
		CANx = CAN2;
	CanTxMsg msg;
	msg.StdId = uppermsg.cm_hdr.ch_id;
	msg.ExtId = 0;

	if(1 == uppermsg.cm_hdr.ch_rtr)
		msg.RTR = CAN_RTR_Data;
	else
		msg.RTR = CAN_RTR_Remote;

	if(1 == uppermsg.cm_hdr.ch_extid)	
		msg.IDE = CAN_Id_Standard;
	else
		msg.IDE = CAN_Id_Extended;
	
	msg.DLC = uppermsg.cm_hdr.ch_dlc;
	int i = 0;
	for(i=0;i<msg.DLC;i++)
	{msg.Data[i] = uppermsg.cm_data[i];}

	//syslog("send id 0x%x",msg.StdId);
	uint8_t temp_mbox = CAN_Transmit(CANx, &msg);
	while(temp_mbox == CAN_TxStatus_NoMailBox)
	{
		syslog(LOG_DEBUG,"no mail box\r\n");
		temp_mbox = CAN_Transmit(CANx, &msg);
	}
}

int sendCANMsg(int can_x,FAR struct can_msg_s *uppermsg)
{
	if(can1_tx_cnt == MAX_CAN_MSG_BUF) return 1;
	irqstate_t flags = enter_critical_section();

	//FAR struct can_msg_que_s* mpb = (FAR struct can_msg_que_s*)dq_remlast(&can1_tx_q);
	//mpb->msg = *uppermsg;
	//dq_addafter(&last_can_msg->dq_entry,mpb,&can1_tx_q);
	//can1_tx_cnt++;
	//last_can_msg = mpb;
//if queue==1,means we have to start a sending
	//if(can1_tx_cnt==1)
	{
		struct can_msg_s msg = *uppermsg;
		anothertransmitCANmsg(can_x,msg);
	}

	leave_critical_section(flags);
	return 0;
}

