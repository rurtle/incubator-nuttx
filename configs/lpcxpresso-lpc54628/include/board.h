/****************************************************************************
 * configs/lpcxpresso-lpc54628/include/board.h
 *
 *   Copyright (C) 2017 Gregory Nutt. All rights reserved.
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
 ****************************************************************************/

#ifndef _CONFIGS_LPCXPRESSO_LPC54628_INCLUDE_BOARD_H
#define _CONFIGS_LPCXPRESSO_LPC54628_INCLUDE_BOARD_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Clocking ****************************************************************/

#undef  BOARD_180MHz
#define BOARD_220MHz                    1

/* System PLL
 *
 * Notation:
 *   Fin  = the input to the PLL.
 *   Fout = the output of the PLL.
 *   Fref = the PLL reference frequency, the input to the phase frequency
 *          detector.
 *   N    = optional pre-divider value.
 *   M    = feedback divider value, which represents the multiplier for the
 *          PLL.
 *   P    = optional post-divider value. An additional divide-by-2 is
 *          included in the post-divider path.
 *
 * In all variations of normal mode, the following requirements must be met:
 *
 *   -275 MHz ≤ Fcco ≤ 550 MHz.
 *   4 kHz ≤ Fin / N ≤ 25 MHz.
 *
 * Normal mode with optional pre-divide.  In the equations, use N = 1 when
 * the pre-divider is not used.  The extra divide by 2 is in the feedback
 * divider path:
 *
 *   Fout = Fcco = 2 x M x Fin / N
 *
 * Normal mode with post-divide and optional pre-divide.  In the equations,
 * use N = 1 when the pre-divider is not used:
 *
 *   Fout = Fcco / (2 x P) = M x Fin / (N x P)
 */

#define BOARD_PLL_SOURCE                          /* Select source FR0 12MHz */
#define BOARD_PLL_FIN            LPC54_FRO_12MHZ  /* PLL input frequency */

#ifdef BOARD_180MHz
/* PLL Clock Source:             CLKIN
 * Main Clock Source:            PLL
 * Fout:                         220000000
 */

#  define BOARD_PLL_CLKSEL       SYSCON_SYSPLLCLKSEL_CLKIN
#  define BOARD_PLL_SELI         32               /* Bandwidth select I value */
#  define BOARD_PLL_SELP         16               /* Bandwidth select P value */
#  define BOARD_PLL_SELR         0                /* Bandwidth select R value */
#  define BOARD_PLL_MDEC         8191             /* Encoded M-divider coefficient */
#  define BOARD_PLL_NDEC         770              /* Encoded N-divider coefficient */
#  define BOARD_PLL_PDEC         98               /* Encoded P-divider coefficient */
#  define BOARD_PLL_FOUT         180000000U       /* Pll output frequency */

#else /* BOARD_220MHz */
/* PLL Clock Source:             FRO 12MHz
 * Main Clock Source:            PLL
 * Fout:                         220000000
 */

#  define BOARD_PLL_CLKSEL       SYSCON_SYSPLLCLKSEL_FFRO
#  define BOARD_PLL_SELI         34               /* Bandwidth select I value */
#  define BOARD_PLL_SELP         31               /* Bandwidth select P value */
#  define BOARD_PLL_SELR         0                /* Bandwidth select R value */
#  define BOARD_PLL_MDEC         13243            /* Encoded M-divider coefficient */
#  define BOARD_PLL_NDEC         1                /* Encoded N-divider coefficient */
#  define BOARD_PLL_PDEC         98               /* Encoded P-divider coefficient */
#  define BOARD_PLL_FOUT         220000000        /* Pll output frequency */
#endif

#define BOARD_MAIN_CLK           BOARD_PLL_FOUT   /* Main clock frequency */

/* CPU Clock:
 *
 *   AHB Clock Divider:          1
 *   AHB Clock Frequency:        180,000,000 or 220,000,000
 */

