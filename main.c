/* 
 * apt-spy (c) Steven Holmes, 2003. 
 * Stefano Canepa <sc@linux.it>, 2008, 2009
 * This software is licensed as detailed in the COPYRIGHT file
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#include "include/main.h"
#include "include/update.h"
#include "include/file.h"
#include "include/parse.h"
#include "include/benchmark.h"
#include "include/global.h"

int bestnumber=BESTNUMBER;

int main(int argc, char *argv[])
{
	int c; /* used to parse arguments */
	char *cur_entry; /* Entry we are benchmarking */

	char *distrib = NULL; /* distribution to use. */
	char *mirror_list = D_MIRROR; /* mirror list file */
	char *config_file = D_CONFIG; /* configuration file */
	char *proxy = NULL; /* Proxy server to use */
	char *infile = NULL; /* optional infile */
	char *outfile = D_OUT; /* outfile name */
	char *topfile = NULL;
	char *area = strdup(D_AREA); /* Area to test */
	char *grab_file = D_FILE; /* File to grab */
	char *update_url = D_UPDATE_URL; /* URL to use for updating */
	char *country_list = NULL; /* List of countries to b/m */
	char *section_list = NULL; /* List of section to include */
	FILE *infile_p, *outfile_p;	/* input/output file pointers */
	FILE *config_p; /* config file pointer */
	FILE *mirror_p; /* mirror list pointer */
	FILE *topfile_p;
	int timeout = 15; /* time to benchmark each server */
	int toplist = 0; /* Whether to write toplist. */
	int argsCount = 0; /* persistent counter for args */
	char *args[100]; /* persistent args array */
	char str[100] = "";
	int i;
	char *end;
	
	/* Number of servers to test. If negative, test them all. */
	int test_number = -1;		

	/* Server information structures. */
	server_t current, *best;

	/* Parse options... */
	while((c = getopt(argc, argv, "y:a:c:d:e:f:i:m:o:p:s:t:u:w:n:vh")) != -1)
	{
		if (optarg)
		{ /* not set with -h */
			for(i = 0; i < 100; i++)
				str[i] = '\0';
			strcpy(str,"");
			str[0] = '-'; str[1] = c; str[2] = ' ';
			strcat(str, optarg);
			args[argsCount] = (char*)malloc(strlen(str));
			strcpy(args[argsCount],str);
			argsCount++;
		}
		
		switch(c) {
		 /* Sections to include into apt-source */
		case 'y':
			section_list = optarg;
			break;
		 /* Area to benchmark */
		case 'a':
			area = optarg;
			break;
		/* Distribution we'll write into apt-sources. */
		case 'd':
			distrib = optarg;
			break;
		/* Configuration file to use */
		case 'c':
			printf("Option: %c\n",c);
			printf("Option arg: %s\n", optarg);
			/* config_file = (char *)malloc(25);
			strcpy(config_file, optarg);
			*/
			config_file = optarg;
			
			printf("config_file: %s\n", config_file);
			break;
		/* Number of servers to benchmark */
		case 'e':
			test_number = strtol(optarg, &end, 10);
			if (!*optarg || end!=optarg+strlen(optarg)) {
				fprintf(stderr, "Error parsing number"
						" of servers to be"
						" benchmarked\n");
				exit(1);
			}
			break;
		/* File, relative to Debian base, to grab from server. */
		case 'f':
			grab_file = optarg;				
			break;
		/* User-specified list of servers to benchmark. */
		case 'i':
			infile = optarg;
			break;
		/* The list of mirrors */
		case 'm':
			mirror_list = optarg;
			break;
		/* The output file we use */
		case 'o':
			outfile = optarg;
			break;
		/* Proxy server we should use */
		case 'p':
			proxy = optarg;
			break;
		/* List of countries to benchmark */
		case 's':
			country_list = optarg;
			break;
		/* Time for which to benchmark each server. */
		case 't':
			timeout = strtol(optarg, &end, 10);
			if (!*optarg || end!=optarg+strlen(optarg)) {
				fprintf(stderr, "Error parsing server"
						" benchmark time"
						" interval\n");
				exit(1);
			}
			break;
		/* The URL we should update ourselves from */					
		case 'u':
			update_url = optarg;
			break;
		/* Should we write a list of the "top" servers? */
		case 'w':
			toplist = 1;
			topfile = optarg;
			break;
		/* Number of servers to write in "top" server list */
		case 'n':
			bestnumber = strtol(optarg, &end, 10);
			if (!*optarg || end!=optarg+strlen(optarg)) {
				fprintf(stderr, "Error parsing number"
						" of best servers to"
						" write\n");
				exit(1);
			}
			break;
		case 'v':
			version();
			break;			
		/* Help!! */
		case 'h':
		default:
			usage();			/* display help */
			break;
		}
	}	
	argc -= optind;
	argv += optind;

	/* Simple check for stupidity */
	if ((test_number >= 0) && (bestnumber > test_number))
		bestnumber = test_number;

	best = malloc(sizeof(server_t) * (bestnumber + 1)); 

	if (best == NULL) {
		perror("malloc");
		exit(1);
	}

	/* We require an area and distribution argument if we are not updating */
	if ((argc == 0) && (distrib == NULL))
		usage();

	/* Check for silly combination of country and area arguments */
