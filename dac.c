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

#include "asf.h"
#include "stdio_serial.h"
#include "twihs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "user_board.h"
#include "port.h"
#include "dac.h"
#include "FreeRTOSuser.h"
#include "printd.h"
#include "pio.h"
#include "task_defs.h"
#include "twi.h"
#include <math.h>
//#include <tgmath.h>

twihs_packet_t packet_tx, packet_rx;
twihs_options_t opt;

twihs_options_t opt2;
twihs_packet_t packet2_tx, packet2_rx;

#define TEST_DATA_LENGTH  (3/ sizeof(uint8_t);

void task_protect_dac(void *pvParameters);
uint16_t send_package(uint8_t cabyte,uint8_t msbyte,uint8_t lsbyte);
void TWIHS0_init(void);
void TWIHS2_init(void);
void turn_dac_channel_off (int i,int check);
void read_dac_value(uint8_t i);
void calculate_dac_value(int i);
void init_temp_sensors(void);

struct DAC_def dac = {
	.heatsink_temp = {0,0},
	.mosfet_temp = {0,0},
	.sensor_thresholdL	= {70,70,70,70,70,70,70,70}, // threshold for temperature value is getting hot can be modifier for each segment separately.
	.sensor_thresholdM	= {80,80,80,80,80,80,80,80}, // threshold for temperature value is hot can be modifier for each segment separately.
	.sensor_thresholdH	= {105,105,105,105,105,105,105,105}, // threshold for temperature value is way to hot can be modifier for each segment separately.
	.dac_on_or_off		= {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 }, // 0 DAC is off / 1 DAC is on
	.selected_dac		= {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 }, // what DAC are selected to change power
	.selected_power		= {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 }, // the selected power for each DAC
	.selected_bottom	= {0 ,0 ,0 ,0 },
	.max_power = MAX_POWER_TOTAL,
};

struct DAC_def *DAC = &dac;

struct DAC_def *dac_def_get(void) {
	return (DAC);
}

static void mdelay(uint32_t ul_dly_ticks)
{
	uint32_t ul_cur_ticks;

	ul_cur_ticks = 0;
	while ((0 - ul_cur_ticks) < ul_dly_ticks);
}