#define BOARD_AHBCLKDIV          1                 /* (un-decremented) */
#define BOARD_AHB_FREQUENCY      (BOARD_MAIN_CLK / BOARD_AHBCLKDIV)
#define BOARD_CPU_FREQUENCY      BOARD_AHB_FREQUENCY

/* Fraction Rate Generator (FRG) Clock (Optional Flexcomm0 function clock)
 *
 * To use the fractional divider, the DIV value must be programmed with the
 * fixed value of 256.  Then:
 *
 *   Ffrg = (Finput) / (1 + (MULT / DIV))
 *
 * Mainclock is used as the FRG clock source.  Divider must be such that the
 * FRG output frequency is less than equal to 48MHz
 *
 *   MUL = (Finput - Ffrg) * 256) / Ffrg
 */

/* Revisit: FRGCLK <= 48MHz cannot be realized with the MainClk source */

#define BOARD FRGCLK_CLKSEL      SYSCON_FRGCLKSEL_MAINCLK
#define BOARD_FRGCLK_INPUT       BOARD_MAIN_CLK   /* FRG input frequency */
#define BOARD_FRGCLK             48000000         /* May not be exact */

/* SysTick:  The SysTick clock may be clocked internally either by the by the
 * system clock (CLKSOURCE==1) or by the SysTick function clock (CLKSOURCE==0).
 * The SysTick Function clock is equal to:
 *
 *   Fsystick = Fmainclk / SYSTICKCLKDIV
 *
 * Tips for selecting BOARD_SYSTICKCLKDIV:  The resulting SysTick reload value
 * should be as large as possible, but must be less than 2^24:
 *
 *   SYSTICKDIV > Fmainclk / CLK_TCK / 2^24
 *
 * The logic in lpc54_timerisr.c will always select the SysTick function clock
 * as the source (CLKSOURCE==0).  NOTE: When the system tick clock divider is
 * selected as the clock source, the CPU clock must be at least 2.5 times
 * faster than the divider output.
 *
 *   SysTick Divider:            (SYSTICKCLKDIV)
 */

#ifndef CONFIG_SCHED_TICKLESS
#  if CONFIG_USEC_PER_TICK == 10000
#    define BOARD_SYSTICKCLKDIV  1
#  else
#    error Missing SYSTICK divider
#  endif
#  define BOARD_SYSTICK_CLOCK    (BOARD_AHB_FREQUENCY / BOARD_SYSTICKCLKDIV)
#endif

/* Flexcomm0:  USART0 (REVIST) */

#define BOARD_FLEXCOMM0_CLKSEL   SYSCON_FCLKSEL_FRO12M
#define BOARD_FLEXCOMM0_FCLK     LPC54_FRO_12MHZ

/* Flexcomm2:  I2C2 (REVIST) */

#define BOARD_FLEXCOMM2_CLKSEL   SYSCON_FCLKSEL_FRO12M
#define BOARD_FLEXCOMM2_FCLK     LPC54_FRO_12MHZ

/* EMC */

#ifdef BOARD_220MHz
#define BOARD_EMC_CLKDIV         3     /* EMC Clock = CPU FREQ/3 */
#else /* if BOARD_180MHz */
#define BOARD_EMC_CLKDIV         2     /* EMC Clock = CPU FREQ/2 */
#endif
#define BOARD_EMC_FREQUENCY      (BOARD_CPU_FREQUENCY / BOARD_EMC_CLKDIV)

/* SD/MMC or SDIO interface
 *
 * NOTE: The SDIO function clock to the interface can be up to 50 MHZ.
 * Example:  BOARD_MAIN_CLK=220MHz, CLKDIV=5
 */

#define BOARD_SDMMC_MAXFREQ      50000000
#define BOARD_SDMMC_CEIL(a,b)    (((a) + (b) - 1) / (b))

