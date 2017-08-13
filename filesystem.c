/*
 *  filesystem.c
 *  filesystem
 *
 */

#include "filesystem.h"


//Dhmiourgia tou filesystem
int create_fs()
{	
	block b;
	int size, fs_descr, dir_size;
	char *filesystem_file = "myfs.dat";		//to onoma tou filesystem mas
	fs_descr = 0;
	dir_size = 0;					//megethos tou directory table
	
	int size_of_blocks = sizeof(b);

	if(allocation(max_blocks, MAX_FILES) == -1)
	{
		printf("Error allocating tables\n");
		return -1;
	}

	initialization();				//arxikopoihsh twn domwn

	superblock = sizeof( directory_table) + sizeof( FAT) + sizeof( buffer) + sizeof (FDT);//megethos twn domwn

	size = max_blocks*size_of_blocks;		//sunoliko megethos tou block

	if(write_disk(superblock, size) == -1)	// eggrafh twn domwn sto system file
		return -1;					// kai dhmiourgia arxeiou
	else
		return 0;
}

//Directory Service API

// file list
int my_files(char **filenames)
{
	directory_table f;
	filesystem fs;

	// Anoigoume to arxeio sistimatos
	int fs_descr = open(filesystem_file, O_RDWR), fs_pos = sizeof(filesystem), read_retval = 0, i = 0, c = 0;

	// Paw k diavazw ton header.
	lseek(fs_descr, sizeof(filesystem), SEEK_SET);
	read(fs_descr, &fs, sizeof(filesystem));

	// Psaxnoume gia MAX_FILES to poly yparxonta arxeia.
	while (c++<MAX_FILES)
	{
		// Diavazoume stin thesi pou perimenoume na yparxei
		// kapoio arxeio
		read_retval = bs_read(fs_descr, fs, (char*)&f, sizeof(directory_table), fs_pos, 0);

		// An den mporesame na diavasoume, termatizoume to broxo.
		if (!read_retval) break;

		// Antigrafw ta filenames sti lista
		if (f.name[0])
		{
			strcpy(filenames[i], f.name);
			++i;
		}

		// Pame stin epomeni pithani thesi
		fs_pos += read_retval;
	}

	close(fs_descr);

	// Epistrefoume to pli8os twn arxeiwn
	return i;
}

// Epistrefei ton ari8mo twn arxeiwn pou yparxoun sto filesystem
// (to idio me to my_files).
int my_file_count()
{
	directory_table f;
	filesystem fs;

	// Anoigoume to arxeio tou sistimatos.
	int fs_descr = open(filesystem_file, O_RDWR), fs_pos = sizeof(filesystem), read_retval = 0, i = 0, c = 0;

	// Diavazoume ta dedomena tou filesystem (blocks, block_size)
	lseek(fs_descr, sizeof(block), SEEK_SET);
	read(fs_descr, &fs, sizeof(filesystem));

	// Psaxnoume gia MAX_FILES to poly yparxonta arxeia.
	while(c++<MAX_FILES)
	{
		// Diavazoume stin thesi pou perimenoume na yparxei
		// kapoio arxeio
		read_retval = bs_read(fs_descr, fs, (char*)&f, sizeof(directory_table), fs_pos, 0);

		// An den mporesame na diavasoume, termatizoume to broxo.
		if (!read_retval) break;

		// Metrame ta arxeia me mi mideniko onoma
		if (f.name[0]) ++i;

		// Pame stin epomeni pithani thesi
		fs_pos += read_retval;
	}

	close(fs_descr);


	// Epistrefoume to pli8os twn arxeiwn
	return i;
}

