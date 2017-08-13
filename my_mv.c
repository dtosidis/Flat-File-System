/* Metonomazei ena arxeio */
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
	// Anoigoume to prwto arxeio
	int fd = my_open(argv[1]);
	// An den yparxei idi arxeio me to zitoumeno onoma
	if (fd && !my_open(argv[2]))
	{
		// kaloume tin my_move me deytero orisma
		// to zitoumeno onoma
		my_move(fd, argv[2]);
	}

	return 0;
}
