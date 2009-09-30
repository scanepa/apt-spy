/* 
 * apt-spy (c) Steven Holmes, 2003.
 *         (c) Stefano Canepa, 2008, 2009
 * This software is licensed as detailed in the COPYRIGHT file
 */

#include <stdio.h>
#include <curl/curl.h>

#include "include/update.h"

int update(FILE *mirror_p, char *update_url, char *proxy)
{
	CURL *curl;
	char errorbuff[CURL_ERROR_SIZE];
	int error;

	curl = curl_easy_init();
	
	if (curl == NULL)
		return 1;

	if (proxy != NULL)
		if (curl_easy_setopt(curl, CURLOPT_PROXY, proxy) != 0)
			return 1;

	/* Turn off libcurl's progress indicator */
	if (curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1) != 0)
		return 1;

	/* Fail on error */
	if (curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1) != 0)
		return 1;

	/* And give us nice, human-readable error messages */
	if (curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorbuff) != 0)
		return 1;

	/* set URL to use */
	if (curl_easy_setopt(curl, CURLOPT_URL, update_url) != 0)
		return 1;

	/* set file to output stuff to */
	if (curl_easy_setopt(curl, CURLOPT_FILE, mirror_p) != 0)
		return 1;

	printf("Updating...\n");
	printf("Grabbing file %s...\n", update_url);

	/* Do the transfer */
	if ((error = curl_easy_perform(curl)) != 0) {
		fprintf(stderr, "Error: %i: %s\n", error, errorbuff);
		return 1;
	}
	
	/* And cleanup */
	curl_easy_cleanup(curl);

	return 0;	
}
