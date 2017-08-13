/* Tupwnei mia lista me tis diathesimes entoles */
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
	printf("my_cat <file> : Pairnei ws orisma to onoma enos arxeiou kai typwnei ta periexomena tou \n");
	printf("my_mv <source> <dest> : metonomazei to arxeio source se dest.\n");
	printf("my_rm <file> : diagrafei to arxeio apo ton disko.\n");
	printf("my_cp <source> <dest> : antigrafei to arxeio source sto dest.\n");
	printf("my_df : anafairei ta eleuthera blocks tou susthmatos kai to sunoliko arithmo blocks.\n");
	printf("my_ls : ektupwnei mia lista me ta arxeia tou susthmatos.\n");
	printf("my_echo <operation> <file> : to kelufos pauei na dexetai entoles kai o xrhsths eisagei eleythera keimeno\n");
	printf("\t >> operation to keimeno eisagetai sto telos tou prouparxontos keimenou.\n");
	printf("\t > operation mono to kainourgio keimeno brisketai sto arxeio.\n");
	printf("my_quit : termatizei to kelufos.\n");
	printf("my_mkfs <blocks> <block-size> : Arxikopoiei  to sistima arxeiwn kai dimiourgei,\n");
	printf("\t <blocks> to arxeio tou susthmatos.\n");
	printf("my_mount : kanei ola ta allocations pou prepei na ginoun sth kuria mnhmhkai sth sunexeia\n");
	printf("\t diabazei apo ton disko sth kuria mnhmh tis basikes domes pou elegxoun\n");
	printf("\t th leitourgia tou susthmatos arxeiwn.\n");
	printf("my_umount : analambanei na grapsei oles tis domes elegxou apo th mnhmh sto disko,\n");
	printf("\t na kleisei ta anoixta arxeia kai na apodesmeusei th mnhmh pou exei desmeutei apo to susthma arxeiwn.\n");
}
