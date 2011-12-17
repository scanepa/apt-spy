/*
 * apt-spy (c) Steven Holmes, 2003.
 * Stefano Canepa <sc@linux.it>, 2008, 2009
 *
 * This software is licensed as detailed in the COPYRIGHT file.
 */

#ifndef __GLOBAL_H
#define __GLOBAL_H
/*
extern char apt_spy_v[];
extern char d_out[];
extern char d_config[];
extern char d_mirror[];
*/

/* Defaults values */

/* Our version number. */
#define	apt_spy_v	"v3.2.2"

/* The default area */
#define	D_AREA		"All"

/* Default file locations */
#define D_OUT           "/etc/apt/sources.list.d/apt-spy.list"
#define D_CONFIG        "/etc/apt-spy.conf"
#define D_MIRROR        "/var/cache/apt/mirrors.txt"

/* Default file to grab when benchmarking */
#define D_FILE  	"ls-lR.gz"

/* Default update URL */
#define	D_UPDATE_URL 	"http://http.us.debian.org/debian/README.mirrors.txt"

#define	BESTNUMBER	5
extern int bestnumber;

#endif
