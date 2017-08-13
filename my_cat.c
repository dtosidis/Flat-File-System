/* Pairnei ws orisma to onoma enos arxeiou
 * kai typwnei ta periexomena tou */
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
	// Anoigoume to arxeio tou orismatos 1
	int file_id = my_open(argv[1]);
	if (file_id)
	{
		// An anoikse epitixws, diavazoume to megethos
		// tou kai desmeyoume analogo xwro sti 
		// mnimi gia na diavasoume ta periexomena.
		int file_size = my_file_size(file_id);
		char *buffer = (char*)malloc(file_size);

		// Kaloume tin my_read gia na diavasoume ta
		// periexomena.
		// Epistrefoume to apotelesma ston buffer
		my_read(file_id, buffer, file_size, 0);

		//Typwnoume to apotelesma.
		printf("%s", buffer);

		free(buffer);
	}

	return 0;
}
