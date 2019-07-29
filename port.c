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
* Author      : Tom Welbers (TW)
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
******************************************************************************/
/*****************************************************************************/

/*****************************************************************************/
/*  INCLUDES                                                                 */
/*****************************************************************************/
#include <asf.h>
#include "port.h"

/*****************************************************************************/
/*  PRIVATE MACROS                                                           */
/*****************************************************************************/
/**
 * \brief Set peripheral mode for IOPORT pins.
 * It will configure port mode and disable pin mode (but enable peripheral).
 * \param port IOPORT port to configure
 * \param masks IOPORT pin masks to configure
 * \param mode Mode masks to configure for the specified pin (\ref ioport_modes)
 */
#define ioport_set_port_peripheral_mode(port, masks, mode) \
	do {\
		ioport_set_port_mode(port, masks, mode);\
		ioport_disable_port(port, masks);\
	} while (0)

/**
 * \brief Set peripheral mode for one single IOPORT pin.
 * It will configure port mode and disable pin mode (but enable peripheral).
 * \param pin IOPORT pin to configure
 * \param mode Mode masks to configure for the specified pin (\ref ioport_modes)
 */
#define ioport_set_pin_peripheral_mode(pin, mode) \
	do {\
		ioport_set_pin_mode(pin, mode);\
		ioport_disable_pin(pin);\
	} while (0)

/**
 * \brief Set input mode for one single IOPORT pin.
 * It will configure port mode and disable pin mode (but enable peripheral).
 * \param pin IOPORT pin to configure
 * \param mode Mode masks to configure for the specified pin (\ref ioport_modes)
 * \param sense Sense for interrupt detection (\ref ioport_sense)
 */
#define ioport_set_pin_input_mode(pin, mode, sense) \
	do {\
		ioport_set_pin_dir(pin, IOPORT_DIR_INPUT);\
		ioport_set_pin_mode(pin, mode);\
		ioport_set_pin_sense_mode(pin, sense);\
	} while (0)