void init_dactwi(void *pvParameters)
{	
	/* turn all DACs off */
	send_package(POWER_DOWN_REGISTER,POWER_DOWN_2BYTE,POWER_DOWN_1BYTE);

	/* update all DACs */
	send_package(broadcoastupdate,0x00,0x00);
	
	/* init the temp sensors */
	init_temp_sensors();
	/* Here would normaly be read from eeprom but it does not exist atm since eeprom does not work. */
	/* The DAC saves on SW resets its value so we read it out with this and save it in our strucutre */
	for (int i = 0 ; i< AMOUNT_DAC_CHANNELS;i++){
		read_dac_value(i);
	}
	TSPAWN("DAC_protect_TASK", TASK_PROTECT_DAC_PRIORITY+1, TASK_PROTECT_DAC_STACK_SIZE, task_protect_dac, NULL, NULL); 
	printf("DAC init done: \n");
}
/* Task which controls the LEDs and checks if the temp value is at the limit compared to our thresholds */
void task_protect_dac(void *pvParameters){
	/* Start endless loop for task */
	while(1){
		/* for loop for each temperature sensor we have */
		for(int i = 0; i< AMOUNT_TEMP_SENSORS;i++){
			/* This if is checking if we ever turned on the segment we want to check. If not we turn the LEDs off and dont bother collecting information from temperature sensor */
			if (DAC->dac_on_or_off[i] == 1){
				/* collecting and saving the temperature value into DAC structure */
				get_tempvalue(i);
				uint8_t temp_ = DAC->mosfet_temp[0]; // temperature value of sensor i
				uint8_t temp_l = DAC->sensor_thresholdL[i]; // low threshold for temperature
				uint8_t temp_m = DAC->sensor_thresholdM[i]; // medium threshold for temperature
				uint8_t temp_h = DAC->sensor_thresholdH[i]; // high threshold for temperature
				
				/* This if / else checks if the temperature is in the thresholds area and turns their LED on. The LED is a bi-color led meaning green/red is turned on/off */
				if(temp_ < temp_m && temp_ >= temp_l){
					ioport_set_pin_level(LEDG_GPIO[i], LED_ACTIVE_LEVEL);
					ioport_set_pin_level(LEDR_GPIO[i], LED_ACTIVE_LEVEL);				
				}
				else if(temp_ < temp_h && temp_ >= temp_m){
					ioport_set_pin_level(LEDG_GPIO[i], LED_INACTIVE_LEVEL);
					ioport_set_pin_level(LEDR_GPIO[i], LED_ACTIVE_LEVEL);
					if(DAC->dac_on_or_off[i] == 1 && temp_ <= temp_h){
 						turn_dac_channel_on(i);
  					}
				}
				else if(temp_ >= temp_h){
					ioport_set_pin_level(LEDG_GPIO[i], LED_INACTIVE_LEVEL);
					pio_toggle_pin(LEDR_GPIO[i]);
					/* If we entered this else if it means that temperature is too high and the channel is turned off to reduce load ( reducing temperature) */
					turn_dac_channel_off(i,0);
				}
				else{
					ioport_set_pin_level(LEDR_GPIO[i], LED_INACTIVE_LEVEL);
					ioport_set_pin_level(LEDG_GPIO[i], LED_ACTIVE_LEVEL);
				}
			}
			else{
				ioport_set_pin_level(LEDR_GPIO[i], LED_INACTIVE_LEVEL);
				ioport_set_pin_level(LEDG_GPIO[i], LED_INACTIVE_LEVEL);
			}
		}
		/* delay for task ( we go though the for loop every 200ms) */
		TSLEEP(200);
	}
}
/* calculates the lest significant byte for our package */
uint8_t calcls(double power) {	
	double tempvalue; //  What value the DAC needs to have for wished W
	double ac = ampere_coefficient; // ampere coefficient
	double vc = voltage_coefficient; // voltage coefficient
	tempvalue = power/(ac*1000*12*((1-vc)/1000)); // calculate temporary DAC value .
	uint16_t ttempvalue = tempvalue; // get rid of after coma
	uint16_t value = (ttempvalue<<4); 
	uint16_t lsbyte = value & 0xFF; // get rid of most significant data byte
	return lsbyte;
}
/* calculates the most significant byte for our package */
uint8_t calcms(double power) {	
	double tempvalue; //  What value the DAC needs to have for wished W
	double ac = ampere_coefficient; // ampere coefficient
	double vc = voltage_coefficient; // voltage coefficient
	tempvalue = power/(ac*1000*12*((1-vc)/1000)); // calculate temporary DAC value .
	uint16_t ttempvalue = tempvalue; // get rid of after coma
	uint16_t value = (ttempvalue<<4); 
	uint16_t msbyte = (value>>8); // get rid of least significant data byte
	return msbyte;
}
/* findaddress finds the first package for I2C which defines what segment (channel) we want to send a message to */
uint8_t findaddress(uint8_t segments[8],uint8_t value,uint8_t lenght ) {
	 int i = 0;
	 int size = lenght;
	 if(value == 0) {
		 i = 0;
	 }
	 else {
		 i = value-1;
		 size = i+1;
	 }
	 int temp = 0x00;
	/* calculates the address for shutdown */ 
	 while (i < size && value == 0) {
		 switch(segments[i]) {
			 case 0:
			 temp += 0x01;
			 break;
			 case 1:
			 temp += 0x02;
			 break;
			 case 2:
			 temp += 0x04;
			 break;
			 case 3:
			 temp += 0x08;
			 break;
			 case 4:
			 temp += 0x10;
			 break;
			 case 5:
			 temp += 0x20;
			 break;
			 case 6:
			 temp += 0x40;
			 break;
			 case 7:
			 temp += 0x80;
			 break;
			 default:
			 break;
		 }
		 i = i + 1;
	 }
	 /* calculates the address for changing the channels data */ 
	 while (i < size && value != 0) {
		 switch(segments[i]) {
			 case 0:
			 temp += 0x00;
			 break;
			 case 1:
			 temp += 0x01;
			 break;
			 case 2:
			 temp += 0x02;
			 break;
			 case 3:
			 temp += 0x03;
			 break;
			 case 4:
			 temp += 0x04;
			 break;
			 case 5:
			 temp += 0x05;
			 break;
			 case 6:
			 temp += 0x06;
			 break;
			 case 7:
			 temp += 0x07;
			 break;
			 default:
			 break;
		 }
		 i = i + 1;
	 }
	 return temp;
}
/* send I2C pacakge to the DAC. */
uint16_t send_package(uint8_t cabyte,uint8_t msbyte,uint8_t lsbyte)
{
	uint8_t data[] = {cabyte,msbyte,lsbyte};
	packet2_tx.chip        = DAC_ADDRESS;
	packet2_tx.buffer      = (uint8_t *) data;
	packet2_tx.length      = 3;
	/* Send test pattern to EEPROM */
	if (twihs_master_write(TWI2_BASE, &packet2_tx) != TWIHS_SUCCESS) {
		puts("-E-\t DAC TWI master write packet failed.\r");
	}
}
/* gets the tempearture value from ADDRESS */
void get_tempvalue(uint8_t i) {
	/** Reception buffer */
	uint8_t address = TEMP_SENSOR_ADDRESS[i];
	uint16_t gs_uc_test_data_rx[2] = {0};
	memset(gs_uc_test_data_rx,0x00,sizeof(gs_uc_test_data_rx));
	packet_rx.chip       = address;
  	packet_rx.buffer      = gs_uc_test_data_rx;
  	packet_rx.length      = 2;
	/* Get data from TEMP_SENSOR */
	if (twihs_master_read(TWI0_BASE, &packet_rx) != TWIHS_SUCCESS) {
		printf("-E-\t get_tempvalue failed.\r");
	}
	
	/* Calculates temperature value to decimal */
	/* Convert to right endian */
	uint16_t buff = ((gs_uc_test_data_rx[0]<< 8) | (gs_uc_test_data_rx[0] >> 8));
	float tmp_value;
	uint32_t buffer;
	uint32_t beforecomma;
	uint32_t aftercomma;
	uint32_t *returnbuf[2] = {0,0};
	buffer = buff;
	/* shifts the buffer since the first bits are unused*/
	buffer = buffer >> shift_for_12bit;
	/* calculates the complete value of C */
	tmp_value = ((float)(buffer)*value_per_resolution_12bit);
	/* save heatsink value */
	beforecomma = floor(tmp_value);
	/* 100 for 2 comma digits, Takes the after comma values and saves them */
	aftercomma = (uint32_t)((tmp_value-beforecomma)*100);
	DAC->heatsink_temp[0] = beforecomma;
	DAC->heatsink_temp[1] = aftercomma;
	
	/* calculates via trend line ( progression ) the temp value to a closer real value */
	float correct_value = 0;
	/* calculate regression line values. */
	float power_value_2 =reg2*pow(tmp_value,2);
	float power_value_1 =reg1*tmp_value;
	float final_value = 0 ;
	if(DAC->dac_on_or_off[i] == 0 || DAC->selected_power[i] == 0){
		final_value = tmp_value;
	}
	else{
		final_value = power_value_2+power_value_1-reg_offeset;
	}
	/* takes before comma values and saves them */
	beforecomma = floor(final_value);
	/* 100 for 2 comma digits, Takes the after comma values and saves them */
	aftercomma = (uint32_t)((final_value-beforecomma)*100);
	/* adds values to structure */
	//printd("beforecomma %u, end: %u\n",beforecomma, (uint32_t)tmp_value);
	DAC->mosfet_temp[0] = beforecomma;
	DAC->mosfet_temp[1] = aftercomma;
}
/* turns i DAC off where i is 0 to 7 (1 to 8 in segments) */
void turn_dac_channel_off (int i,int check){
	uint8_t fakearray[1] = {i};
	/* checks if turning of was because of overheating or general turning off*/ 
	if (check == 1){	
		DAC->dac_on_or_off[i] = 0;
	}
	uint16_t temp = findaddress(fakearray,0,1);
	uint16_t value = (temp<<5); //shift since the first 5 bits are unused 
	uint16_t msbyte = (value>>8)| DAC_SHUTDOWN_100k_PULLDOWN;; // get rid of least significant data byte
	uint16_t lsbyte = value & 0xFF; // get rid of most significant data byte
	// turn off selected segment
	send_package(POWER_DOWN_REGISTER,msbyte,lsbyte);
}
/* turns the DAC channel on where i is 0 to 7 (1 to 8 in segments) */
void turn_dac_channel_on (int i){
	uint8_t fakearray[1] = {i};	
	DAC->dac_on_or_off[i] = 1;
	int temp = findaddress(fakearray,0,1);
	uint16_t value = (temp<<5); // shift since the first 5 bits are unused 
	uint16_t msbyte = (value>>8)| DAC_TURNON; // get rid of least significant data byte
	uint16_t lsbyte = value & 0xFF; // get rid of most significant data byte
	DAC->dac_on_or_off[i] = 1;
	send_package(POWER_DOWN_REGISTER,msbyte,lsbyte);
}
/* reads the dac value of i */
void read_dac_value(uint8_t i){
	uint8_t fakearray[1] = {i};
	uint8_t temp_buff = findaddress(fakearray,1,1);
	uint16_t gs_uc_test_data_rx[2] = {0};
	memset(gs_uc_test_data_rx,0x00,sizeof(gs_uc_test_data_rx));
	packet2_rx.chip        = DAC_ADDRESS;
	packet2_rx.buffer      = gs_uc_test_data_rx;
	packet2_rx.length      = 3;
	packet2_rx.addr[0]     = temp_buff | READ_DAC_REGISTER;
	packet2_rx.addr_length = 1;
	/* Get data from DAC for the specific address */
	if (twihs_master_read(TWI2_BASE, &packet2_rx) != TWIHS_SUCCESS) {
		printf("-E-\t DAC TWI master read packet failed.\r");
	}

	uint16_t buff = ((gs_uc_test_data_rx[0]<< 8) | (gs_uc_test_data_rx[0] >> 8));
	DAC->dac_value_hex[i] = buff;
	calculate_dac_value(i);
	
	
}
/* translate dac value from hex to decimal */
void calculate_dac_value(int i){
	/* calculates from hex to int */
	uint16_t buff;
	uint16_t power = DAC->dac_value_hex[i] >> 4;
	double ac = ampere_coefficient; // ampere coefficient
	double vc = voltage_coefficient; // voltage coefficient
	/* calculate temporary DAC value the 0.5 is for rounding up since calculation eror occurs */
	buff = power*(ac*1000*12*((1-vc)/1000))+0.5;
	DAC->selected_power[i] = buff;
}
 /* changes the resolution on the TEMP sensors from 9 bit to 12bit*/
void init_temp_sensors(void){
	uint8_t data[1] = {0};
	for (int i = 0; i<AMOUNT_TEMP_SENSORS ;i++){

		delay_ms(100);
		data[0] = 0x01;
		packet_tx.chip        = TEMP_SENSOR_ADDRESS[3];
		packet_tx.length = 1;
		if (twihs_master_write(TWI0_BASE, &packet_tx) != TWIHS_SUCCESS) {
			puts("-E-\t TEMP_CONFIG change pointer failed.\r");
		}
		delay_ms(20);
		data[0] = set_to_12bit_resolution;
		packet_tx.length = 1;
		if (twihs_master_write(TWI0_BASE, &packet_tx) != TWIHS_SUCCESS) {
			puts("-E-\t TEMP_CONFIG change resolution failed.\r");
		}
		delay_ms(20);
		data[0] = 0x00;
		packet_tx.length = 1;
		if (twihs_master_write(TWI0_BASE, &packet_tx) != TWIHS_SUCCESS) {
			puts("-E-\t TEMP_CONFIG change back pointer failed.\r");
		}
	}
}