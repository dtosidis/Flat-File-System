/* Pairnei ws orismata dyo onomata arxeiwn. To ena thewreite
 * panta topiko arxeio UNIX kai to allo panta arxeio tou dikou
 * mas filesystem. 
 * Antigrafei ta periexomena tou prwtou arxeiou sto deytero */
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
		int src_file = 0, dst_file = 0;
		// Diavazoume se vimata BLOCK_SIZE, opote
		// desmeyoume antistoixi mnimi
		char *buffer = (char*)malloc(BLOCK_SIZE);
		int bytes = 0, pos = 0;

		printf("Copying %s to %s\n", argv[1], argv[2]);

		// Epixeiroume na anoiksoume to prwto arxeio ws
		// arxeio tou dikou mas filesystem.
		src_file = my_open(argv[1]);

		if (src_file)
		{
			// To arxeio yparxei sto diko mas FS opote to
			// antigrafoume sto UNIX

			// Anoigoume ena arxeio me onoma to deytero orisma.
			// An den yparxei to dimiourgoume.
			dst_file = open(argv[2], O_WRONLY|O_CREAT, S_IWUSR|S_IRUSR);

			for (;;)
			{
				// Read BLOCK_SIZE bytes
				bytes = my_read(src_file, buffer, BLOCK_SIZE, pos);
				if (bytes > 0)
				{
					// Write to (unix) file
					write(dst_file, buffer, bytes);
					// Proceed
					pos += bytes;
					bzero(buffer, BLOCK_SIZE);
				}
				else break;
			}
		}
		else 
		{
			// Check for unix file
			src_file = open(argv[1], O_RDONLY);
			if (src_file)
			{
				// To arxeio yparxei sto UNIX opote to
				// antigrafoume sto diko mas FS

				// Anoigoume ena arxeio me onoma to deytero orisma.
				// Kaloume tin my_create wste na diagrapsoume ta periexomena
				// tou an yparxei idi i na to dimiourgisoume an
				// den yparxei.
				my_create(argv[2]);
				dst_file = my_open(argv[2]);

				for (;;)
				{
					// Read BLOCK_SIZE bytes
					bytes = read(src_file, buffer, BLOCK_SIZE);
					if (bytes > 0)
					{
						// write to (our) file
						my_write(dst_file, buffer, bytes, pos);
						pos += bytes;
						bzero(buffer, BLOCK_SIZE);
					}
					else break;
				}

			}
		}
	}
	return 0;
}