/*	if ((area != NULL) && (country_list != NULL))
		usage();
*/
	/* Setup default area argument */
/*	if ((area == NULL) && (country_list == NULL))
		area = d_area;
*/
	/* Setup default file argument if none given */
/*	if (grab_file == NULL)
		grab_file = d_file;
*/

	if (strcmp(area, D_AREA) && (country_list != NULL))
		usage();

	/* Open mirror file. We pass argc so it can tell if we're updating 
	   or not */
	mirror_p = select_mirror(mirror_list, argc);
	if (mirror_p == NULL) {
		perror("fopen");
		fprintf(stderr, "Error opening mirror file. Exiting.\n");
		exit(1);
	}

	/* the only possible argument now is "update", for updating the 
	   mirrors list */
	if (argc == 1) {
		if (strcmp(argv[0], "update") != 0)
			usage();
/* If necessary, set update_url to default */
		/*	if (update_url == NULL)
			update_url = d_update_url;
		*/
		if (update(mirror_p, update_url, proxy) != 0) {
			fprintf(stderr, "Update failed. Exiting.\n");
			exit(1);
		}
		printf("Update complete. Exiting.\n");
		exit(0);
	}
	
	/* argc should be 0. If not, there's something wrong. */
	if (argc != 0)
		usage();
	                                

	/* We open the infile. Either a temporary file, or a user-specified 
	   one. */
	infile_p = select_infile(infile);
	if (infile_p == NULL) {
		perror("tmpfile");
		fprintf(stderr, "Failed to open infile. Exiting.\n");
		exit(1);
	}

	/* Set up default if user hasn't specified an outfile */
