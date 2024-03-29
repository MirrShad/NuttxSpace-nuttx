/************************************************************************************
 * configs/metro-m4/include/board.h
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ************************************************************************************/

#ifndef __CONFIG_METRO_M4_INCLUDE_BOARD_H
#define __CONFIG_METRO_M4_INCLUDE_BOARD_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <nuttx/config.h>

#ifndef __ASSEMBLY__
#  include <stdint.h>
#  include <stdbool.h>
#endif

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* Clocking *************************************************************************/
/* Overview
 *
 * The Adafruit Metro M4 Pro has one on-board crystal:
 *
 *   X4 32.768KHz XOSC32
 *
 * Since there is no high speed crystal, we will run from the OSC16M clock source.
 *
 * OSC48M               Output     = 48Mhz
 *  |
 * FDLL                 Input      = 48MHz
 *  |                   Output     = 48MHz
 * GCLK5                Input      = 48MHz
 *  |                   Output     = 2MHz
 * DPLL0                Input      = 2MHz
 *  |                   Output     = 120MHz
 * GCLK0                Input      = 120MHz
 *  |                   Output     = 120MHz
 * MCK                  Input      = 120MHz
 *  |                   Output     = 120MHz
 * CPU                  Input      = 120MHz
 */

#define BOARD_XOSC32K_FREQUENCY 32768     /* XOSC32K frequency 32.768 KHz */
#define BOARD_DFLL_FREQUENCY    48000000  /* FDLL frequency 28MHz */
#define BOARD_XOSC0_FREQUENCY   12000000  /* XOSC0 frequency 12MHz (disabled) */
#define BOARD_XOSC1_FREQUENCY   12000000  /* XOSC0 frequency 12MHz (disabled)*/
#define BOARD_DPLL0_FREQUENCY   120000000 /* DPLL0 output frueuency (120MHz) */
#define BOARD_DPLL1_FREQUENCY   47985664  /* DPLL1 output frequency (disabled) */

#define BOARD_GCLK0_FREQUENCY   BOARD_DPLL0_FREQUENCY
#define BOARD_GCLK1_FREQUENCY   BOARD_DFLL_FREQUENCY
#define BOARD_GCLK2_FREQUENCY   (BOARD_XOSC32K_FREQUENCY / 4)  /* Disabled */
#define BOARD_GCLK3_FREQUENCY   BOARD_XOSC32K_FREQUENCY        /* Disabled */
#define BOARD_GCLK4_FREQUENCY   BOARD_DPLL0_FREQUENCY
#define BOARD_GCLK5_FREQUENCY   (BOARD_DFLL_FREQUENCY / 24)
#define BOARD_GCLK6_FREQUENCY   BOARD_XOSC1_FREQUENCY          /* Disabled */
#define BOARD_GCLK7_FREQUENCY   BOARD_XOSC1_FREQUENCY          /* Disabled */
#define BOARD_GCLK8_FREQUENCY   BOARD_XOSC1_FREQUENCY          /* Disabled */
#define BOARD_GCLK0_FREQUENCY   BOARD_XOSC1_FREQUENCY          /* Disabled */
#define BOARD_GCLK11_FREQUENCY  BOARD_XOSC1_FREQUENCY          /* Disabled */

#define BOARD_CPU_FREQUENCY     BOARD_GCLK0_FREQUENCY /* CPU frequency 120MHz */

/* XOSC32 */

#define BOARD_HAVE_XOSC32K      1         /* 32.768KHz XOSC32 crystal installed */
#define BOARD_XOSC32K_ENABLE    false     /* Don't enable XOSC32 */
#define BOARD_XOSC32K_XTALEN    false     /* External clock connected on XIN32 */
#define BOARD_XOSC32K_EN32K     false     /* No 32KHz output */
#define BOARD_XOSC32K_EN1K      false     /* No 1KHz output */
#define BOARD_XOSC32K_HIGHSPEED true      /* High speed mode */
#define BOARD_XOSC32K_RUNSTDBY  false     /* Don't run in standby */
#define BOARD_XOSC32K_ONDEMAND  true      /* Enable on-demand control */
#define BOARD_XOSC32K_CFDEN     false     /* Clock failure detector not enabled */
#define BOARD_XOSC32K_CFDEO     false     /* No clock failure event */
#define BOARD_XOSC32K_CALIBEN   false     /* No OSCULP32K calibration */
#define BOARD_XOSC32K_STARTUP   0         /* Startup time: 62592us */
#define BOARD_XOSC32K_CALIB     0         /* Dummy OSCULP32K calibration value */
#define BOARD_XOSC32K_RTCSEL    0         /* RTC clock = ULP1K */

