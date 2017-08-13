/*
 *  filesystem.h
 *  filesystem
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


// Megisto mikos tou onomatos arxeiou
#define MAX_FILENAME    16
// Megistos ari8mos arxeiwn (tha desmeysoume analoga bytes
// stin arxti tou arxeiou sistimatos)
#define MAX_FILES	100
// To BLOCK_SIZE to xrisimopoioume mono
// stin copy, gia to megethos tou block pou
// tha kanoume copy. Den antistoixei aparaitita
// sto block_size tou filesystem.
#define BLOCK_SIZE 1024
#define max_blocks 100

extern int superblock;			//size of block which holds FAT, free blocks list, directory table,
					//and file descriptor table

char *filesystem_file;			//to onoma tou arxeiou pou "krataei" to filesystem.

// Orizoume tin domi enos block
typedef struct
{
	// To epomeno block enos arxeiou. An einai to teleytaio arxeio
	// isoutai me miden.
	unsigned int next_block;
	// O ari8mos twn bytes pou xrisimopoii8ikan se ayto to block.
	// OXI to megethos tou block.
	unsigned int length;
} block;

// Orizoume tin domi arxeiou
typedef struct
{
	// To onoma tou arxeiou
	char name[MAX_FILENAME];
	// To prwto block pou anikei sto arxeio.
	// Ta ypoloipa blocks ta vriskoume mesw tou
	// next_block gia kathe block
	long int size;			//megethos arxeiou
	int fat_index;			//index of FAT
	unsigned int first_block;
} directory_table;


int (*FDT)[2];				//File descriptor table, first column holds file descriptor and second
					//directory index.

int (*FAT)[2];				//FAT table made on a two-dimensional array
char *buffer;  //lista me ta eleuthera blocks
// Orizoume tin domi tou filesystem
typedef struct
{
	// Megethos tou block
	unsigned int block_size;
	// Ari8mos twn blocks
	unsigned int blocks;
} filesystem;

//Directory Service API
int my_files(char **filenames);
int my_file_count();
int my_create(const char *filename);
int my_delete(const char *filename);
int my_open(const char *filename);

//File Service API
int my_close(int fd);
int my_file_size(int fd);
int my_move(int fd, char *dest);
int my_read(int fd, char *buf, int len, int pos);
int my_write(int fd, char *buf, int len, int pos);


//Dhlwsh sunarthsewn pou xrhsimopoioume
//int create_fs(int block_size,int blocks);
//int allocation(int blocks, int MAX_FILES);
//void initialization();
int write_disk(int domes_size, int data_size);
//Block Service API (internal use only)
int bs_read(int fs_descr, filesystem fs, char *buf, int len, int pos, int block_pos);
int bs_write(int fs_descr, filesystem fs, char *buf, int len, int pos, int block_pos);

void bs_free(int fs_descr, filesystem fs, unsigned int block_pos);
int bs_find_free(int fs_descr, filesystem fs);
int first_data_block(void);