// Dimiourgei ena kainourgio arxeio me onoma filename
int my_create(const char *filename)
{
	directory_table f;
	filesystem fs;
	int file_id = 0;

	// Open filesystem_file
	int fs_descr = open(filesystem_file, O_RDWR), fs_pos = sizeof(filesystem), read_retval = 0, i = 0;

	// Prospa8oume na anoiksoume to arxeio
	// (an idi yparxei)
	file_id = my_open(filename);

	if (file_id)
	{
		// An yparxei, epistrefoume to id
		// tou. (Ta periexomena tou tha diagrafoun)
		return file_id;
	}


	// An den mporesame na anoiksoume to arxeio tou sistimatos,
	// epistrefoume -1.
	if (fs_descr == -1)
	{
		perror("my_create: open");
		return -1;
	}

	// Go read header
	lseek(fs_descr, sizeof(block), SEEK_SET);
	read(fs_descr, &fs, sizeof(filesystem));

	// Psaxnoume gia MAX_FILES to poly yparxonta arxeia.
	while (i<MAX_FILES)
	{
		// Diagrafoume to proigoumeno f.name
		bzero(f.name, 10);
		if (lseek(fs_descr, 0, SEEK_SET) == -1) perror("my_create: lseek");
		// Diavazoume stin thesi pou diatirountai ta stoixeia tou arxeiou
		read_retval = bs_read(fs_descr, fs, (char*)&f, sizeof(directory_table), fs_pos, 0);

		if (!f.name[0])
		{
			// Brikame ena adeio id, opote dimiourgoume to arxeio se ayto.

			// To prwto block einai to 0 (adeio arxeio)
			f.first_block = 0;
			// Grafoume to onoma tou arxeiou
			strcpy(f.name, filename);
			// Swzoume ayta ta dedomena
			bs_write(fs_descr, fs, (char*)&f, sizeof(directory_table), fs_pos, 0);

			// Epistrefoume 0 (epitixia)
			return 0;
		}

		// Pame stin epomeni thesi
		fs_pos += read_retval;

		++i;
	}

	printf("Max files reached\n");

	close(fs_descr);

	return -1;
}

// Delete file
int my_delete(const char *filename)
{
	directory_table f;
	filesystem fs;
	int fs_descr = open(filesystem_file, O_RDWR), fs_pos = sizeof(filesystem), read_retval = 0, i = 0;

	if (fs_descr == -1) perror("my_delete: open");

	lseek(fs_descr, sizeof(filesystem), SEEK_SET);
	read(fs_descr, &fs, sizeof(filesystem));

	while(i<MAX_FILES)
	{
		bzero(f.name, 10);
		lseek(fs_descr, 0, SEEK_SET);
		read_retval = bs_read(fs_descr, fs, (char*)&f, sizeof(directory_table), fs_pos, 0);

		// Sigkrinoume to onoma tou arxeiou sti thesi ayti, me to
		// zitoumeno onoma
		if (!strcmp(f.name,filename))
		{

			// An yparxei to arxeio, apodesmeyoume ta blocks pou
			// tou anikoun
			if (f.first_block)
			{
				bs_free(fs_descr, fs, f.first_block);
			}

			// Diagrafoume to onoma tou apo to sistima
			f.name[0] = 0;

			lseek(fs_descr, 0, SEEK_SET);
			// Apothikeyoume ta nea dedomena
			bs_write(fs_descr, fs, (char*)&f, sizeof(directory_table), fs_pos, 0);

			// Epistrefoume 0 gia epitixia
			return 0;
		}

		// Psaxnoume stin epomeni thesi.
		fs_pos += read_retval;
		++i;
	}

	// To arxeio den vre8ike

	return -1;
}

// Anoigei ena arxeio
int my_open(const char *filename)
{
	directory_table f;
	filesystem fs;

	// Anoigma tou filesystem_file
	int fs_descr = open(filesystem_file, O_RDWR), fs_pos = sizeof(filesystem), read_retval = 0, i = 0;

	// Anagnwsi header
	lseek(fs_descr, sizeof(block), SEEK_SET);
	read(fs_descr, &fs, sizeof(filesystem));

	while (i < MAX_FILES)
	{
		// Anagnwsi tvn blocks
		read_retval = bs_read(fs_descr, fs, (char*)&f, sizeof(directory_table), fs_pos, 0);

		if (!strcmp(f.name,filename))
		{
			return i+1;
		}

		fs_pos += read_retval;
		++i;
	}

	return 0;
}

//File Service API
int my_close(int fd)
{
	return 0;
}

