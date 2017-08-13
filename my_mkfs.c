/* Arxikopoiei  to sistima arxeiwn kai dimiourgei
 * to arxeio tou sistimatos */
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
	if (argc >= 3)
	{
		block b;
		filesystem fs;
		int blocks = atoi(argv[1]);
		// Dimiourgoume to arxeio tou sistimatos.
		// An yparxei idi, diagrafoume ola ta stoixeia tou.
		int fs_descr = open(filesystem_file, O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
		int i = 0;

		// Diavazoume to epithimito megethos tou block.
		int block_size = atoi(argv[2]);
		
		char *buffer = (char*)malloc(block_size);

		// Epeidi kratame diafora stoixeia gia to block, 
		// stin arxi tou kathe block, prepei to block na min einai
		// poly mikro. Typwnoume to katalilo minima.
		if (block_size < 256) printf("Warning: Bigger block_size Recommended\n");
		printf("File System File: %s, %d blocks x %d bytes\n", filesystem_file, blocks, block_size);

		if (fs_descr <= 0)
		{
			// An ypirkse lathos kata ti dimiourgia tou
			// arxeiou, epistrefoume to lathos kai termatizoume
			// to programma.
			perror("open");
			return 0;
		}

		memset(buffer, 0, block_size);

		// Arxikopoioume ta blocks.
		while (i < blocks)
		{
			if (write(fs_descr, buffer, block_size) == -1)
			{
				perror("write");
			}
			++i;
		}

		// Kratame ta stoixeia tou prwtou block
		b.length = sizeof(filesystem);
		b.next_block = 0;

		// Kai grafoume to arxiko block kai tin epikefalida tou
		// sistimatos arxeiwn.
		if (lseek(fs_descr, 0, SEEK_SET) == -1)
		{
			perror("lseek");
		}

		if (write(fs_descr, &b, sizeof(block)) == -1)
		{
			perror("write");
		}

		fs.block_size = block_size;
		fs.blocks = blocks;

		if (write(fs_descr, &fs, sizeof(filesystem)) == -1)
		{
			perror("write");
		}


		close(fs_descr);
		free(buffer);

	}

	return 0;
}
