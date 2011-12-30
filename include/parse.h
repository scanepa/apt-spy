/* 
 * apt-spy (c) Steven Holmes, 2003. 
 * This software is licensed as detailed in the COPYRIGHT file. 
 */
 
#ifndef __PARSE_H
#define __PARSE_H

enum protocol { FTP, HTTP };

/* hack */
/*extern int BESTNUMBER;
 */

struct stats_struct {
	int protocol;
	double speed;
};

struct server_struct {
	char	*hostname;
	char	*path[2];
	char	*url[2];
	struct	stats_struct stats;
};

typedef struct server_struct server_t;

char *str_toupper(char *str);
int build_area_file(FILE *config_p, FILE *infile_p, FILE *mirror_list,
					char *area);
int build_country_file(FILE *config_p, FILE *infile_p, FILE *mirror_p,
					   char *country_list);
int find_country(FILE *mirror_list, char *country_code);
void tokenise(server_t *current, char *cur_entry);
char *get_mirrors(FILE *mirror_list);
int write_list(FILE *outfile_p, server_t *best, char *dist, char *section_list, char *args[], int l);
int write_top(FILE *infile_p, FILE *outfile_p, server_t *best);

#endif