int my_file_size(int fd)
{
	directory_table f;
	filesystem fs;
	block b;

	int fs_pos = ((fd-1)*sizeof(directory_table))+sizeof(filesystem);
	int fs_descr = open(filesystem_file, O_RDWR);
	int block_pos = 0, file_size = 0;

	lseek(fs_descr, sizeof(filesystem), SEEK_SET);
	read(fs_descr, &fs, sizeof(filesystem));

	lseek(fs_descr, 0, SEEK_SET);
	bs_read(fs_descr, fs, (char*)&f, sizeof(directory_table), fs_pos, 0);

	// Anagnwsi tvn block
	block_pos = f.first_block;
	while (block_pos)
	{
		lseek(fs_descr, block_pos, SEEK_SET);
		read(fs_descr, &b, sizeof(block));

		// To sinolo tvn mikvn einai to megethos tou arxeiou.
		file_size += b.length;

		if (block_pos == b.next_block) break;
		block_pos = b.next_block;
	}

	return file_size;
}

int my_move(int fd, char *dest)
{
	directory_table f;
	filesystem fs;

	int fs_pos = ((fd-1)*sizeof(directory_table))+sizeof(filesystem);
	int fs_descr = open(filesystem_file, O_RDWR);

	lseek(fs_descr, sizeof(block), SEEK_SET);
	read(fs_descr, &fs, sizeof(filesystem));

	// diavazoume ta dedomena tou arxeiou
	bs_read(fs_descr, fs, (char*)&f, sizeof(directory_table), fs_pos, 0);
	// Antigrafoume to zitoumeno onoma sto onoma tou arxeiou
	// kai apothikeyoume
	strcpy(f.name,dest);
	bs_write(fs_descr, fs, (char*)&f, sizeof(directory_table), fs_pos, 0);

	return fd;
}

int my_read(int fd, char *buf, int len, int pos)
{
	directory_table f;
	filesystem fs;

	int fs_pos = ((fd-1)*sizeof(directory_table))+sizeof(filesystem);
	int fs_descr = open(filesystem_file, O_RDWR);

	lseek(fs_descr, sizeof(filesystem), SEEK_SET);
	read(fs_descr, &fs, sizeof(filesystem));

	lseek(fs_descr, 0, SEEK_SET);
	bs_read(fs_descr, fs, (char*)&f, sizeof(directory_table), fs_pos, 0);

	// An to arxeio den exei dedomena epistrefoume
	if (!f.first_block) return 0;

	// Pigainoume stin thesi tou prwtou block
	// kai diavazoume osa dedomena zitithikan, apo
	// ti thesi pou zitithikan.
	lseek(fs_descr, f.first_block, SEEK_SET);
	return bs_read(fs_descr, fs, buf, len, pos, f.first_block);
}


int my_write(int fd, char *buf, int len, int pos)
{
	directory_table f;
	filesystem fs;
	block b;

	int search_block = 0;
	int fs_pos = ((fd-1)*sizeof(directory_table))+sizeof(filesystem);
	int fs_descr = open(filesystem_file, O_RDWR);

	lseek(fs_descr, sizeof(filesystem), SEEK_SET);
	read(fs_descr, &fs, sizeof(filesystem));

	lseek(fs_descr, 0, SEEK_SET);
	bs_read(fs_descr, fs, (char*)&f, sizeof(directory_table), fs_pos, 0);

	// Prepei na arxisoume na psaxoume apo to prwto block pou vrisketai
	// *meta* apo tin desmevmeni perioxi.
	search_block = first_data_block();

	while (!f.first_block && search_block < fs.block_size*fs.blocks)
	{
		//Append another block to the file
		lseek(fs_descr, search_block, SEEK_SET);

		read(fs_descr, &b, sizeof(block));

		if (!b.length)
		{
			f.first_block = search_block;
		}
		else
		{
			search_block = search_block+fs.block_size;
		}
	}

	if (!f.first_block) return 0;

	lseek(fs_descr, 0, SEEK_SET);
	bs_write(fs_descr, fs, (char*)&f, sizeof(directory_table), fs_pos, 0);

	// Grafoume mesw tis bs_write ta dedomena.
	lseek(fs_descr, f.first_block, SEEK_SET);
	return bs_write(fs_descr, fs, buf, len, pos, f.first_block);
}




