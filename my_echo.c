/* to kelufos pauei na dexetai entoles kai o xrhsths eisagei eleythera keimeno, me ton  >> operator
to keimeno eisagetai sto telos tou prouparxontos keimenou, enw me ton > operator
 mono to kainourgio keimeno brisketai sto neo arxeio pou dhmiourgeitai. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "filesystem.h"


int main(int argc, char * const argv[])
{
	char *keimeno;
	int size;
	size = BLOCK_SIZE;
	printf("Eisagwgh eleutherou keimenou\n");
	keimeno = (char *) malloc(size*sizeof(char *));
	textenter(keimeno,size);					// eisagei to keimeno pou dinei o xrhsths

	if(!memcmp(argv[1],">>", strlen(argv[1])))		// An o operator einai o >>
	{
		printf(">> to keimeno episunaptetai sto telos tou prouparxontos\n");

		if(episunapsh(argv[2], keimeno))			// prospatheia gia episunapsh tou keimenou sto arxeio
		{
			printf("Error appending file\n");	//error
			exit(1);				//eksodos
		}
	}
	else if(!memcmp(argv[1],">", strlen(argv[1])))	//An o operator einai o >
	{
		printf("> to keimeno eisagetai se ena neo arxeio pou dhmiourgeitai\n");

		if(dhmiourgia(argv[2], keimeno))			// prospatheia eisagwghs keimenou sto arxeio
		{
			printf("Error creating file\n");	//error
			exit(1);				//eksodos
		}
	}
	free(keimeno);
}


/* An o operator einai o >> to keimeno episunaptetai sto telos tou prouparxontos keimenou */

int episunapsh(char *argv[2], char *keimeno)
{
	int ufid, pos, keimeno_size;

	keimeno_size = strlen(keimeno);			// pairnei to megethos tou keimenou

	ufid = my_open(argv[2]);			// anoigma arxeiou
	if ( ufid < 0 )					// an de mporei na anoiksei
	{
		printf("Error opening file\n");		// error message
		my_close(ufid);				// kleisimo tou arxeiou
		return -1;
	}

	pos = my_file_size(ufid);			// topothetoume th thesh sto telos toy arxeiou
	if(my_write(ufid, keimeno, keimeno_size, pos) != keimeno_size)	// prospatheia na eisagei to keimeno sto arxeio
	{
		printf("De mporei na episunaptei to sto arxeio\n");	// error message
		my_close(ufid);				//kleisimo tou arxeiou
		return -1;
	}

	my_close(ufid);					// kleisimo tou arxeiou
	return 0;
}


/* An o operator einai o > to keimeno eisagetai se ena kainourgio arxeio pou dhmiourgeitai. */

int dhmiourgia(char *argv[2], char *keimeno)
{
	int ufid, keimeno_size, pos;

	keimeno_size = strlen(keimeno);			//pairnei to megethos toy keimenoy
	pos = 0;					//h thesh toy prwtoy arxeioy

	if (my_create(argv[2]))			//an de mporei na dhmiourghthei arxeio
	{
		printf("Error opening file\n");		//error message
		return -1;
	}

	ufid = my_open(argv[2]);			// anoigma tou arxeiou
	if ( ufid < 0 )					// an de mporei na anoiksei
	{
		printf("Error opening file\n");		// error message
		my_close(ufid);				// kleisimo tou arxeiou
		return -1;
	}

	if(my_write(ufid, keimeno, keimeno_size, pos) != keimeno_size)	// prospatheia na eisagei to keimeno sto arxeio
	{
		printf("Lathos sthn eisagwgh sto arxeio\n");	//error message
		my_close(ufid);				// kleisimo tou arxeiou
		return -1;
	}

	my_close(ufid);					// kleisimo tou arxeiou
	return 0;
}


/* pairnei to keimeno pou eisagei o xrhsths kai to vazei se enan pinaka.
Epistrefei ton pinaka kai to megetos tou keimenou */
void textenter(char *txt, int size)
{

	char *termat, temp[100];

	printf("\nParakalw eisagetai to keimeno.Gia termatismo pathste ctrl+D\n");
	termat=fgets(temp,sizeof temp,stdin);	// pairnei tous 4 prwtous xarakthres

	memcpy(txt,temp,strlen(temp));	//tous antigrafei sto buffer keimeno

	while(termat!=NULL)	// oso den exei pathsei to ctrl+D
	{
		bzero(temp,sizeof temp);	//arxikopoihsh tou temp me mhdenika

		termat=fgets(temp,sizeof temp,stdin);	        //painrei tous xarakthres pou eggrapse o xrhsths
		size=size+strlen(temp);			                 //pros8esh tou megethous twn newn xarakthrwn me to palio megethos
		txt=(char *) realloc(txt,size*sizeof(char *));	//desmeush mnhmhs gia tous neous xarakthres
		memcpy(txt+strlen(txt),temp,strlen(temp));		//antigrafh twn newn xarakthrwn sto keimeno
	}

}