/* XOSC0 */

#define BOARD_HAVE_XOSC0        0         /* No XOSC0 clock/crystal installed */
#define BOARD_XOSC0_ENABLE      false     /* Don't enable XOSC0 */
#define BOARD_XOSC0_XTALEN      false     /* External clock connected */
#define BOARD_XOSC0_RUNSTDBY    false     /* Don't run in standby */
#define BOARD_XOSC0_ONDEMAND    true      /* Disable on-demand control */
#define BOARD_XOSC0_LOWGAIN     false     /* Disable low buffer gain */
#define BOARD_XOSC0_ENALC       false     /* Disable automatic loop control */
#define BOARD_XOSC0_CFDEN       false     /* Clock failure detector not enabled */
#define BOARD_XOSC0_SWBEN       false     /* XOSC clock switch not enabled */
#define BOARD_XOSC0_STARTUP     0         /* XOSC0 start-up time 31µs */

/* XOSC1 */

#define BOARD_HAVE_XOSC1        0         /* No XOSC0 clock/crystal installed */
#define BOARD_XOSC1_ENABLE      false     /* Don't enable XOSC1 */
#define BOARD_XOSC1_XTALEN      true      /* External crystal connected */
#define BOARD_XOSC1_RUNSTDBY    false     /* Don't run in standby */
#define BOARD_XOSC1_ONDEMAND    true      /* Disable on-demand control */
#define BOARD_XOSC1_LOWGAIN     false     /* Disable low buffer gain */
#define BOARD_XOSC1_ENALC       false     /* Disable automatic loop control */
#define BOARD_XOSC1_CFDEN       false     /* Clock failure detector not enabled */
#define BOARD_XOSC1_SWBEN       false     /* XOSC clock switch not enabled */
#define BOARD_XOSC1_STARTUP     0         /* XOSC0 start-up time 31µs */

/* GCLK */

#define BOARD_GCLK_SET1         0x0000    /* The empty set */
#define BOARD_GCLK_SET2         0x0fff    /* All GCLKs */

#define BOARD_GCLK0_ENABLE      true      /* Enable GCLK0 */
#define BOARD_GCLK0_IDC         false     /* Don't improve duty cycle */
#define BOARD_GCLK0_OOV         false     /* Clock output will be LOW */
#define BOARD_GCLK0_OE          true      /* Generate output on GCLK_IO */
#define BOARD_GCLK0_DIVSEL      0         /* GCLK frequency is source/DIV */
#define BOARD_GCLK0_RUNSTDBY    false     /* Don't run in standby */
#define BOARD_GCLK0_SOURCE      7         /* Select DPLL0 output as GLCK0 source */
#define BOARD_GCLK0_DIV         1         /* Division factor */

#define BOARD_GCLK1_ENABLE      true      /* Enable GCLK1 */
#define BOARD_GCLK1_IDC         false     /* Don't improve duty cycle */
#define BOARD_GCLK1_OOV         false     /* Clock output will be LOW */
#define BOARD_GCLK1_OE          true      /* Generate output on GCLK_IO */
#define BOARD_GCLK1_DIVSEL      0         /* GCLK frequency is source/DIV */
#define BOARD_GCLK1_RUNSTDBY    false     /* Don't run in standby */
#define BOARD_GCLK1_SOURCE      6         /* Select DFLL output as GLCK1 source */
#define BOARD_GCLK1_DIV         1         /* Division factor */

#define BOARD_GCLK2_ENABLE      false     /* Don't enable GCLK2 */
#define BOARD_GCLK2_IDC         false     /* Don't improve duty cycle */
#define BOARD_GCLK2_OOV         false     /* Clock output will be LOW */
#define BOARD_GCLK2_OE          false     /* No generator output of GCLK_IO */
#define BOARD_GCLK2_DIVSEL      1         /* GCLK frequency is source/(2^(N+1) */
#define BOARD_GCLK2_RUNSTDBY    false     /* Don't run in standby */
#define BOARD_GCLK2_SOURCE      5         /* Select XOSC32K as GLCK2 source */
#define BOARD_GCLK2_DIV         1         /* Division factor */