// Epistrefei tin thesi tou prwtou block sto opoio mporoume
// na grapsoume dedomena.
int first_data_block(void)
{
	int fs_descr = open(filesystem_file, O_RDWR);
	int last_pos = lseek(fs_descr, 0, SEEK_CUR);
	filesystem fs;

	lseek(fs_descr, sizeof(filesystem), SEEK_SET);
	read(fs_descr, &fs, sizeof(filesystem));

	lseek(fs_descr, last_pos, SEEK_SET);

	/* Desmeyoume sizeof(files)*MAX_FILES bytes stin arxi tou arxeiou
	 * sistimatos, gia ta dedomena twn arxeiwn (onoma, first_block) kai
	 * sizeof(filesystem) bytes gia ta dedomena tou filesystem(blocks kai
	 * block_size).   Apo ekei kai meta, mporoume na grafoume ta periexomena
	 * twn arxeiwn sta blocks */

	return (fs.block_size*((int)((float)(sizeof(filesystem)+sizeof(directory_table)*(MAX_FILES-1))/(float)fs.block_size) + 2));
}

/*
 * Take a buffer to write to a file, and an offset to write it at. Append blocks
 * to the file as necessary.
 */

int bs_write(int fs_descr, filesystem fs, char *buf, int len, int pos, int block_pos)
{
	block b;
	int block_count = 1, bytes_written = 0;
	int last_pos = lseek(fs_descr, 0, SEEK_CUR);


	// Pigainoume sto zitoumeno block kai diavazoume
	// ta stoixeia tou (length, next_block)
	if (lseek(fs_descr, block_pos, SEEK_SET) == -1) perror("bs_write: lseek");
	if (read(fs_descr, &b, sizeof(block)) == -1) perror("bs_write: read");




	// An i thesi einai mikroteri apo tin thesi tou prwtou block, simainei
	// pws diavazoume stoixeia arxeiwn opote den mas apasxoloun block dedomena,
	// apla grafoume me write:
	if (block_pos + pos < first_data_block()) {
		lseek(fs_descr, pos+sizeof(block), SEEK_SET);
		bytes_written = write(fs_descr, buf, sizeof(directory_table));
		lseek(fs_descr, last_pos, SEEK_SET);
		return bytes_written;
	}
	// Ena block xwraei (block_size - sizeof(block)) dedomenwn, opote
	// me tetoio vima proxwrame mexri ti thesi pou theloume (pos).
	while (pos >= block_count*(fs.block_size-sizeof(block)))
	{
		if (!b.next_block)
		{
			// An to block pou koitame twra den exei next_block,
			// briskoume ena eley8ero block k tou to anathetoume.
			b.next_block = bs_find_free(fs_descr, fs);
		}

		// Thetoume ws megethos tou block, to megisto megethos
		// dedomenwn pou mporei na kratisei.
		b.length = fs.block_size-sizeof(block);

		// Pigainoume stin katalili thesi tou arxeiou sistimatos kai
		// grafoume ta stoixeia tou trexontws block
		if (lseek(fs_descr, block_pos, SEEK_SET) == -1) perror("bs_write: lseek");
		if (write(fs_descr, &b, sizeof(block)) == -1) perror("bs_write: write");

		// Proxwrame sto epomeno block
		block_pos = b.next_block;
		// kai diavazoume ta stoixeia tou
		if (lseek(fs_descr, block_pos, SEEK_SET) == -1) perror("bs_write: lseek");
		if (read(fs_descr, &b, sizeof(block)) == -1) perror("bs_write: read");

		block_count++;
	}

	//Adjust offset to be relative to current block, move file pointer
	pos = pos-((block_count-1)*(fs.block_size-sizeof(block)));
	if (lseek(fs_descr, block_pos+sizeof(block)+pos, SEEK_SET) == -1) perror("bs_write: lseek");

	// Grafoume kapoia tyxaia dedomena sto block gia
	// na min thewri8ei adeio otan tha psaksoume
	// gia eley8ero block.
	b.length = 1;
	lseek(fs_descr, block_pos, SEEK_SET);
	write(fs_descr, &b, sizeof(block));

	// Oso exoume dedomena gia na grapsoume ...
	while (len)
	{
		// Pame sti thesi pou tha graftoun ta dedomena
		lseek(fs_descr, block_pos+sizeof(block)+pos, SEEK_SET);

		if (fs.block_size-sizeof(block) >= pos+len)
		{
			//Rest of the buffer will fit in current block
			if (write(fs_descr, buf, len) == -1) perror("bs_write: write");
			b.length = len;

			if (b.length > pos)
			{
				if (len > b.length-pos)
				{
					// Xwrane ta dedomena sto idio block, opote enimerwnoume
					// to megethos tou block.
					b.length = b.length+(len-(b.length-pos));
				}
			}
			else
			{
				b.length = pos+len;
			}

			// Apothikeyoume ta dedomena.
			if (lseek(fs_descr, block_pos, SEEK_SET) == -1) perror("bs_write: lseek");
			if (write(fs_descr, &b, sizeof(block)) == -1) perror("bs_write: write");

			// Epistrefoume stin teleytaia thesi.
			lseek(fs_descr, last_pos, SEEK_SET);
			return bytes_written+len;
		}
		else
		{
			// Ta dedomena den xwrane, opote tha xreiastoume k epomeno block.
			if (!b.next_block || b.next_block == block_pos)
			{
				// An den yparxei epomeno block, desmeyoume twra ena.
				b.next_block = bs_find_free(fs_descr, fs);
			}

			// Apothikeyoume ta dedomena
			lseek(fs_descr, block_pos+sizeof(block)+pos, SEEK_SET);
			if (write(fs_descr, buf, (fs.block_size-sizeof(block))-pos) == -1) perror("bs_write: write");

			// Enimerwnoume gia to megethos tou block.
			b.length = fs.block_size-sizeof(block);

			bytes_written = bytes_written+((fs.block_size-sizeof(block))-pos);
			buf = buf+((fs.block_size-sizeof(block))-pos);
			len = len-((fs.block_size-sizeof(block))-pos);
			pos = 0;

			// Apothikeyoume gia ta stoixeia tou block.
			if (lseek(fs_descr, block_pos, SEEK_SET) == -1) perror("bs_write: lseek");
			if (write(fs_descr, &b, sizeof(block)) == -1) perror("bs_write: write");

			block_pos = b.next_block;
			if (lseek(fs_descr, block_pos, SEEK_SET) == -1) perror("bs_write: lseek");
			if (write(fs_descr, &b, sizeof(block)) == -1) perror("bs_write: read");
		}
	}

	// Epistrefoume stin teleytaia thesi.
	lseek(fs_descr, last_pos, SEEK_SET);
	return bytes_written;
}

