/**
 **  RPiCauldron.cpp
 **  
 **  Need to create a simple but good debug logging system!
 **  
 ***/
#include <iostream>

#include <pthread.h>

using namespace std;
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

// For I2C 
#include <stdint.h>
#include <time.h>
#include <sys/ioctl.h>
#include "linux/i2c-dev.h"

// from tutorial on semaphores:
#include <semaphore.h>
#include <unistd.h>
  
sem_t mutex;
  
void* thread(void* arg)
{
	    //wait
	    sem_wait(&mutex);
	    printf("\nEntered..\n");
	  
	    //critical section
	    sleep(4);
	      
	    //signal
	    printf("\nJust Exiting...\n");
	    sem_post(&mutex);
}
  
  
int main_g()
{
	    sem_init(&mutex, 0, 1);
	    pthread_t t1, t2;
	    pthread_create(&t1, NULL, thread, NULL);
	    sleep(2);
	    pthread_create(&t2, NULL, thread, NULL);
	    pthread_join(t1, NULL);
	    pthread_join(t2, NULL);
	    sem_destroy(&mutex);
	    return 0;
}

sem_t MainMutex;

// * * * *  * * * *  * * * * * * * * * * * *
// WARNING!
// The temperature probe is OneWire, NOT i2c - it might use the same i2c API
// Must check into
// * * * *  * * * *  * * * * * * * * * * * *



// Monitor Thread
pthread_t	CLDRN_Mon_ID;
void	*CLDRN_Monitor_Thread(void *arg)
{
	int fd;
	struct timespec _500ms;
	_500ms.tv_sec = 0;
	_500ms.tv_nsec = 5000000L;
	
	sem_wait(&MainMutex);
	
	printf("Starting CLDRN_Monitor_Thread...\n");
	
	printf("opening dev i2c... \n");
	
	fd = open("/dev/i2c-1", O_RDWR);
	if (fd < 0) 
	{
		fprintf(stderr, "Error opening device   errno = %d, strerror = %s\n", errno, strerror(errno));
	    exit(EXIT_FAILURE);
	}
	
	printf("setting slave addr for i2c... \n");
	
	if(ioctl(fd, I2C_SLAVE, 0x20) < 0) 
	{
		fprintf(stderr, "Error setting slave address\n");
		close(fd);
		exit(EXIT_FAILURE);
	}

	char buf[10];
	
	buf[0] = 0x07; // IODIR PORT1
	buf[1] = 0x00; // all output
	if (write(fd, buf, 2) != 2) {
		fprintf(stderr, "Error writing (1)\n");
		close(fd);
		exit(EXIT_FAILURE);
	}
	printf("Write success!\n");

	nanosleep(&_500ms, NULL);

	buf[0] = 0x01; // PORT1
	buf[1] = 0xff; // output high
	if (write(fd, buf, 2) != 2) {
		fprintf(stderr, "Error writing (2)\n");
		close(fd);
		exit(EXIT_FAILURE);
	}

	close(fd);
	printf("Done.\n");
	
	return NULL;
}

// Control Thread
pthread_t	CLDRN_Ctrl_ID;
void	*CLDRN_Control_Thread(void *arg)
{
	
	return NULL;
}

int	main(int argc, char *argv[])
{
	int		retval;
	std::cout << "RPiCauldron starting\n";

	retval = sem_init(&MainMutex, 0, 1);
	
	// create monitor
	retval = pthread_create(&CLDRN_Mon_ID, NULL, &CLDRN_Monitor_Thread, NULL);
	if (retval)
	{
		cout << "Unable to create Monitor Thread: retval = " << retval << "\n";
		return -1;
	}
	cout << "Monitor thread created\n";
	
	// create controller
	retval = pthread_create(&CLDRN_Mon_ID, NULL, &CLDRN_Control_Thread, NULL);
	if (retval)
	{
		cout << "Unable to create Control Thread: retval = " << retval << "\n";
		return -1;
	}
	cout << "Control thread created\n";
	
	cout << "Bye!\n";
	
	return 0;
}
