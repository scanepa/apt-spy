/*
 * Copyright (C), 2008, 2009 by Stefano Canepa <sc@linux.it>
 * All Rights Reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* (c) Steven Holmes, 2003.
 * (c) Stefano Canepa <sc@linux.it>, 2008
 *
 * This software was licensed as detailed in the COPYRIGHT.old file
 * as of 22nd Feb 2009 it is licensed under the license specified
 * into the header of this file
 */

/* This file is a bit icky */

/*
 * Moved to the .h file
 */
/* char *str_toupper(char *str); */

#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "include/global.h"
#include "include/parse.h" 
#include "include/file.h"
#include "include/trim.h"

/**
 * @param The config file
 * @param 
 * @param The mirror list file
 * @param Area
 */
int build_area_file(FILE *config_p, FILE *infile_p, FILE *mirror_list,
					char *area)
{
	char *line;		/* Where we read the lines into */
	char *tmp;		/* Temp. pointer */
	char *inputline;	/* The line that will be written to config_p */
	char *country_code;	/* where we put the country code */
	int infilePos;
	char *dirs;
	char *aliasList;
	char *dst;
	char *mirrorData;
	char *token;
	
	/* Uppercase area */
	str_toupper(area);

	while((line = next_entry(config_p)) != NULL) {
		/* Uppercase line */
		str_toupper(line);
		
		/* test for area string */
		if ((tmp = strstr(line, area)) != NULL) {
		
			if ((strchr(tmp,':')) == NULL) { 	/* And for trailing colon.. */
				free(line);
				continue;			/* .. it's not there */
			}
			break;
		}
			
		free(line);

		if (ferror(config_p) != 0)			/* Check for File error */
			return 1;
	}

	/* No match. Return. */
	if (line == NULL) {
		fprintf(stderr, "Could not find area named %s\n", area);
		return 1;
	}
	
	free(line);
		
	/* We now have the "label" line. Country list begins on the next line. */
	while ((line = next_entry(config_p)) != NULL) {

		if (ferror(config_p) != 0) {	/* Check for file error */
			free(line);
			return 1;
		}

		/* Skip blank lines */
		country_code = line;

		/*
		 * We check for either a non-space or a '\n'. Has the useful
		 * side-effect of fast-forwarding country_code past any preceding
		 * whitespace.
		 */
		while ((*country_code != '\n') && (isspace(*country_code) != 0))
			++country_code;
		
		/*
		 * If country_code points to a '\n', there were no other characters.
		 * It was a blank line. If it points to a '#', there is a comment.
		 * We skip it too.
		 */
		if ((*country_code == '\n') || (*country_code == '#'))
			continue;

		if ((strchr(line, ':')) != NULL)
			return 0;			/* End of list. Return. */

		/*
		 * We do a little fiddling to get the country code down to 2 letters
		 * and a space
		 */
		*(country_code + 2) = ' ';
		*(country_code + 3) = '\0';

		/*
		 * Sigh. The country code is in "country_code". We now parse the
		 * mirrors file for this and set the file position so that the next
		 * read will return the first mirror.
		 */
		if (find_country(mirror_list, country_code) == 1) {
			fprintf(stderr, "Couldn't find country %s. Skipping.\n",
					country_code);
			free(line);
			continue;
		}

		/*
		 * The next read of infile_p will return the first mirror entry.
		 * We parse this and build a line to put into the temporary file.
		 */
		
		while ((inputline = get_mirrors(mirror_list)) != NULL) {
			/* if the line does not begin with "  (" */
			if (strstr(inputline, "  (") == NULL) {
/*			if(!isspace(*inputline)) { */
				/* get position of infile_p */
				infilePos = ftell(infile_p);		
				/* We now write the line to the temporary file */	
				fputs(inputline, infile_p);
				free(inputline);
	
				if ((ferror(infile_p)) != 0) { 	/* Check for file error */
					free(line);
					return 1;
				}
			}
			/* if get_mirrors returns a space it means that inputline contains
			   the list of servers that reply to ftp.XX.debian.org */
			else
			{
				/* go back one line in infile to delete the useless entry for
				   ftp.XX.debian.org */
				fseek(infile_p, infilePos, SEEK_SET);
				mirrorData = next_entry(infile_p);
				
				/* create a copy of inputline with the list of real name of the
				   alias and trim leading and trailing space and parentesis from
				   inputline */
				aliasList = trim(inputline,' ');
				aliasList = ltrim(aliasList,'(');
				aliasList = rtrim(aliasList,')');
				
				/* split aliasList into token and concat them with the info
				   extracted from inputline */
				/* mirrorData contains all the line except the mirror name */
				dirs = strstr(mirrorData, ":"); /* strtok(mirrorData, ":"); */
				token = strtok(aliasList, ", ");

				/* record this info into infile */
				while (token != NULL) {
					dst = (char *)calloc(strlen(token) + strlen(mirrorData) + 1,
										 sizeof(char));
					strcpy(dst, token);
					strcat(dst, dirs);
					fputs(dst, infile_p);
					token = strtok(NULL, ", ");					
				}
				
			}
		}

		free(line);
	}
	return 0;
}

