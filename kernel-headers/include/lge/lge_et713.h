#pragma once

// addresses
#define FP_ADDRESS_0				0x00
#define FP_WRITE_ADDRESS			0xAC
#define FP_READ_DATA				0xAF
#define FP_WRITE_DATA				0xAE

// registers
#define FDATA_FP_ADDR				0x00
#define FSTATUS_FP_ADDR				0x01
// functions
#define FP_REGISTER_READ			 1
#define FP_REGISTER_WRITE			 2
#define FP_GET_ONE_IMG				3
#define FP_GET_ONE_IMG_2				3
#define FP_SENSOR_RESET				 4
#define FP_POWER_ONOFF				5
#define FP_SET_SPI_CLOCK			6
#define FP_RESET_SET				 7

/* trigger signal initial routine*/
#define INT_TRIGGER_INIT			164
/* trigger signal close routine*/
#define INT_TRIGGER_CLOSE			165
/* read trigger status*/
#define INT_TRIGGER_READ			166
/* polling trigger status*/
#define INT_TRIGGER_POLLING		 167
/* polling abort*/
#define INT_TRIGGER_ABORT			168
#define FP_TRANSFER_SYNC			170
