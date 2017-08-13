/* kanei ola ta allocations pou prepei na ginoun sth kuria mnhmhkai sth sunexeia
diabazei apo ton disko sth kuria mnhmh tis basikes domes pou elegxoun
th leitourgia tou susthmatos arxeiwn */

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
	int fs_descr, i;
    fs_descr = 0;

	initialization();			//arxikopoihsh twn domwn mas.

	fs_descr = open(filesystem_file, O_RDONLY);				// anoigma tou file system
	if (fs_descr == -1)
	{
		perror("errno");				//an den anoikse, mhnuma lathous
		exit(1);
	}

	if(read(fs_descr, &directory_table, sizeof(directory_table)) < 0)		// diabasma tou directory table apo to file system
	{
		printf("Error reading directory table\n");
		close(fs_descr);						// kleisimo filesystem
        return -1;
	}

	if(read(fs_descr, &FAT, sizeof(FAT)) < 0)			// diabasma tou file allocation table apo to file system
	{
		printf("Error reading FAT\n");
		close(fs_descr);						// kleisimo filesystem
        return -1;
	}

	if(read(fs_descr, &FDT, sizeof(FDT)) < 0)			// diabasma tou File Descriptor Table apo to file system
	{
		printf("Error reading File Descriptor Table\n");
		close(fs_descr);						// kleisimo filesystem
        return -1;
	}

	if(read(fs_descr, &buffer, sizeof(buffer)) < 0)	// diabasma ths buffer list apo to file system
	{
		printf("Error reading bufferlist\n");
		close(fs_descr);						// kleisimo filesystem
        return -1;
	}

	close(fs_descr);						// kleisimo filesystem
	return 0;

}
