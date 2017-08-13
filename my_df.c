/* Epistrefei ton ari8mo twn eley8erwn blocks 
 * kai ta sinolika block */
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
	block b;
	filesystem fs;

	// Anoigoume to arxeio tou systimatos.
	int fs_descr = open(filesystem_file, O_RDONLY), block_pos = 0, free = 0;

	// Elegxoume gia epitixes anoigma.
	if (fs_descr == -1)
	{
		perror("open");
	}

	// Diavazoume ta dedomena tou filesystem pou vriskontai
	// stin arxi tou arxeiou (sto deytero block).
	lseek(fs_descr, sizeof(block), SEEK_SET);
	read(fs_descr, &fs, sizeof(filesystem));

	// Typwnoume ton ari8mo twn sinolikwn blocks.
	printf("Total blocks: %d (block size: %d bytes)\n", fs.blocks, fs.block_size);

	// Psaxnoume ena ena ta blocks gia na broume
	// ta eley8era.
	while (block_pos < fs.blocks*fs.block_size)
	{
		// Pigainoume sto swsto simeio tou arxeiou
		lseek(fs_descr, block_pos, SEEK_SET);
		// Kai diavazoume ta stoixeia tou block
		// pou brisketai ekei.
		read(fs_descr, &b, sizeof(block));

		if (!b.length)
		{
			// An to mikos tou block (diladi ta eggegramena ekei
			// stoixeia) einai miden, tote to block thewreite
			// eley8ero.
			free += 1;
		}

		// Proxwrame parakatw sto arxeio tou systimatos.
		block_pos += fs.block_size;
	}

	// Typwnoume ton ari8mo twn eley8erwn blocks.
	printf("Free blocks: %d\n", free);

	return 0;
}