#define BOARD_SDMMC_CLKSRC       SYSCON_SDIOCLKSEL_MAINCLK
#define BOARD_SDMMC_CLKDIV       BOARD_SDMMC_CEIL(BOARD_MAIN_CLK, BOARD_SDMMC_MAXFREQ)
#define BOARD_SDMMC_FREQUENCY    (BOARD_MAIN_CLK / BOARD_SDMMC_CLKDIV)

/* LED definitions *********************************************************/
/* The LPCXpress-LPC54628 has three user LEDs: D9, D11, and D12.  These
 * LEDs are for application use. They are illuminated when the driving
 * signal from the LPC546xx is low. The LEDs are driven by ports P2-2 (D9),
 * P3-3 (D11) and P3-14 (D12).
 */

/* LED index values for use with board_userled() */

#define BOARD_D9                   0
#define BOARD_D11                  1
#ifndef CONFIG_ARCH_LEDS
#  define BOARD_D12                2
#  define BOARD_NLEDS              3
#else
#  define BOARD_NLEDS              2
#endif

/* LED bits for use with board_userled_all() */

#define BOARD_D9_BIT               (1 << BOARD_D9)
#define BOARD_D11_BIT              (1 << BOARD_D11)
#ifndef CONFIG_ARCH_LEDS
#  define BOARD_D12_BIT            (1 << BOARD_D12)
#endif

/* These LEDs are not used by the NuttX port unless CONFIG_ARCH_LEDS is
 * defined.  In that case, the usage by the board port is defined in
 * include/board.h and src/lpc54_autoleds.c.  The LEDs are used to encode
 * OS-related events as follows:
 */
                                      /* D9     D11    D12 */
#define LED_STARTED                0  /* OFF    OFF    OFF */
#define LED_HEAPALLOCATE           1  /* ON     OFF    OFF */
#define LED_IRQSENABLED            2  /* OFF    ON     OFF */
#define LED_STACKCREATED           3  /* OFF    OFF    OFF */

#define LED_INIRQ                  4  /* NC     NC     ON  (momentary) */
#define LED_SIGNAL                 4  /* NC     NC     ON  (momentary) */
#define LED_ASSERTION              4  /* NC     NC     ON  (momentary) */
#define LED_PANIC                  4  /* NC     NC     ON  (2Hz flashing) */
#undef  LED_IDLE                      /* Sleep mode indication not supported */

/* After booting, LEDs D9 and D11 are avaible for use by the user.  If the
 * system booted properly, D9 and D11 should be OFF and D12 should be glowing
 * to indicate that interrupts are occurring.  If D12 is flash at 2Hz, then
 * the system has crashed.
 */

/* Button definitions *******************************************************/
/* The LPCXpresso has four switches:
 *
 *   SW2 ISP2         P0.6
 *   SW3 ISP1         P0.5
 *   SW4 ISP0         P0.4
 *   SW5 User Button  P1.1
 *
 * In all cased, the signal is low when the button is pressed.
 *
 * SW2, SW3, SW4 can be used to force the LPC546xx in to ISP boot modes.
 * After boot these buttons could be used as user buttons.  However, they are
 * not available when the on-board SRDRAM is used because P0.4, P0.5, and
 * P0.6 are also used as EMC_D2, EMC_D3, and EMC_D4, respectively.
 *
 * So SW5 is really the only button that that is generally available for
 * software usage.
 */

#define BUTTON_USER                0
#define NUM_BUTTONS                1

#define BUTTON_USER_BIT            (1 << BUTTON_USER)

/* Pin Disambiguation *******************************************************/
/* Flexcomm0/USART0
 *
 * USART0 connects to the serial bridge on LPC4322JET100 and is typlical used
 * for the serial console.
 *
 *   BRIDGE_UART_RXD -> P0_29-ISP_FC0_RXD -> P0.29  GPIO_FC0_RXD_SDA_MOSI_2
 *   BRIDGE_UART_TXD <- P0_30-ISP_FC0_TXD <- P0.30  GPIO_FC0_TXD_SCL_MISO_2
 */

