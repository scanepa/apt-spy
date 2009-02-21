/* 
 * apt-spy (c) Steven Holmes, 2003. 
 * This software is licensed as detailed in the COPYRIGHT file
 */

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <curl/curl.h>

#include "include/parse.h"
#include "include/protocols.h"

int get_file(server_t *current, CURL *curl, char *file, enum protocol protocol, size_t *total_bytes)
{
	char errorbuff[CURL_ERROR_SIZE + 1];
	int urlsize=0;
	char *url;
	double download_speed;
	double download_time;
	int error;

	/* Reset byte counter */
	*total_bytes = 0;
	
	/* Give us human readable error messages in errorbuff */
	if (curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorbuff) != 0)
		return 1;

	assert(HTTP==protocol || FTP==protocol);
	urlsize=8+strlen(current->hostname)+strlen(current->path[protocol])+strlen(file);
	url=malloc(urlsize);
	if (!url) {
		perror("malloc");
		exit(1);
	}

	if (HTTP==protocol)	strcpy(url, "http://");
	else if (FTP==protocol)	strcpy(url, "ftp://");
	
	strcat(url, current->hostname);

	/* Complete the URL. Also save it for later use when writing sources.list */
	if (protocol == HTTP) {
		strcat(url, current->path[HTTP]);
		current->url[HTTP]=strdup(url);
	} else {
		strcat(url, current->path[FTP]);
		current->url[FTP]=strdup(url);
	}

	strcat(url, file);
	
	/* And register it */
	if (curl_easy_setopt(curl, CURLOPT_URL, url) != 0) {
		free(url);
		return 1;
	}
	
	/* Now do the actual transfer */
	error = curl_easy_perform(curl);
	if (error != 0) {
		if (error != CURLE_OPERATION_TIMEDOUT) {
			free(url);
			fprintf(stderr, "\t\tError: %s\n", errorbuff);
			return 1;
		} else if (*total_bytes == 0) {
			free(url);
			fprintf(stderr, "\t\tError: %s\n", errorbuff);
			return 1;
		}			
	}
	
	/* And get the statistics and store them */
	curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &download_time);

	if (*total_bytes == 0) {
		fprintf(stderr, "\t\tNo data was transfered\n");
		free(url);
		return 1;
	}
	                                                
	printf("\t\tDownloaded %i bytes in %2.2f seconds\n", *total_bytes, download_time);

	/* Calculate download speed... */
	download_speed = (double) (*total_bytes) / download_time;

	printf("\t\tDownload speed: %.2f kB/sec\n", download_speed / (double) 1024);

	if (download_speed > current->stats.speed) {
		current->stats.speed = download_speed;
		current->stats.protocol = protocol;
	}	

	free(url);
	return 0;
}