#define BOARD_GCLK3_ENABLE      false     /* Don't enable GCLK3 */
#define BOARD_GCLK3_IDC         false     /* Don't improve duty cycle */
#define BOARD_GCLK3_OOV         false     /* Clock output will be LOW */
#define BOARD_GCLK3_OE          false     /* No generator output of GCLK_IO */
#define BOARD_GCLK3_DIVSEL      0         /* GCLK frequency is source/DIV */
#define BOARD_GCLK3_RUNSTDBY    false     /* Don't run in standby */
#define BOARD_GCLK3_SOURCE      5         /* Select XOSC32K as GLCK3 source */
#define BOARD_GCLK3_DIV         1         /* Division factor */

#define BOARD_GCLK4_ENABLE      true      /* Enable GCLK4 */
#define BOARD_GCLK4_IDC         false     /* Don't improve duty cycle */
#define BOARD_GCLK4_OOV         false     /* Clock output will be LOW */
#define BOARD_GCLK4_OE          true      /* Generate output on GCLK_IO */
#define BOARD_GCLK4_DIVSEL      0         /* GCLK frequency is source/DIV */
#define BOARD_GCLK4_RUNSTDBY    false     /* Don't run in standby */
#define BOARD_GCLK4_SOURCE      7         /* Select DPLL0 output as GLCK4 source */
#define BOARD_GCLK4_DIV         1         /* Division factor */

#define BOARD_GCLK5_ENABLE      true      /* Enable GCLK5 */
#define BOARD_GCLK5_IDC         false     /* Don't improve duty cycle */
#define BOARD_GCLK5_OOV         false     /* Clock output will be LOW */
#define BOARD_GCLK5_OE          true      /* Generate output on GCLK_IO */
#define BOARD_GCLK5_DIVSEL      0         /* GCLK frequency is source/DIV */
#define BOARD_GCLK5_RUNSTDBY    false     /* Don't run in standby */
#define BOARD_GCLK5_SOURCE      6         /* Select DFLL output as GLCK5 source */
#define BOARD_GCLK5_DIV         24        /* Division factor */

#define BOARD_GCLK6_ENABLE      false     /* Don't enable GCLK6 */
#define BOARD_GCLK6_IDC         false     /* Don't improve duty cycle */
#define BOARD_GCLK6_OOV         false     /* Clock output will be LOW */
#define BOARD_GCLK6_OE          false     /* No generator output of GCLK_IO */
#define BOARD_GCLK6_DIVSEL      0         /* GCLK frequency is source/DIV */
#define BOARD_GCLK6_RUNSTDBY    false     /* Don't run in standby */
#define BOARD_GCLK6_SOURCE      1         /* Select XOSC1 as GLCK5 source */
#define BOARD_GCLK6_DIV         1         /* Division factor */

#define BOARD_GCLK7_ENABLE      false     /* Don't enable GCLK7 */
#define BOARD_GCLK7_IDC         false     /* Don't improve duty cycle */
#define BOARD_GCLK7_OOV         false     /* Clock output will be LOW */
#define BOARD_GCLK7_OE          false     /* No generator output of GCLK_IO */
#define BOARD_GCLK7_DIVSEL      0         /* GCLK frequency is source/DIV */
#define BOARD_GCLK7_RUNSTDBY    false     /* Don't run in standby */
#define BOARD_GCLK7_SOURCE      1         /* Select XOSC1 as GLCK5 source */
#define BOARD_GCLK7_DIV         1         /* Division factor */

#define BOARD_GCLK8_ENABLE      false     /* Don't enable GCLK8 */
#define BOARD_GCLK8_IDC         false     /* Don't improve duty cycle */
#define BOARD_GCLK8_OOV         false     /* Clock output will be LOW */
#define BOARD_GCLK8_OE          false     /* No generator output of GCLK_IO */
#define BOARD_GCLK8_DIVSEL      0         /* GCLK frequency is source/DIV */
#define BOARD_GCLK8_RUNSTDBY    false     /* Don't run in standby */
#define BOARD_GCLK8_SOURCE      1         /* Select XOSC1 as GLCK5 source */
#define BOARD_GCLK8_DIV         1         /* Division factor */