/*---------------------------------------------------------------------------*
Description:	Initialize MCU IO pins
Parameters:		none
Return:			none
*-----------------------------------------------------------------------------*/
void io_init(void)
{
	/*
	 * This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */

	/* Initialize IOPORTs */
	ioport_init();
	/* Configure UART pins */
	ioport_set_pin_peripheral_mode(UART0_RXD_GPIO, UART0_RXD_FLAGS);
	MATRIX->CCFG_SYSIO |= CCFG_SYSIO_SYSIO4;
	ioport_set_pin_peripheral_mode(UART0_TXD_GPIO, UART0_TXD_FLAGS);

	/* Configure heartbeat pin to AVR */
	ioport_set_pin_dir(PIN_SAM_HEARTBEAT, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIN_SAM_HEARTBEAT, IOPORT_PIN_LEVEL_LOW);

	/* Configure LED pins */
	ioport_set_pin_dir(LED1R_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED1G_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(LED1G_GPIO,LED_INACTIVE_LEVEL);
	ioport_set_pin_level(LED1R_GPIO,LED_INACTIVE_LEVEL);
	ioport_set_pin_dir(LED2R_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED2G_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(LED2G_GPIO,LED_INACTIVE_LEVEL);
	ioport_set_pin_level(LED2R_GPIO,LED_INACTIVE_LEVEL);
	ioport_set_pin_dir(LED3R_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED3G_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(LED3G_GPIO,LED_INACTIVE_LEVEL);
	ioport_set_pin_level(LED3R_GPIO,LED_INACTIVE_LEVEL);
	ioport_set_pin_dir(LED4R_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED4G_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(LED4G_GPIO,LED_INACTIVE_LEVEL);
	ioport_set_pin_level(LED4R_GPIO,LED_INACTIVE_LEVEL);
	ioport_set_pin_dir(LED5R_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED5G_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(LED5G_GPIO,LED_INACTIVE_LEVEL);
	ioport_set_pin_level(LED5R_GPIO,LED_INACTIVE_LEVEL);
	ioport_set_pin_dir(LED6R_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED6G_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(LED6G_GPIO,LED_INACTIVE_LEVEL);
	ioport_set_pin_level(LED6R_GPIO,LED_INACTIVE_LEVEL);
	ioport_set_pin_dir(LED7R_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED7G_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(LED7G_GPIO,LED_INACTIVE_LEVEL);
	ioport_set_pin_level(LED7R_GPIO,LED_INACTIVE_LEVEL);
	ioport_set_pin_dir(LED8R_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED8G_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(LED8G_GPIO,LED_INACTIVE_LEVEL);	
	ioport_set_pin_level(LED8R_GPIO,LED_INACTIVE_LEVEL);
	/* Bot heating segment (FETs) */
	ioport_set_pin_dir(BOT_HEAT_SEGMENT_1_GPIO, BOT_HEAT_SEGMENT_INACTIVE_LEVEL);
	ioport_set_pin_dir(BOT_HEAT_SEGMENT_2_GPIO, BOT_HEAT_SEGMENT_INACTIVE_LEVEL);
	ioport_set_pin_dir(BOT_HEAT_SEGMENT_3_GPIO, BOT_HEAT_SEGMENT_INACTIVE_LEVEL);
	ioport_set_pin_dir(BOT_HEAT_SEGMENT_4_GPIO, BOT_HEAT_SEGMENT_INACTIVE_LEVEL);
	/* Configure TWIHS0 pin */
	ioport_set_pin_peripheral_mode(TWIHS0_DATA_GPIO, TWIHS0_DATA_FLAGS);
	ioport_set_pin_peripheral_mode(TWIHS0_CLK_GPIO, TWIHS0_CLK_FLAGS);

	/* Configure TWIHS1 pin */
	/* Enable PB4 and PB5 since these pins are system io (ref. Datasheet p.123) */
	MATRIX->CCFG_SYSIO |= CCFG_SYSIO_SYSIO4;
	MATRIX->CCFG_SYSIO |= CCFG_SYSIO_SYSIO5;

	ioport_set_pin_peripheral_mode(TWIHS1_DATA_GPIO, TWIHS1_DATA_FLAGS);
	ioport_set_pin_peripheral_mode(TWIHS1_CLK_GPIO, TWIHS1_CLK_FLAGS);

	/* Configure TWIHS2 pin */
	ioport_set_pin_peripheral_mode(TWIHS2_DATA_GPIO, TWIHS2_DATA_FLAGS);
	ioport_set_pin_peripheral_mode(TWIHS2_CLK_GPIO, TWIHS2_CLK_FLAGS);


	/* Configure ethernet switch pins */
	ioport_set_pin_dir(PIN_88E6320_RSTN, IOPORT_DIR_OUTPUT);
	
	ioport_set_pin_level(PIN_88E6320_RSTN, PIN_88E6320_RSTN_INACTIVE_LEVEL);
	ioport_set_pin_dir(PIN_88E6320_INT, IOPORT_DIR_INPUT);
	ioport_set_pin_peripheral_mode(PIN_88E6320_TXC_IDX,  PIN_88E6320_TXC_FLAGS);
	ioport_set_pin_peripheral_mode(PIN_88E6320_TXEN_IDX, PIN_88E6320_TXEN_FLAGS);
	ioport_set_pin_peripheral_mode(PIN_88E6320_TXD1_IDX, PIN_88E6320_TXD1_FLAGS);
	ioport_set_pin_peripheral_mode(PIN_88E6320_TXD0_IDX, PIN_88E6320_TXD0_FLAGS);
	ioport_set_pin_peripheral_mode(PIN_88E6320_RXD1_IDX, PIN_88E6320_RXD1_FLAGS);
	ioport_set_pin_peripheral_mode(PIN_88E6320_RXD0_IDX, PIN_88E6320_RXD0_FLAGS);
	//ioport_set_pin_peripheral_mode(PIN_88E6320_RXDV_IDX, PIN_88E6320_RXDV_FLAGS); /* Used as TX_CTL in RGMII mode */
	ioport_set_pin_peripheral_mode(PIN_88E6320_MDC_IDX,  PIN_88E6320_MDC_FLAGS);
	ioport_set_pin_peripheral_mode(PIN_88E6320_MDIO_IDX, PIN_88E6320_MDIO_FLAGS);
	delay_ms(10);
}
