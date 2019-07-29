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

#include "port.h"

void init_dactwi();
uint8_t calcms(double power);
uint8_t calcls(double power);
uint16_t send_package(uint8_t cabyte,uint8_t msbyte,uint8_t lsbyte);
uint8_t findaddress(uint8_t segments[8],uint8_t value,uint8_t lenght);
void printsegment(uint8_t segments[8]);
void get_tempvalue(uint8_t address);
void read_dac_value(uint8_t i);
void turn_dac_channel_on (int i);
void turn_dac_channel_off (int i,int check);


#define TEMP_SENSOR8_ADDRESS     0x48
#define TEMP_SENSOR7_ADDRESS     0x49
#define TEMP_SENSOR6_ADDRESS     0x4A
#define TEMP_SENSOR5_ADDRESS     0x4B
#define TEMP_SENSOR4_ADDRESS     0x4C
#define TEMP_SENSOR3_ADDRESS     0x4D
#define TEMP_SENSOR2_ADDRESS     0x4E
#define TEMP_SENSOR1_ADDRESS     0x4F

#define MAX_POWER_PER_SEGMENT 15
#define MAX_POWER_TOTAL 80
#define LCD_DISPLAY_ADDRESS 0x3D
#define AMOUNT_DAC_CHANNELS 0x08
#define POWER_DOWN_REGISTER 0x40
#define POWER_DOWN_2BYTE 0x5F
#define POWER_DOWN_1BYTE 0xE0
#define DAC_ADDRESS 0x48
#define DAC_SHUTDOWN_1k_PULLDOWN 0x20;
#define DAC_SHUTDOWN_100k_PULLDOWN 0x40
#define DAC_TURNON 0x00
#define READ_DAC_REGISTER 0x10
#define updatecommand 0x10 // updates DAC
#define writecommand 0x00 // writes to DAC
#define writeandupdatecommand 0x30// writes to specific DAC and update it
#define writeandupdateall 0x20
#define broadcoastupdate 0x1F
#define broadcoastmode 0x3F // set value to the all DAC and update these.
/* This value are different in the real software !!! */
#define ampere_coefficient 5
/* This value are different in the real software !!! */
#define voltage_coefficient 3
/* 9bit resolution default */
#define value_per_resolution_9bit 0.5
#define shift_for_9bit 7
#define amount_bot_segments 4
/* 12bit resolution set to */
#define value_per_resolution_12bit 0.0625
#define shift_for_12bit 4

#define sign_of_temp 0x8000 // to get rid of the first bit if its negative. 

/* commands for temperature sensor */
#define set_to_12bit_resolution 0x6000
#define address_configuration_register 0x01;

/* trend line / regression values*/
/* This values are different in the real software !!! */
 #define reg6 1 /* to the power of 6 */
 #define reg5 2
 #define reg4 3
 #define reg3 4
 #define reg2 5
 #define reg1 6
 #define reg_offeset 7


struct DAC_def {
	uint8_t heatsink_temp[2]; // saves the sensor value of the asked sensor ( [0] before comma- [1] after comma values)
	uint8_t mosfet_temp[2];
	uint8_t sensor_thresholdL[8]; // low threshold
	uint8_t sensor_thresholdM[8]; // medium threshold
	uint8_t sensor_thresholdH[8]; // limit threshold
	uint8_t dac_on_or_off[8]; // if dac channel is online or offline
	uint16_t dac_value_hex[8]; // dac value in hex 
	uint8_t selected_dac[8]; // what DAC channels are selected
	uint8_t selected_power[8]; // what power the uses selected for each segment
	uint8_t selected_bottom[4]; // if bottom segment is on or off
	uint8_t max_power;
};

/* array for GPIO of LED color red */
static const uint32_t LEDR_GPIO[] = {
	LED1R_GPIO,
	LED2R_GPIO,
	LED3R_GPIO,
	LED4R_GPIO,
	LED5R_GPIO,
	LED6R_GPIO,
	LED7R_GPIO,
	LED8R_GPIO,
};
/* array for GPIO of LED color green */
static const uint32_t LEDG_GPIO[] = {
	LED1G_GPIO,
	LED2G_GPIO,
	LED3G_GPIO,
	LED4G_GPIO,
	LED5G_GPIO,
	LED6G_GPIO,
	LED7G_GPIO,
	LED8G_GPIO,
};
/* temperature sensor address array */
static const uint32_t TEMP_SENSOR_ADDRESS[] = {
	TEMP_SENSOR1_ADDRESS,
	TEMP_SENSOR2_ADDRESS,
	TEMP_SENSOR3_ADDRESS,
	TEMP_SENSOR4_ADDRESS,
	TEMP_SENSOR5_ADDRESS,
	TEMP_SENSOR6_ADDRESS,
	TEMP_SENSOR7_ADDRESS,
	TEMP_SENSOR8_ADDRESS,
};
#define AMOUNT_TEMP_SENSORS 8