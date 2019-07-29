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
* Author      : Tom Welbers
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
#include "stdio_serial.h"
#include "twi.h"
#include "ethernet.h"
#include "FreeRTOSuser.h"
#include "task_defs.h"
#include "dac.h"
#include "shell.h"


/*****************************************************************************/
/*  PRIVATE MACROS                                                           */
/*****************************************************************************/


/*****************************************************************************/
/*  EXTERNAL REFERENCES                                                      */
/*****************************************************************************/
extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName);
extern void vApplicationTickHook(void);
extern void vApplicationIdleHook(void);
extern void vApplicationMallocFailedHook(void);


/*****************************************************************************/
/*  PRIVATE FUNCTION PROTOTYPES                                              */
/*****************************************************************************/
static void configure_console(void);
static void task_heartbeat(void *pvParameters);


/*****************************************************************************/
/*  PRIVATE GLOBALS                                                          */
/*****************************************************************************/


/*****************************************************************************/
/*  PUBLIC FUNCTION DEFINITIONS                                              */
/*****************************************************************************/

/*---------------------------------------------------------------------------*
Description:	Main program
Parameters:		none
Return:			none
*-----------------------------------------------------------------------------*/
int main(void)
{
	/* Disable watchdog timer */
	WDT->WDT_MR = WDT_MR_WDDIS;

	sysclk_init();
	io_init();
	configure_console();
	printf("\r\n-----------------------------------------\r\n");
	printf("Console: OK!\r\n");

	/* Initialize TWI devices */
	twi0_init();
	twi1_init();
	twi2_init();

	printf("TWI DONE\n");
	/* Initialize DAC*/
	init_dactwi();

	/* Initialize shell */
	shell_init();
	
	/* Scan for twi slaves */
 	twi_scan_bus(TWI0_BASE);
 	twi_scan_bus(TWI1_BASE);

	/* Initialize interface and lwip stack */
	ethernet_init();

	TSPAWN("HEARTBEAT", TASK_HEARBEAT_PRIORITY, TASK_HEARBEAT_STACK_SIZE, task_heartbeat, &task_handle.task_heartbeat, NULL);
	/* Start the scheduler. */
	vTaskStartScheduler();

	/*
	 * If all is well, the scheduler will now be running, and the following
	 * line will never be reached.  If the following line does execute, then
	 * there was insufficient FreeRTOS heap memory available for the idle
	 * and/or timer tasks to be created.
	 * See the memory management section on the FreeRTOS web site for more
	 * details.
	 */
	return (0);
}


/*****************************************************************************/
/*  PRIVATE FUNCTION DEFINITIONS                                             */
/*****************************************************************************/

/*---------------------------------------------------------------------------*
Description:	Perform heartbeat signal to AVR in order to detect a crash
Parameters:		none
Return:			none
*-----------------------------------------------------------------------------*/
static void task_heartbeat(void *pvParameters)
{
	printf(" TSPAWN: task_heartbeat.\r\n");

	init_status.task_heartbeat = true;

	while (1) {
		TSLEEP(1000);
		ioport_toggle_pin_level(PIN_SAM_HEARTBEAT);
	}

	TKILL(NULL);
}


/*---------------------------------------------------------------------------*
Description:	Configure UART0 to use stdio
Parameters:		none
Return:			none
*-----------------------------------------------------------------------------*/
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.charlength = CONF_UART_CHAR_LENGTH,
		.paritytype = CONF_UART_PARITY,
		.stopbits = CONF_UART_STOP_BITS,
	};
	/* Configure UART console. */
	sysclk_enable_peripheral_clock(ID_UART0);
	stdio_serial_init(UART0, &uart_serial_options);
	setbuf(stdout, NULL);
}

void UART0_Handler(void)
{
	uint8_t c = 0;
	uint32_t sr = uart_get_status(UART0);
	if (sr & US_CSR_RXRDY)
	{
		usart_read(UART0, &c);
		if (c != 0)
		{
			shell_input(c);
		}
	}
}

/*---------------------------------------------------------------------------*
Description:	Called if stack overflow during execution
Parameters:
Return:			none
*-----------------------------------------------------------------------------*/
extern void vApplicationStackOverflowHook(xTaskHandle *pxTask,
		signed char *pcTaskName)
{
	printf("stack overflow %x %s\r\n", pxTask, (portCHAR *)pcTaskName);
	/*
	 * If the parameters have been corrupted then inspect pxCurrentTCB to
	 * identify which task has overflowed its stack.
	 */
	for (;;) {}
}


/*---------------------------------------------------------------------------*
Description:	This function is called by FreeRTOS idle task
Parameters:
Return:			none
*-----------------------------------------------------------------------------*/

extern void vApplicationIdleHook(void)
{
}

/*---------------------------------------------------------------------------*
Description:	This function is called by FreeRTOS each tick
Parameters:
Return:			none
*-----------------------------------------------------------------------------*/

extern void vApplicationTickHook(void)
{
}

/*---------------------------------------------------------------------------*
Description:Called if a call to pvPortMalloc() fails because there is insufficient
			free memory available in the FreeRTOS heap.  pvPortMalloc() is called
			internally by FreeRTOS API functions that create tasks, queues, software
			timers, and semaphores.  The size of the FreeRTOS heap is set by the
			configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h
Parameters:
Return:			none
*-----------------------------------------------------------------------------*/
extern void vApplicationMallocFailedHook(void)
{
	/* Force an assert. */
	configASSERT( ( volatile void * ) NULL );
}
