/*
 * apt-spy (c) Steven Holmes, 2003.
 * This software is licensed as detailed in the COPYRIGHT file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "include/global.h"
#include "include/file.h"

/**
 * @brief Selects an infile
 *
 * @param infile
 *
 * @return file pointer of the read access opened file
 */
FILE *select_infile(char *infile)
{
	if (infile != NULL)
		return fopen(infile, "r");

	/* Else return a temporary file */
	return tmpfile();
}


/**
 * @brief Selects an output file
 *
 * @param outfile
 *
 * @return file pointer to the write access opened file
 */
FILE *select_outfile(char *outfile)
{
	FILE *fp;

	/* Check if we've to use standard output... */
	if (strcmp(outfile, "-") == 0)
		return stdout;

	/* Otherwise, we open the file */
	else {
		if (backup(outfile) == 1)	/* backup old file */
			return NULL;

		fp = fopen(outfile, "w");
	}
	return fp;
}

/**
 * @brief Open configuration file
 *
 * @param config_file
 *
 * @return
 */
FILE *select_config(char *config_file)
{
	FILE *fp;

	fp = fopen(config_file, "r");

	return fp;
}

FILE *select_mirror(char *mirror_list, int are_updating)
{
	FILE *fp;

	switch (are_updating) {
 	case -1:
		printf("Creating mirror file: %s\n", mirror_list);
		fp = fopen(mirror_list, "w+");
		break;
	case 0:
    fp = fopen(mirror_list, "r");
		break;
	case 1:
		fp = fopen(mirror_list, "w");
		break;
	default:
		return NULL;
		break;
	}

	return fp;
}
/**
 * @brief Get the next line of infile_p
 *
 * @param infile_p
 *
 * @return The next line
 */
char *next_entry(FILE *infile_p)
{
	char *temp = NULL;
	/* long int values because ftell returns -1 when it fails */
	long int orig_position = 0;
	long int buffsize = 0;
	int c;
	int count;

	/* Save original file position */
	orig_position = ftell(infile_p);
	if (orig_position < 0) {
		perror("Input error (ftell)");
		exit(1);
	}

	/* Fast-forward to new line */
	/*
	 * 2008-08-24 Stefano Canepa <sc@linux.it>
	 * Modified to return and entry terminating with EOF or \n
	 */
	count = 0;
	if((c = getc(infile_p)) < 0) /* get the first char */
		return NULL;

	while ((c != '\n') && (c != EOF)) {
		count++;
		if ((c == EOF) && (count = 1))
		{
			return NULL;		/* We've exhausted the list */
		}
		c = getc(infile_p);
	}

	/* Calculate buffer size and check for ftell errors */
	buffsize = ftell(infile_p);
	if (buffsize < 0) {
		perror("Input error (ftell)");
		exit(1);
	}
	buffsize -= orig_position;

	/* create storage space for line */
	temp = malloc(buffsize + 1);

	if (temp == NULL) {
		perror("malloc");
		exit(1);
	}

	/* Rewind file to original position and check for errors */
	if (fseek(infile_p, orig_position, SEEK_SET) != 0) {
		perror("Input error (fseek)");
		exit(1);
	}

	/* We now read the line into the newly created buffer */
	if (!fgets(temp, buffsize + 1, infile_p)) {
		perror("Input error (fgets)");
		exit(1);
	}

	return temp;
}

/**
 * @brief Copy orig_file into a new file named as orig_file with ".bak" appended
 *
 * @param orig_file
 *
 * @return Upon error, "1" is returned
 */
int backup(char *orig_file)
{
	FILE *in_file, *out_file;
	char *new_name;
	int c, namesize;
	struct stat file_stat;

	if (stat(orig_file, &file_stat) == -1) {
		if (errno == ENOENT)
			return 0;	/* doesn't exist */
		else {
			perror("stat");		/* or there's been an error */
			return 1;
		}
	}

	if (file_stat.st_size == 0)
		return 0;		/* Don't backup empty file */

	in_file = fopen(orig_file, "r");

	if (in_file == NULL) {
		perror("fopen");	/* eep! */
		return 1;
	}

	/* Saves multiple calls to strlen() later */
	namesize = strlen(orig_file);

	/* We need to create the new filename. */
	new_name = malloc(namesize + 5);
	sprintf(new_name, "%s.bak", orig_file);

	out_file = fopen(new_name, "w");

	if (out_file == NULL){
    perror("fopen");
    fclose(in_file);
    free(new_name);
		return 1;
  }

	/* Do the copying */
	while((c = getc(in_file)) != EOF)
		putc(c, out_file);

	if (ferror(in_file) || ferror(out_file)) {
		perror("putc");
    fclose(in_file);
    fclose(out_file);
    free(new_name);
		return 1;
	}

	fclose(in_file);
	fclose(out_file);

	free(new_name);
	return 0;
}
