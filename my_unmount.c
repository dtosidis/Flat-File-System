/* prin termatistei to kelufos, h my_umount analambanei na grapsei oles tis domes elegxou apo th mnhmh sto disko,
 na kleisei ta anoixta arxeia kai na apodesmeusei th mnhmh pou exei desmeutei apo to susthma arxeiwn. */
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
	int i, fs_descr;

	for(i=0; i<MAX_FILES; i++)				// kleisimo olwn twn anoixtwn arxeiwn
		FDT[i][1] = -1;
	fs_descr = 0;
	fs_descr = open(filesystem_file, O_WRONLY );			// anoigma tou file system
	if (fs_descr == -1)
	{
		perror("errno");				// an den anoikse, mhnuma lathous
		exit(1);
	}

	if(write(fs_descr,&directory_table,sizeof(directory_table)) < 0)		//eggrafh tou directory table sto file system
	{
		printf("Error writing directory table\n");
		close(fs_descr);						//kleisimo filesystem
        return -1;
	}

	if(write(fs_descr,&FAT,sizeof(FAT)) < 0)			//eggrafh tou file allocation table sto file system
	{
		printf("Error writing FAT\n");
		close(fs_descr);						//kleisimo filesystem
        return -1;
	}

	if(write(fs_descr,&FDT,sizeof(FDT)) < 0)			//eggrafh tou file descriptor table sto file system
	{
		printf("Error writing file descriptor table\n");
		close(fs_descr);						//kleisimo filesystem
        return -1;
	}

	if(write(fs_descr,&buffer, sizeof(buffer)) < 0)	//eggrafh ths buffer list sto file system
	{
		printf("Error writing buffer list\n");
		close(fs_descr);						//kleisimo filesystem
        return -1;
	}

	close(fs_descr);				//kleisimo filesystem
	free(FAT);						//apodesmeush tou xwrou pou pianei to file allocation table
	free(FDT);						//apodesmeush tou xwrou pou pianei to file descriptor table
	free(buffer);					//apodesmeush tou xwrou pou pianei h buffer list
	free(directory_table);			//apodesmeush tou xwrou pou pianei to directory table
	return 0;

}
