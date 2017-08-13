/* ektypwnei mia lista me ta arxeia 
 * tou systimatos */
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
	// Diavazoume ton ari8mo twn arxeiwn.
	int file_count = my_file_count(), i = 0;

	// Desmeyoume xwro sti mnimi gia ton pinaka twn arxeiwn.
	char **filenames = (char**)calloc(file_count, sizeof(char*));
	// Gia ola ta arxeia
	while (i < file_count)
	{
		// Desmeyoume xwro gia ton pinaka twn onomatwn
		filenames[i] = (char*)malloc(MAX_FILENAME);
		++i;
	}

	i = 0;

	// Kaloume tin my_files pou "simplirwnei" ton pinaka twn arxeiwn.
	my_files(filenames);

	printf("File listing for %s (%d total):\n", filesystem_file, file_count);

	// Diatrexoume ola ta arxeia
	while (i < file_count)
	{
		// Anoigoume to kathe arxeio gia na diavasoume to mikos
		// tou.
		int file_id = my_open(filenames[i]), file_size = my_file_size(file_id);
		// Typwnoume to mikos tou se bytes, to id tou kai to onoma tou.
		printf("\t%d. \"%s\" (%d bytes)\n", file_id, filenames[i], file_size);
		++i;
	}

	free(filenames);

	return 0;
}