#define BOARD_GCLK9_ENABLE      false     /* Don't enable GCLK9 */
#define BOARD_GCLK9_IDC         false     /* Don't improve duty cycle */
#define BOARD_GCLK9_OOV         false     /* Clock output will be LOW */
#define BOARD_GCLK9_OE          false     /* No generator output of GCLK_IO */
#define BOARD_GCLK9_DIVSEL      0         /* GCLK frequency is source/DIV */
#define BOARD_GCLK9_RUNSTDBY    false     /* Don't run in standby */
#define BOARD_GCLK9_SOURCE      1         /* Select XOSC1 as GLCK5 source */
#define BOARD_GCLK9_DIV         1         /* Division factor */

#define BOARD_GCLK10_ENABLE     false     /* Don't enable GCLK10 */
#define BOARD_GCLK10_IDC        false     /* Don't improve duty cycle */
#define BOARD_GCLK10_OOV        false     /* Clock output will be LOW */
#define BOARD_GCLK10_OE         false     /* No generator output of GCLK_IO */
#define BOARD_GCLK10_DIVSEL     0         /* GCLK frequency is source/DIV */
#define BOARD_GCLK10_RUNSTDBY   false     /* Don't run in standby */
#define BOARD_GCLK10_SOURCE     1         /* Select XOSC1 as GLCK5 source */
#define BOARD_GCLK10_DIV        1         /* Division factor */

#define BOARD_GCLK11_ENABLE     false     /* Don't enable GCLK11 */
#define BOARD_GCLK11_IDC        false     /* Don't improve duty cycle */
#define BOARD_GCLK11_OOV        false     /* Clock output will be LOW */
#define BOARD_GCLK11_OE         false     /* No generator output of GCLK_IO */
#define BOARD_GCLK11_DIVSEL     0         /* GCLK frequency is source/DIV */
#define BOARD_GCLK11_RUNSTDBY   false     /* Don't run in standby */
#define BOARD_GCLK11_SOURCE     1         /* Select XOSC1 as GLCK5 source */
#define BOARD_GCLK11_DIV        1         /* Division factor */
#define BOARD_GCLK11_FREQUENCY  BOARD_XOSC1_FREQUENCY

/* FDLL */

#define BOARD_DFLL_ENABLE       true      /* DFLL enable */
#define BOARD_DFLL_RUNSTDBY     false     /* Don't run in standby */
#define BOARD_DFLL_ONDEMAND     false     /* No n-demand control */
#define BOARD_DFLL_MODE         false     /* Open loop mode */
#define BOARD_DFLL_STABLE       false     /* No stable DFLL frequency */
#define BOARD_DFLL_LLAW         false     /* Don't ose lock after wake */
#define BOARD_DFLL_USBCRM       true      /* Use USB clock recovery mode */
#define BOARD_DFLL_CCDIS        true      /* Chill cycle disable */
#define BOARD_DFLL_QLDIS        false     /* No Quick Lock Disable */
#define BOARD_DFLL_BPLCKC       false     /* No ypass coarse clock */
#define BOARD_DFLL_WAITLOCK     true      /* Wait lock */
#define BOARD_DFLL_CALIBEN      false     /* Don't verwrite factory calibration */
#define BOARD_DFLL_FCALIB       128       /* Coarse calibration value (if caliben) */
#define BOARD_DFLL_CCALIB       (31 / 4)  /* Fine calibration value (if caliben) */
#define BOARD_DFLL_FSTEP        1         /* Fine maximum step */
#define BOARD_DFLL_CSTEP        1         /* Coarse maximum step */
#define BOARD_DFLL_GCLK         3         /* GCLK source (if !usbcrm && !mode) */
#define BOARD_DFLL_MUL          0         /* DFLL multiply factor */

