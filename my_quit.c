/* termatizei to kelufos mono an exei ginei unmound to file system  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "filesystem.h"

int main (int argc, char * const argv[])
{	
	int um = atoi(argv[1]);
	if(um == 1)						//elegxos an to system file einai unmounted
		printf("Unmount file system first\n");
	else
	{
		printf("Terminating program");				//an einai unmounted ektupwnei to mhnuma
		exit(0);						//kai termatizei to kelufoa
	}
}