/** 
 * 
 * 
 * @param config_p 
 * @param infile_p 
 * @param mirror_list 
 * @param country_list 
 * 
 * @return 
 */
int build_country_file(FILE *config_p, FILE *infile_p, FILE *mirror_list,
					   char *country_list)
{
	char *country_code;
	char *p, *q; 
	char *inputline;

	int found = 0;

	/* Upper-case country list */
	str_toupper(country_list);

	/* A cheap way to make sure we have enough space */
	country_code = malloc(strlen(country_list));

	p = country_list;

	while (*p != '\0') {
		/* Reset country code pointer */
		q = country_code;
	
		/* Skip white space */
		while (isspace(*p))
			++p;
			
		/* Copy up until end or comma */
		while ((*p != '\0') && (*p != ',') && (isspace(*p) == 0))
			*q++ = *p++;
			
		/* Skip more white space. *sigh* */
		while (isspace(*p))
			++p;
	
		/* skip past comma */
		if (*p != '\0')
			++p;
		
		/* String-ify */
		*q++ = ' ';
		*q = '\0';

		/* And find the country/build the file */
		if (find_country(mirror_list, country_code) == 1) {
			fprintf(stderr, "Couldn't find country %s. Skipping.\n", country_code);
			continue;
		}

		found = 1;

		while ((inputline = get_mirrors(mirror_list)) != NULL) {
			if( ! isspace(*inputline)) {
				fputs(inputline, infile_p);
				free(inputline);
				
				if (ferror(infile_p)) {
					free(country_code);
					return 1;
				}
			}
		}
	}
	free(country_code);
	
	/* Check we have found at least one country */
	if (found == 0)
		return 1;
	else
		return 0;
}

/** 
 * 
 * 
 * @param mirror_list 
 * @param country_code 
 * 
 * @return 
 */
int find_country(FILE *mirror_list, char *country_code)
{
	char *line, *cc;

	/* Make sure we're at beginning of file */
	rewind(mirror_list);

	/* This is a hack to allow users to specify "UK" instead of "GB" */
	if (!strcmp(country_code, "UK "))
		strncpy(country_code, "GB ", 4);

	/* Read until we find the country code */
	while ((line = next_entry(mirror_list)) != NULL) {

		cc = strstr(line, country_code);
		if (cc == NULL)
			continue;
			
		/* Skip white space */
		while (isspace(*line))
			++line;
		
		/* Country code should be first two characters on line. */
		if (cc == line)		
			break;
	}

	if (line == NULL)
		return 1;
	
	next_entry(mirror_list);		/* Skip a line */
	
	if (ferror(mirror_list)) {
		free(line);
		return 1;
	}
	return 0;		/* We're positioned nicely for the next read */
}

/* line format is "server:ftp-path:http-path". */

/*
 *
 */
char *get_mirrors(FILE *mirror_list)
{
	char *line, *save_line;
	char *creation, *save_creation;
	int counter = 0;
	int len;

	/* First, we read in a line from the file */
	/*
	 * 2008-08-29 Stefano Canepa <sc@linux.it>
	 *
	 * Verify if the next_entry returns NULL before going on
	 */
	if((save_line = line = next_entry(mirror_list)) == NULL)
		return NULL;
	
	/* Allocate space for creation */
	len=5+strlen(line);
	save_creation = creation = malloc(len);

	if (creation == NULL) {
		perror("malloc");
		exit(1);
	}

	/* test for file error */
	if (ferror(mirror_list)) {
		perror("fopen");
		free(save_creation);
		return NULL;
	}

	/*
	 * If the line begins with a "  (" it's a mirror alias so we need to
	 * continue some way
	 */	
	if (strstr(line, "  ("))
		return save_line;
	
	/*
	 * If the line begins with a space, we assume it is empty and the list
	 * is exhausted.
	 */
	if (isspace(*line) != 0) {
		free(save_creation);
		return NULL;
	}

	/* We now read the server name into "creation" */
	while (isspace(*line) == 0)
		*creation++ = *line++;

	/* And add a colon, which is the field seperator */
	*creation++ = ':';
	
	/*
	 * We skip over whitespace. If there is a lot of whitespace, we assume
	 * there is no FTP entry.
	 */
	while ((isspace(*line) != 0) && (counter < 30)) {
		++line;
		++counter;
	}

	/* Check if there is an entry or just more space */	
	while (isspace(*line) == 0) 
		*creation++ = *line++;

	*creation++ = ':';

	/* We now check for an HTTP entry */
	while (*line != '\n') {
		if (isspace(*line) == 0)
			break;
		line++;
	}

	/*
	 * the entry should start with slash sign, if not it's a list of
	 * architectures
	 */
	if (*line == '/') {
		while (isspace(*line) == 0)
			*creation++ = *line++;
	}			

	*creation++ = ':';
	*creation++ = '\n';
	*creation++ = '\0';

	free(save_line);
	
	return save_creation;
}

