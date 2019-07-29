/*****************************************************************************
******************************************************************************
*                                                                            *
*     NNNNNN         N                A             TTTTTTT TTTTTTTTTT       *
*     N NNNNN        N               AAA                  T TTTTT            *
*     NN NNNNN       N              AAAAA                 T TTTTT            *
*     N N NNNNN      N             A AAAAA                T TTTTT            *
*     N  N NNNNN     N            A A AAAAA               T TTTTT            *
*     N   N NNNNN    N           A   A AAAAA              T TTTTT            *
*     N    N NNNNN   N          A     A AAAAA             T TTTTT            *
*     N     N NNNNN  N         AAAAAAAAA AAAAA            T TTTTT            *
*     N      N NNNNN N        A         A AAAAA           T TTTTT            *
*     N       N NNNNNN       A           A AAAAA          T TTTTT            *
*     N        N NNNNN  OO  A             A AAAAA  OO     T TTTTT     OO     *
*                       OO                         OO                 OO     *
*                                                                            *
*     Gesellschaft fuer Netzwerk- und Automatisierungstechnologie m.b.H      *
*         Konrad-Zuse-Platz 9, D-53227 Bonn, Tel.:+49 228/965 864-0          *
*                             www.nateurope.com                              *
*                                                                            *
******************************************************************************
******************************************************************************
*
* Module      :
*
* Description :
*
* Author      :
*
******************************************************************************
******************************************************************************
*
*                    Copyright (c) by N.A.T. GmbH
*
*       All rights reserved. Copying, compilation, modification,
*       distribution or any other use whatsoever of this material
*       is strictly prohibited except in accordance with a Software
*       License Agreement with N.A.T. GmbH.
*
******************************************************************************
******************************************************************************/


#ifndef NAMC_PTM_PORT_H_
#define NAMC_PTM_PORT_H_

#include <asf.h>


void io_init(void);

/** USART0 pins definitions, PB0,PB1. */
#define UART0_RXD_GPIO   PIO_PA9_IDX
#define UART0_RXD_FLAGS  IOPORT_MODE_MUX_A
#define UART0_TXD_GPIO   PIO_PA10_IDX
#define UART0_TXD_FLAGS  IOPORT_MODE_MUX_A

/* SAM heartbeat signal to AVR */
#define PIN_SAM_HEARTBEAT	PIO_PD18_IDX

/* Front LED pins definitions */
#define LED_ACTIVE_LEVEL (IOPORT_PIN_LEVEL_LOW)
#define LED_INACTIVE_LEVEL (IOPORT_PIN_LEVEL_HIGH)

#define LED1R_GPIO            (PIO_PA26_IDX)
#define LED1G_GPIO            (PIO_PA25_IDX)

#define LED2R_GPIO            (PIO_PA24_IDX)
#define LED2G_GPIO            (PIO_PA23_IDX)

#define LED3R_GPIO            (PIO_PA16_IDX)
#define LED3G_GPIO            (PIO_PA15_IDX)

#define LED4R_GPIO            (PIO_PA21_IDX)
#define LED4G_GPIO            (PIO_PA22_IDX)

#define LED5R_GPIO            (PIO_PA18_IDX)
#define LED5G_GPIO            (PIO_PA17_IDX)

#define LED6R_GPIO            (PIO_PA20_IDX)
#define LED6G_GPIO            (PIO_PA19_IDX)

#define LED7R_GPIO            (PIO_PA28_IDX)
#define LED7G_GPIO            (PIO_PA27_IDX)

#define LED8R_GPIO            (PIO_PA31_IDX)
#define LED8G_GPIO            (PIO_PA30_IDX)

/** TWI0 pins definition */
#define TWIHS0_DATA_GPIO   PIO_PA3_IDX
#define TWIHS0_DATA_FLAGS  (IOPORT_MODE_MUX_A)
#define TWIHS0_CLK_GPIO    PIO_PA4_IDX
#define TWIHS0_CLK_FLAGS   (IOPORT_MODE_MUX_A)