/*	if (outfile == NULL)
		outfile = d_out;
*/
	/* Open the output file... */
	outfile_p = select_outfile(outfile);
	if (outfile_p == NULL) {
		perror("fopen");
		fprintf(stderr, "Error opening output file. Exiting.\n");
		exit(1);
	}

	/* Open the topfile */
	if (topfile) {
		topfile_p = select_outfile(topfile);
		if (topfile_p == NULL) {
			perror("fopen");
			fprintf(stderr, "Error opening topfile. Exiting.\n");
			exit(1);
		}
	}

	/* Open config file */
	config_p = select_config(config_file);
	if (config_p == NULL) {
		perror("fopen");
		fprintf(stderr, "Error opening config file. Exiting.\n");
		exit(1);
	}

	/* Fill temporary file with useful stuff if it's not user-specified. */
	if (infile == NULL) {
/*		if (area != NULL) {
			if (build_area_file(config_p, infile_p, mirror_p, area) != 0) {
				fprintf(stderr, 
				"Error building area file. Exiting.\n");
				exit(1);
			}
		} else {
*/
		if (country_list) {
			if (build_country_file(config_p, infile_p, mirror_p,
			    country_list) != 0) {
				fprintf(stderr,
				"Error building country file. Exiting.\n");
				exit(1);
			}

		} else {
			if (build_area_file(config_p, infile_p, mirror_p, area) != 0) {
				fprintf(stderr, 
						"Error building area file. Exiting.\n");
				exit(1);
			}
		}		
	}
	
	/* Make sure we're at the beginning... */
	rewind(infile_p);

	/* Zero the "best" structure */
	for (c = 0; c < bestnumber; c++)
		memset(&best[c], 0, sizeof(server_t));

	/* This is the main loop. It'll exit when we've exhausted the URL 
	   list or test_number is 0 */

	while (((cur_entry = next_entry(infile_p)) != NULL) && test_number != 0) {
		if (ferror(infile_p) != 0) {
			fprintf(stderr, "Error while reading input file\n");
			exit(1);
		}

		/* Turn entry into a pretty structure */
		tokenise(&current, cur_entry);

		/* Do the benchmark */
		if (benchmark(&current, proxy, timeout, grab_file) != 0) {
			fprintf(stderr, 
			"Error while performing benchmark. Exiting.\n");
			exit(1);
		}

		decide_best(&current, best);
		free(cur_entry);

		if (test_number > 0)
			--test_number;
	}

	/* write the results */
        printf("Writing new sources.list file: %s \n", outfile);
	/*
	 * sc
	 */
/*	if (write_list(outfile_p, best, distrib) != 0) { */
	if (write_list(outfile_p, best, distrib, section_list, args, argsCount) != 0) {
	fprintf(stderr, "Error writing output file. Exiting.");
		exit(1);
	}
	/* close the file */
	fclose(outfile_p);

	/* We write out the top servers to a file if asked. Note there's no 
	   point in freeing the "best" structures. */
	if (topfile) {
	        printf("writing topfile: %s\n", topfile);

		if (write_top(infile_p, topfile_p, best) != 0) {
			fprintf(stderr, 
			"Error writing top servers list. Exiting.");
			exit(1);
		}
		/* close the file */
		fclose(topfile_p);
	}
	/* We're all done */
	free(best);
	
	exit(0);
}

void usage()
{
	printf("Usage: apt-spy [options]\n"
		"options:\n"
		"-d distribution\tDebian distribution (ie, stable). Required unless updating.\n"
		"-a area\t\tArea to benchmark. (eg, Europe).\n"
		"-c config\tConfiguration file to use.\n"
		"-e number\tNumber of servers to benchmark before exiting.\n"
		"-y section-list\tComma separated list of section other then main to include.\n");
	printf("-f file\t\tFile to grab when benchmarking. (relative to Debian base).\n"
		"-i file\t\tSpecify input file. For use with the -w option.\n"
		"-m mirror-list\tMirror list to use, or mirror-list to update when updating.\n"
		"-o output-file\tWhere to put output.\n"
		"-p proxy\tProxy server to use. In format <server>:[port]\n"
		"-s country_list\tList of countries to benchmark. Cannot be used with -a.\n"
		"-t time\t\tTime to benchmark each server for. An approximate value only.\n"
		"-u update-URL\tURL to grab mirror-list from when updating.\n");
	printf("-w file\t\tOutput top servers (5 by default) to file for use with -i.\n"
		"-n number\tSpecifies number of top servers to output with -w.\n"
		"-v\t\tOutput a version number.\n"
		"-h\t\tDisplay this message.\n"
		"update\t\tUpdate the mirror list.\n");
	exit(0);
}

void version()
{
	printf("apt-spy %s\n", apt_spy_v);
	exit(0);
}