void tokenise(server_t *current, char *cur_entry)
{
	char *temp;	/* We use this for temporary string-pointing :P */
	static char null_string[]="";

	/* We initialise the structure to 0 */
	memset(current, 0, sizeof(*current));

	/* First, we copy the server name into the struct. */
	current->hostname=malloc(strlen(cur_entry));
	if (!current->hostname) {
		perror("malloc");
		exit(1);
	}

	temp = current->hostname;
	while (*cur_entry != ':')
		*temp++ = *cur_entry++;

	*temp++ = '\0';		/* Turn into string */
	current->hostname=realloc(current->hostname, 1+strlen(current->hostname));
	if (!current->hostname) {
		perror("realloc");
		exit(1);
	}

	/* We now check for an ftp entry and copy it in */
	if (*(++cur_entry) != ':') {
		current->path[FTP]=malloc(strlen(cur_entry));
		if (!current->path[FTP]) {
			perror("malloc");
			exit(1);
		}
		temp = current->path[FTP];		
		while (*cur_entry != ':')
			*temp++ = *cur_entry++;
			
		*temp++ = '\0';
		current->path[FTP]=realloc(current->path[FTP],
								   1+strlen(current->path[FTP]));
		if (!current->path[FTP]) {
			perror("malloc");
			exit(1);
		}
	} else current->path[FTP]=null_string;

	/* And now check for HTTP entry */
	if (*(++cur_entry) != ':') {
		current->path[HTTP]=malloc(strlen(cur_entry));
		if (!current->path[HTTP]) {
			perror("malloc");
			exit(1);
		}
		temp = current->path[HTTP];
		while (*cur_entry != ':')
			*temp++ = *cur_entry++;
			
		*temp++ = '\0';
		current->path[HTTP]=realloc(current->path[HTTP],
									1+strlen(current->path[HTTP]));
		if (!current->path[HTTP]) {
			perror("realloc");
			exit(1);
		}
	} else current->path[HTTP]=null_string;

	/* We're done for now */
}
/*
 * sc
 */
/* int write_list(FILE *outfile_p, server_t *best, char *dist) */
int write_list(FILE *outfile_p, server_t *best, char *dist,
			   char *section_list, char *args[], int l)
{
	char *url;
	int i;
	char *p, t[100];
	/* Make our mark ;) */
	fprintf(outfile_p, "# sources.list generated by apt-spy %s\n", apt_spy_v);
	fprintf(outfile_p, "#\n# Generated using:\n#\n# apt-spy \\\n");
	for(i = 0; i < l-1; i++) 
	{
		fprintf(outfile_p, "# \t%s \\\n", args[i]);
	}
	fprintf(outfile_p, "# \t%s\n#\n", args[i]);
	/* Copy URL information */
	if (best[0].stats.protocol == FTP) url=best[0].url[FTP];
	else {
		assert(best[0].stats.protocol == HTTP);
		url=best[0].url[HTTP];
	}
	
	/* And write the line */
	if(section_list != NULL) 
	{
		/* substitute , with a white space */
		p = strstr(section_list, ",");
		t[0] = 0; /*now t contains empty string */
		strncat(t, section_list, p-section_list); /*copy part of s preceding a */
		strcat(t, " "); 
		strcat(t, p+strlen(",")); /*add on the rest of s */
		strcpy(section_list, t);
		fprintf(outfile_p, "deb %s %s main %s\n", url, dist, section_list);		
		/* We also write a deb-src line */
		fprintf(outfile_p, "deb-src %s %s main %s\n", url, dist, section_list);
	} else {
		fprintf(outfile_p, "deb %s %s main #contrib non-free\n", url, dist);		
		/* We also write a deb-src line */
		fprintf(outfile_p, "deb-src %s %s main #contrib non-free\n", url, dist);
	}


	/* And if we are using "stable", add a security line. Otherwise comment it out.*/
	if (strcmp(dist, "stable") == 0)
		fprintf(outfile_p, "deb http://security.debian.org/ stable/updates main\n");
	else if (strcmp(dist, "testing") == 0)
		fprintf(outfile_p, "deb http://security.debian.org/ testing/updates main\n");
	else
		fprintf(outfile_p, "#deb http://security.debian.org/ stable/updates main\n");

	if (ferror(outfile_p) != 0) {
		perror("fprintf");
		return 1;
	}
	return 0;
}

int write_top(FILE *infile_p, FILE *outfile_p, server_t *best)
{
	int i = 0;
	char *line;
	
	while (i < bestnumber) {
	
		/* Make sure we're at the beginning */
		rewind(infile_p);
		
		/* Read in a line... */
		while ((line = next_entry(infile_p)) != NULL) {
			if (best[i].hostname != NULL && 
			    strstr(line, best[i].hostname) != NULL) {	/* Check for hostname */
				fputs(line, outfile_p);		/* if it's there, write to file */
				break;
			}
		}
		
		if ((ferror(infile_p) != 0) || (ferror(outfile_p) != 0))
			return 1;
		++i;
	}
	return 0;
}

/** 
 * Utility function 
 */
char *str_toupper(char *str)
{
	while (*str != '\0') {
		*str = toupper(*str);
		str++;
	}
	return str;
}
