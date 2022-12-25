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

#if 0
// For testing
int main(int argc, char *argv[])
{
	char szHost[128];
	gethostname(szHost, sizeof(szHost));
	printf("The host name is %s\n", szHost);
	return 0;
}
#endif

// Monitor Thread
pthread_t	CLDRN_Mon_ID;
void	*CLDRN_Monitor_Thread(void *arg)
{

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