/** TWI1 pins definition */
#define TWIHS1_DATA_GPIO   PIO_PB4_IDX
#define TWIHS1_DATA_FLAGS  (IOPORT_MODE_MUX_A)
#define TWIHS1_CLK_GPIO    PIO_PB5_IDX
#define TWIHS1_CLK_FLAGS   (IOPORT_MODE_MUX_A)

/** TWI2 pins definition */
#define TWIHS2_DATA_GPIO   PIO_PD27_IDX
#define TWIHS2_DATA_FLAGS  (IOPORT_MODE_MUX_C)
#define TWIHS2_CLK_GPIO    PIO_PD28_IDX
#define TWIHS2_CLK_FLAGS   (IOPORT_MODE_MUX_C)

/* Ethernet Switch: 88E6320 relate PIN definition */
#define PIN_88E6320_TXC_IDX                PIO_PD0_IDX  /* Used as refclk src in RMII mode */
#define PIN_88E6320_TXC_FLAGS             (IOPORT_MODE_MUX_A) /* Used as refclk src in RMII mode */
#define PIN_88E6320_TXEN_IDX                PIO_PD1_IDX
#define PIN_88E6320_TXEN_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_88E6320_TXD1_IDX                PIO_PD3_IDX
#define PIN_88E6320_TXD1_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_88E6320_TXD0_IDX                PIO_PD2_IDX
#define PIN_88E6320_TXD0_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_88E6320_RXD1_IDX                PIO_PD6_IDX
#define PIN_88E6320_RXD1_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_88E6320_RXD0_IDX                PIO_PD5_IDX
#define PIN_88E6320_RXD0_FLAGS            (IOPORT_MODE_MUX_A)

// #define PIN_88E6320_RXDV_IDX                PIO_PD4_IDX /* Used as TX_CTL in RGMII mode */
// #define PIN_88E6320_RXDV_FLAGS            (IOPORT_MODE_MUX_A) /* Used as TX_CTL in RGMII mode */
#define PIN_88E6320_MDC_IDX					PIO_PD8_IDX
#define PIN_88E6320_MDC_FLAGS            (IOPORT_MODE_MUX_A)
#define PIN_88E6320_MDIO_IDX                PIO_PD9_IDX
#define PIN_88E6320_MDIO_FLAGS            (IOPORT_MODE_MUX_A)

#define PIN_88E6320_INT	                PIO_PD10_IDX
#define PIN_88E6320_INT_MASK	        PIO_PD10
#define PIN_88E6320_INT_PIO				PIOD
#define PIN_88E6320_INT_ID				ID_PIOD
#define PIN_88E6320_INT_IRQn			PIOD_IRQn

#define PIN_88E6320_RSTN				PIO_PD11_IDX
#define PIN_88E6320_RSTN_ACTIVE_LEVEL	IOPORT_PIN_LEVEL_LOW
#define PIN_88E6320_RSTN_INACTIVE_LEVEL	IOPORT_PIN_LEVEL_HIGH

/* Bot heat segment */

#define BOT_HEAT_SEGMENT_1_GPIO   PIO_PA11_IDX
#define BOT_HEAT_SEGMENT_1_FLAGS  IOPORT_MODE_MUX_A
#define BOT_HEAT_SEGMENT_2_GPIO   PIO_PA12_IDX
#define BOT_HEAT_SEGMENT_2_FLAGS  IOPORT_MODE_MUX_A
#define BOT_HEAT_SEGMENT_3_GPIO   PIO_PA13_IDX
#define BOT_HEAT_SEGMENT_3_FLAGS  IOPORT_MODE_MUX_A
#define BOT_HEAT_SEGMENT_4_GPIO   PIO_PA14_IDX
#define BOT_HEAT_SEGMENT_4_FLAGS  IOPORT_MODE_MUX_A
#define BOT_HEAT_SEGMENT_ACTIVE_LEVEL    (IOPORT_PIN_LEVEL_LOW)
#define BOT_HEAT_SEGMENT_INACTIVE_LEVEL  (IOPORT_PIN_LEVEL_HIGH)


#endif  /* NAMC_PTM_PORT_H_ */