/*
 * Take a specified length of bytes to read, an offset, and the block to start at. Navigate
 * to the approrpriate starting block dictated by the offset, and read until the buffer has
 * been filled or the file ends.
 *
 */

int bs_read(int fs_descr, filesystem fs, char *buf, int len, int pos, int block_pos)
{
	block b;
	int block_count = 1, bytes_read = 0;
	int last_pos = lseek(fs_descr, 0, SEEK_CUR);

	//Read the initial block header
	if (lseek(fs_descr, block_pos, SEEK_SET) == -1) perror("bs_read: lseek");
	if (read(fs_descr, &b, sizeof(block)) == -1) perror("bs_read: read");

	// Diavazoume dedomena arxeiou
	if (block_pos + pos < first_data_block()) {
		lseek(fs_descr, pos+sizeof(block), SEEK_SET);
		bytes_read = read(fs_descr, buf, sizeof(directory_table));
		lseek(fs_descr, last_pos, SEEK_SET);
		return bytes_read;
	}
	//Skip all the blocks before our offset
	while (pos >= block_count*(fs.block_size-sizeof(block)))
	{
		// An yparxei epomeno block, proxwrame se ekeino
		if (b.next_block && b.next_block != block_pos)
		{
			block_pos = b.next_block;

			if (lseek(fs_descr, b.next_block, SEEK_SET) == -1) perror("bs_read: lseek");
			if (read(fs_descr, &b, sizeof(block)) == -1) perror("bs_read: read");
			block_count++;
		}
		else
		{
			//Offset is outside file size
			// Epistrefoume stin teleytaia thesi.
			lseek(fs_descr, last_pos, SEEK_SET);
			return 0;
		}
	}

	//Adjust offset to be relative to current block, move file pointer
	pos = pos-((block_count-1)*(fs.block_size-sizeof(block)));
	if (lseek(fs_descr, block_pos+sizeof(block)+pos, SEEK_SET) == -1) perror("bs_read: lseek");

	if (pos > b.length)
	{
		// H thesi einai eksw apo to block.
		// Epistrefoume stin teleytaia thesi.
		lseek(fs_descr, last_pos, SEEK_SET);
		return 0;
	}

	// Oso yparxoun dedomena gia diavasma
	while (len)
	{
		// Pame sti thesi pou yparxoun dedomena gia anagnwsi
		if (lseek(fs_descr, block_pos+sizeof(block)+pos, SEEK_SET) == -1) perror("bs_read: lseek");

		if (len > b.length-pos)
		{
			//Read the rest of the block, and then move on to the next block
			read(fs_descr, buf, b.length-pos);

			if (b.next_block && b.next_block != block_pos)
			{
				bytes_read = bytes_read+(b.length-pos);
				buf = buf+(b.length-pos);
				len = len-(b.length-pos);
				pos = 0;

				block_pos = b.next_block;

				lseek(fs_descr, b.next_block, SEEK_SET);
				read(fs_descr, &b, sizeof(block));
			}
			else
			{
				lseek(fs_descr, last_pos, SEEK_SET);
				return b.length-pos;
			}
		}
		else
		{
			//Read into the rest of the buffer and return
			read(fs_descr, buf, len);
			lseek(fs_descr, last_pos, SEEK_SET);
			return bytes_read+len;
		}
	}

	// Den yparxoun alla dedomena.
	lseek(fs_descr, last_pos, SEEK_SET);
	return bytes_read;
}