/* DPLL0/1
 *
 * Fckr is the frequency of the selected reference clock reference:
 *
 *    BOARD_XOSC32K_FREQENCY,
 *    BOARD_XOSCn_FREQUENCY / DIV, or
 *    BOARD_GCLKn_FREQUENCY
 *
 * The DPLL output frequency is then given by:
 *
 *   Fdpll = Fckr * (LDR + 1 + LDRFRAC / 32)
 *
 * DPLL0:
 *   Fckr  = BOARD_GCLK5_FREQUENCY = BOARD_DFLL_FREQUENCY / 24 = 2MHz
 *   Fdpll = 2Mhz * (59 + 1 + 0 / 32) = 120MHz
 *
 * DPLL1: (not enabled)
 *   Fckr  = BOARD_XOSCK32_FREQUENCY = 32.768KHz
 *   Fdpll = 32768 * (1463 + 1 + 13/32) = 47.986 MHz
 */

#define BOARD_DPLL0_ENABLE      true      /* DPLL enable */
#define BOARD_DPLL0_DCOEN       false     /* DCO filter enable */
#define BOARD_DPLL0_LBYPASS     false     /* Lock bypass */
#define BOARD_DPLL0_WUF         false     /* Wake up fast */
#define BOARD_DPLL0_RUNSTDBY    false     /* Run in standby */
#define BOARD_DPLL0_ONDEMAND    false     /* On demand clock activation */
#define BOARD_DPLL0_REFCLK      0         /* Reference clock selection */
#define BOARD_DPLL0_LTIME       0         /* Lock time  */
#define BOARD_DPLL0_FILTER      0         /* Proportional integer filter selection */
#define BOARD_DPLL0_DCOFILTER   0         /* Sigma-delta DCO filter selection */
#define BOARD_DPLL0_GCLK        5         /* GCLK5 source (if refclock == 0) */
#define BOARD_DPLL0_LDRFRAC     0         /* Loop divider fractional part */
#define BOARD_DPLL0_LDRINT      59        /* Loop divider ratio */
#define BOARD_DPLL0_DIV         0         /* Clock divider */

#define BOARD_DPLL1_ENABLE      false     /* DPLL enable */
#define BOARD_DPLL1_DCOEN       false     /* DCO filter enable */
#define BOARD_DPLL1_LBYPASS     false     /* Lock bypass */
#define BOARD_DPLL1_WUF         false     /* Wake up fast */
#define BOARD_DPLL1_RUNSTDBY    false     /* Run in standby */
#define BOARD_DPLL1_ONDEMAND    false     /* On demand clock activation */
#define BOARD_DPLL1_REFCLK      1         /* Reference clock = XOSCK32 */
#define BOARD_DPLL1_LTIME       0         /* Lock time  */
#define BOARD_DPLL1_FILTER      0         /* Sigma-delta DCO filter selection */
#define BOARD_DPLL1_DCOFILTER   0         /* Sigma-delta DCO filter selection */
#define BOARD_DPLL1_GCLK        0         /* GCLK source (if refclock == 0) */
#define BOARD_DPLL1_LDRFRAC     13        /* Loop divider fractional part */
#define BOARD_DPLL1_LDRINT      1463      /* Loop divider ratio */
#define BOARD_DPLL1_DIV         0         /* Clock divider */

/* Master Clock (MCLK)
 *
 * GCLK0 is always the direct source the GCLK_MAIN.
 * CPU frequency = 120MHz / 1 = 120MHz
 */

#define BOARD_MCLK_CPUDIV       1         /* MCLK divder to get CPU frequency */

/* Peripheral clocking */

#define BOARD_GLCK_EIC          4         /* EIC GLCK index */

/* FLASH wait states
 *
 * Vdd Range Wait states Maximum Operating Frequency
 * --------- ----------- ---------------------------
 * > 2.7V    0            24 MHz
 *           1            51 MHz
 *           2            77 MHz
 *           3           101 MHz
 *           4           119 MHz
 *           5           120 MHz
 * >1.71V    0            22 MHz
 *           1            44 MHz
 *           2            67 MHz
 *           3            89 MHz
 *           4           111 MHz
 *           5           120 MHz
 */

#define BOARD_FLASH_WAITSTATES  6

/* LED definitions ******************************************************************/
/* The Adafruit Metro M4 has four LEDs, but only two are controllable by software:
 *
 *   1. The red LED on the Arduino D13 pin, and
 *   2. A NeoPixel RGB LED.
 *
 * Currently, only the red LED is supported.
 *
 *   ------ ----------------- -----------
 *   SHIELD SAMD5E5           FUNCTION
 *   ------ ----------------- -----------
 *   D13    PA16              GPIO output
 */