#define GPIO_USART0_RXD            (GPIO_FC0_RXD_SDA_MOSI_2 | GPIO_FILTER_OFF)
#define GPIO_USART0_TXD            (GPIO_FC0_TXD_SCL_MISO_2 | GPIO_FILTER_OFF)

/* Flexomm2/I2C
 *
 * For I2C:
 *   Type A & D pins need:
 *     GPIO_OPENDRAIN + GPIO_FILTER_OFF
 *   Type I pins need for Standard mode I2C need:
 *     GPIO_FILTER_OFF + GPIO_I2C_FILTER_ON + GPIO_I2CDRIVE_LOW
 *   Type I pins need for fast speed I2C need:
 *     GPIO_FILTER_OFF + GPIO_I2C_FILTER_ON or OFF +
 *     GPIO_I2CDRIVE_LOW or HIGH
 *   Type I pins need for high speed I2C need:
 *     GPIO_FILTER_OFF + GPIO_I2C_FILTER_OFF + GPIO_I2CDRIVE_HIGH
 *
 * The touchscreen controller is on I2C2: SCL P3.24, SDA P3.23.  These are
 * both Type D/I pins.
 */

#if defined(CONFIG_LPC54_I2C_FAST)
#  define _I2CFILTER               GPIO_I2C_FILTER_OFF
#  define _I2CDRIVE                GPIO_I2CDRIVE_HIGH
#elif defined(CONFIG_LPC54_I2C_HIGH)
#  define _I2CFILTER               GPIO_I2C_FILTER_OFF
#  define _I2CDRIVE                GPIO_I2CDRIVE_HIGH
#else
#  define _I2CFILTER               GPIO_I2C_FILTER_ON
#  define _I2CDRIVE                GPIO_I2CDRIVE_LOW
#endif

#define GPIO_I2C2_SCL              (GPIO_FC2_RTS_SCL_SSEL1_2 | \
                                    GPIO_FILTER_OFF | _I2CFILTER | \
                                    _I2CDRIVE)
#define GPIO_I2C2_SDA              (GPIO_FC2_CTS_SDA_SSEL0_2 | \
                                    GPIO_FILTER_OFF | _I2CFILTER | \
                                    _I2CDRIVE)

/* SD/MMC
 *
 *  P2_10-SD_CDn
 *  P2_6-SD_D0
 *  P2_7-SD_D1
 *  P2_8-SD_D2
 *  P2_9-SD_D3
 *  P2_3-SD_CLK
 *  P2_4-SD_CMD
 *  P2_5-SD_POW_EN
 *  P3_15-SD_WPn
 */

#define GPIO_SD_CARD_DET_N         GPIO_SD_CARD_DET_N_2     /* P2.10 */
#define GPIO_SD_D0                 GPIO_SD_D0_3             /* P2.9 */
#define GPIO_SD_D1                 GPIO_SD_D1_3             /* P2.7 */
#define GPIO_SD_D2                 GPIO_SD_D2_3             /* P2.8 */
#define GPIO_SD_D3                 GPIO_SD_D3_3             /* P2.9 */
#define GPIO_SD_CLK                GPIO_SD_CLK_3            /* P2.3 */
#define GPIO_SD_CMD                GPIO_SD_CMD_3            /* P2.4 */
#define GPIO_SD_POW_EN             GPIO_SD_POW_EN_2         /* P2.5 */
#define GPIO_SD_WR_PRT             GPIO_SD_WR_PRT_2         /* P2.15 */

/* LCD
 *
 *   There are no alternatives for LCD pins except for the VD0-VD3 pins. 
 *   VD0-VD2 are not used in this hardware configuration.  VD3 is on
 *   P2.21.
 */

#define GPIO_LCD_VD3               GPIO_LCD_VD3_1

/****************************************************************************
 * Public Types
 ****************************************************************************/

#ifndef __ASSEMBLY__

/****************************************************************************
 * Public Data
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif  /* _CONFIGS_LPCXPRESSO_LPC54628_INCLUDE_BOARD_H */
