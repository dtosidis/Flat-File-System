/* afairesh arxeiou apo ton disko */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "filesystem.h"

int main(int argc, char *argv[])
{
	//afairesh tou arxeiou xrhsimopoiwntas thn my_delete
	if( my_delete( argv[ 1 ] ) == 0 ) {
        printf("File %s was deleted successfully\n");
        return 0;
	}
    printf("Error deleting file\n");	//an uparksei lathos enhmerwse
	return -1;				//eksodos

}