// Diagrafi enos block.
void bs_free(int fs_descr, filesystem fs, unsigned int block_pos)
{
	block b;
	// Swzoume ti thesi mas.
	int last_pos = lseek(fs_descr, 0, SEEK_CUR);

	// Diavazoume ta stoixeia tou block
	if (lseek(fs_descr, block_pos, SEEK_SET) == -1) perror("bs_free: lseek");
	if (read(fs_descr, &b, sizeof(block)) == -1) perror("bs_free: read");

	// An to block ayto einai sindedemeno me allo block,
	// diagrafoume kai ekeinou ta stoixeia.
	if (b.next_block && b.next_block != block_pos)
	{
		bs_free(fs_descr, fs, b.next_block);
	}

	b.next_block = 0;
	b.length = 0;

	// Apothikeyoume sto arxeio sistimatos tis allages.
	if (lseek(fs_descr, block_pos, SEEK_SET) == -1) perror("bs_free: lseek");
	if (write(fs_descr, &b, sizeof(block)) == -1) perror("bs_free: write");

	// Epanaferoume ti thesi mas.
	lseek(fs_descr, last_pos, SEEK_SET);
}

// Vriskei ena eley8ero block k epistrefei
// ti thesi tou.
int bs_find_free(int fs_descr, filesystem fs)
{
	block search_block;

	int block_pos = 0, search_block_pos = 0;
	// Swzoume tin thesi pou vriskomaste.
	int last_pos = lseek(fs_descr, 0, SEEK_CUR);

	// Ksekiname apo to prwto dinato block
	search_block_pos = first_data_block();

	// Psaxnoume mexri na ftasoume sto telos tou
	// filesystem i na vroume kapoio eleyer8o
	// block.
	while (!block_pos && search_block_pos < fs.blocks*fs.block_size)
	{
		// Diavazoume ena-ena ta blocks
		lseek(fs_descr, search_block_pos, SEEK_SET);
		read(fs_descr, &search_block, sizeof(block));

		// An exei mi mideniko mikos, simainei pws einai
		// desmeymeno.
		if (!search_block.length)
		{
			// An einai mideniko to megethos tou,
			// exoume vrei ena eley8ero block
			block_pos = search_block_pos;
		}
		else
		{
			// Pigainoume sto epomeno block gia
			// na elenksoume an einai eley8ero.
			search_block_pos += fs.block_size;
		}
	}

	// Epistrefoume sti thesi pou vriskomastan prin
	// tin klisi tis bs_find_free
	lseek(fs_descr, last_pos, SEEK_SET);
	return block_pos;
}

