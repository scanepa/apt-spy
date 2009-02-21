/* apt-spy (c) Steven Holmes, 2003. This software is licensed as detailed in the LICENSE file. */

#ifndef __UPDATE_H
#define __UPDATE_H

#include <curl/curl.h>

int update(FILE *mirror_p, char *update_url, char *proxy);

#endif
