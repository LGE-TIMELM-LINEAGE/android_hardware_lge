#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <syslog.h>
#include <poll.h>
#include <assert.h>

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



int fps_init(int Fd, int value){
if (value == 1)
{
syslog(LOG_INFO,"Triggered FPS Init\n");
return ioctl(Fd, INT_TRIGGER_INIT, 3); //Activates interrupts
}
else
{
 syslog(LOG_INFO,"Closing interrupts for FPS\n");
 return ioctl(Fd, INT_TRIGGER_CLOSE, 1);    //Closes interrupts
}
}

int fps_power(int Fd, int onoff){   // Handles turning on and off of FPS. Still needs to be intialized afterwards

if (onoff == 0)
{
    printf("Powering Off FPS\n");
}
else if (onoff == 1)
{
    printf("Powering On FPS\n");
}
else
{
    printf("Error: Invalid value for device");
}
return ioctl(Fd, FP_POWER_ONOFF, (int32_t*) &onoff);
}

int fps_reset (int Fd)  //resets the FPS device
{
printf("Sensor reset\n");
ioctl(Fd, FP_SENSOR_RESET, 1);
return 0;
}

int fps_poll(int Fd){   //Sets up polling for finger
struct pollfd Fp_Poll;
Fp_Poll.fd = Fd;
ioctl(Fd, INT_TRIGGER_POLLING, 1);
int ret;
char* output;
Fp_Poll.events = ( POLLIN | POLLRDNORM | POLLOUT | POLLWRNORM);
    while ( 1 )
    {
        puts("Starting poll");
        ret = poll(&Fp_Poll, (unsigned long)1, 10000); // Poll for 10 seconds

    if( ret < 0 )
    {
      perror("poll");
      assert(0);
    }

    if( ( Fp_Poll.revents & POLLIN )  == POLLIN )
    {
      read(Fp_Poll.fd, &output, sizeof(output));
      printf("POLLIN : output = %s\n", output);
    }

    if( ( Fp_Poll.revents & POLLRDNORM)  == POLLRDNORM )
    {
      strcpy( output, "User Space");
      write(Fp_Poll.fd, &output, strlen(output));
      printf("POLLOUT : output = %s\n", output);
    }

        if( ( Fp_Poll.revents & POLLWRNORM )  == POLLWRNORM )
    {
      strcpy( output, "User Space");
      write(Fp_Poll.fd, &output, strlen(output));
      printf("POLLOUT : output = %s\n", output);
    }

    }
}