//Desmeush apaitoumenhs mnhmhs gia tis domes
int allocation(int max_blocks, int MAX_FILES)
{
	int i;

	if((buffer = malloc(max_blocks*sizeof(char))) == NULL)	//desmeush xwrou gia ton arithmo twn eleutherwn blocks
	{
		printf("Error allocating buffer list\n");
		free(buffer);						// apodesmeush xwrou pou pianei to buffer me ta eleuthera blocks

        return -1;
	}

	if((FAT = malloc(max_blocks*sizeof(int[2]))) == NULL)		//desmeush xwrou gia to dile allocation table
	{
		printf("Error allocating FAT\n");
		free(buffer);						// apodesmeush xwrou pou pianei to buffer me ta eleuthera blocks
        free(FAT);							//apodesmeush xwrou pou pianei to file allocation table

        return -1;
	}

	if((FDT = malloc(MAX_FILES*sizeof(int[2]))) == NULL)		//desmeush xwrou gia to file descriptor table
	{
		printf("Error allocating FDT\n");
		free(buffer);						// apodesmeush xwrou pou pianei to buffer me ta eleuthera blocks
        free(FAT);							//apodesmeush xwrou pou pianei to file allocation table
        free(FDT);							//apodesmeush xwrou pou pianei to file descriptor table

        return -1;
	}

	if((directory_table = malloc(num_files*sizeof(struct directory_table *))) == NULL)
								//desmeush xwrou gia to directory table
	{
		printf("Error allocating directory table\n");
		free(buffer);						// apodesmeush xwrou pou pianei to buffer me ta eleuthera blocks
        free(FAT);							//apodesmeush xwrou pou pianei to file allocation table
        free(FDT);							//apodesmeush xwrou pou pianei to file descriptor table
        free(directory_table);				//apodesmeush xwrou pou pianei to directory table

        return -1;
	}

	return 0;

}



//Arxikopoihsh twn domwn
void initialization()
{
	int i;

	for(i=0; i<MAX_FILES; i++)				//Arxikopoihsh tou directory table
	{
		directory_table[i].size=-1;				//Arxikopoihsh tou megethous
		directory_table[i].fat_index=-1;			//Arxikopoihsh tou FAT index
	}

	for(i=0; i<max_blocks; i++)				// Arxikopoihsh tou file allocation table
	{
		FAT[i][0]=-1;					// arxikopoihsh ths dieuthunshs tou block sto disko
		FAT[i][1]=-2;					// arxikopoihsh ths dieuthunshs tou epomenou block
	}

	for(i=0; i<MAX_FILES; i++)				// arxikopoihsh tou file descriptor table
	{
		FDT[i][0] = i;					// arxikopoihsh twn  file descriptors
		FDT[i][1] = -1;
	}

	for(i=0; i < max_blocks; i++)				// arxikopoihsh ths listas me ta eleuthera blocks
	{
		buffer[i] = 1;				// ola ta blocks einai eleuthera

	}
}


/* Anoigma tou file system , eggrafh twn Directory Table, File Allocation Table, buffer list
   kai File Descriptor Table sto file system kai orismos tou telikou megethous toy file system. */
int write_disk(int domes_size, int data_size)
{
	//anoigoume to arxeio tou susthmatos
	int fs_descr = open(filesystem_file, O_RDWR), fs_pos = sizeof(filesystem), read_retval = 0, i = 0;

	if(write(fs_descr,&directory_table,sizeof(directory_table)) < 0)		//eggrafh tou directory table sto file system
	{
		printf("Error writing directory table\n");
        close(fs_descr);			//kleisimo file system
        return -1;
	}

	if(write(fs_descr,&FAT,sizeof(FAT)) < 0)			//eggrafh tou file allocation table sto file system
	{
		printf("Error writing FAT\n");
        close(fs_descr);			//kleisimo file system
        return -1;
	}

	if(write(fs_descr,&buffer,sizeof(buffer)) < 0)	//eggrafh ths lista me ta eleythera blocks sto file system
	{
		printf("Error writing free blocks list\n");
        close(fs_descr);			//kleisimo file system
        return -1;
	}

	if(write(fs_descr, &FDT, sizeof(FDT)) < 0)			//eggrafh tou file descriptor table sto file system
	{
		printf("Error writing file descriptor table\n");
        close(fs_descr);			//kleisimo file system
        return -1;
	}

	//thetoume me thn truncate to megethos tou file system pou theloume
	if(ftruncate(fs_descr, data_size + superblock) == -1)
	{
        close(fs_descr);			//kleisimo file system
        return -1;
    	}

	close(fs_descr);			//kleisimo file system
	return 0;


}