/* LED index values for use with board_userled() */

#define BOARD_RED_LED     0
#define BOARD_NLEDS       1

/* LED bits for use with board_userled_all() */

#define BOARD_RED_LED_BIT (1 << BOARD_RED_LED)

/* This LED is not used by the board port unless CONFIG_ARCH_LEDS is
 * defined.  In that case, the usage by the board port is defined in
 * include/board.h and src/sam_autoleds.c. The LEDs are used to encode
 * OS-related events as follows:
 *
 *   ------------------- ---------------------------- ------
 *   SYMBOL                  Meaning                  LED
 *   ------------------- ---------------------------- ------   */

#define LED_STARTED      0 /* NuttX has been started  OFF      */
#define LED_HEAPALLOCATE 0 /* Heap has been allocated OFF      */
#define LED_IRQSENABLED  0 /* Interrupts enabled      OFF      */
#define LED_STACKCREATED 1 /* Idle stack created      ON       */
#define LED_INIRQ        2 /* In an interrupt         N/C      */
#define LED_SIGNAL       2 /* In a signal handler     N/C      */
#define LED_ASSERTION    2 /* An assertion failed     N/C      */
#define LED_PANIC        3 /* The system has crashed  FLASH    */
#undef  LED_IDLE           /* MCU is is sleep mode    Not used */

/* Thus is LED is statically on, NuttX has successfully  booted and is,
 * apparently, running normally.  If LED is flashing at approximately
 * 2Hz, then a fatal error has been detected and the system has halted.
 */

/* Alternate function pin selections ************************************************/

/* SERCOM definitions ***************************************************************/
/* The SERCOM bus clock (CLK_SERCOMx_APB) can be enabled and disabled in the Main
 * Clock Controller.  The SERCOM uses two generic clocks: GCLK_SERCOMx_CORE and
 * GCLK_SERCOMx_SLOW. The core clock (GCLK_SERCOMx_CORE) is required to clock the
 * SERCOM while working as a master.  The slow clock (GCLK_SERCOMx_SLOW) is only
 * required for certain functions.
 *
 * These clocks must be configured and enabled in the Generic Clock Controller (GCLK)
 * before using the SERCOM.
 */

/* SERCOM3
 *
 * An Arduino compatible serial Shield is assumed (or equivalently, an external
 * RS-232 or serial-to-USB adapter connected on Arduino pins D0 and D1):
 *
 *   ------ ----------------- ---------
 *   SHIELD SAMD5E5           FUNCTION
 *   ------ ----------------- ---------
 *   D0     PA23 SERCOM3 PAD1 RXD
 *   D1     PA22 SERCOM3 PAD0 TXD
 *
 * NOTES:
 *   USART_CTRLA_TXPAD0_1: TxD=PAD0XCK=N/A RTS/TE=PAD2 CTS=PAD3
 *   USART_CTRLA_RXPAD2:   RxD=PAD1
 */

#define BOARD_SERCOM3_MUXCONFIG      (USART_CTRLA_TXPAD0_2 | USART_CTRLA_RXPAD2)
#define BOARD_SERCOM3_PINMAP_PAD0    PORT_SERCOM3_PAD0_1 /* USART TX */
#define BOARD_SERCOM3_PINMAP_PAD1    PORT_SERCOM3_PAD1_1 /* USART RX */
#define BOARD_SERCOM3_PINMAP_PAD2    0                   /* (not used) */
#define BOARD_SERCOM3_PINMAP_PAD3    0                   /* (not used) */

#define BOARD_TXIRQ_SERCOM3          SAM_IRQ_SERCOM3_0
#define BOARD_RXIRQ_SERCOM3          SAM_IRQ_SERCOM3_1

#define BOARD_SERCOM3_COREGEN        1                   /* 48MHz, common to all SERCOMS */
#define BOARD_SERCOM3_SLOWGEN        3                   /* 48MHz */
#define BOARD_SERCOM3_FREQUENCY      BOARD_GCLK1_FREQUENCY

/* USB */

#define BOARD_USB_GCLKGEN            1                   /* GCLK1, 48MHz */

#endif  /* __CONFIG_METRO_M4_INCLUDE_BOARD_H */
