/* apt-spy (c) Steven Holmes, 2003.
 * This software is licensed as detailed in the COPYRIGHT file.
 */

#ifndef __PROTOCOLS_H
#define __PROTOCOLS_H

#include "parse.h" /* enum protocol */

int get_file(server_t *current, CURL *curl, char *file, enum protocol protocol, int *total_bytes);

#endif
