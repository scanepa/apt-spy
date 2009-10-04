/*
 * apt-spy (c) Steven Holmes, 2003.
 *         (c) Stefano Canepa <sc@linux.it>, 2008, 2009.
 *
 * This software is licensed as detailed in the COPYRIGHT file 
 */

#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#include "include/parse.h"
#include "include/benchmark.h"
#include "include/protocols.h"
#include "include/global.h"

/* 
 * It is safer to keep track of the total amount of data read ourselves.
 * This fixes a bug appearing when libcurl would not reset its own byte counter
 */
/* static size_t total_bytes = 0; */
static int total_bytes = 0;

/** 
 * 
 * 
 * @param current 
 * @param proxy 
 * @param timeout 
 * @param file 
 * 
 * @return 
 */
int benchmark(server_t *current, char *proxy,  int timeout, char *file)
{

	CURL *curl;
	
	printf("\nSERVER:\t%s\n", current->hostname);
	
	curl_global_init (CURL_GLOBAL_ALL);
	/* We use libcurl - here we setup some global options */
	curl = curl_easy_init();
	
	if (curl == NULL)
		return 1;
   
	/* Set proxy server to use */
	if (proxy != NULL)
		if (curl_easy_setopt(curl, CURLOPT_PROXY, proxy) != 0)
			return 1;

	/* Connection timeout */
	if (curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, (long) timeout) != 0)
		return 1;
	
	/* Total timeout */
	if (curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long) timeout) != 0)
		return 1;

	/* Select callback function to deal with data. */
	if (curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, null_write_function) != 0)
		return 1;
	
	/* Turn off libcurl's progress indicator */
	if (curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1) != 0)
		return 1;

	/* Fail on error */
	if (curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1) != 0)
		return 1;

	
	/* Test for an FTP entry */
	if (strlen(current->path[FTP]) != 0) {
		printf("Benchmarking FTP...\n");
		get_file(current, curl, file, FTP, &total_bytes);
	}
	
	/* Test for an HTTP entry */
	else if (strlen(current->path[HTTP]) != 0) {
		printf("Benchmarking HTTP...\n");
		get_file(current, curl, file, HTTP, &total_bytes);
	} else {
		printf("UNKNOWN TYPE\n");
	}

	curl_easy_cleanup(curl);

	return 0;	
}

/** 
 * 
 * 
 * @param current 
 * @param best 
 */
void decide_best(server_t *current, server_t *best)
{
	int i, j;
	
	/* Inefficient sorting algorithm, but small number of entries so
	   it doesn't matter. */

	/* move 'i' to the correct place in the array to place the new entry */
	for (i = 0; i < bestnumber; ++i)
		if (current->stats.speed > best[i].stats.speed)
			break;
	
	/* shove everything along one */
	for (j = bestnumber - 2; j >= i; --j)
		memcpy(&best[j + 1], &best[j], sizeof(server_t));
	
	/* copy the new entry into the correct place */
	memcpy(&best[i], current, sizeof(server_t));	

}

/** 
 * @brief Callback from libcurl to allow us to save data. We discard it and
 *        return success. 
 * 
 * @param bytes 
 * @param size 
 * @param nmemb 
 * @param stream 
 * 
 * @return 
 */
size_t null_write_function(double *bytes, size_t size, size_t nmemb, void *stream)
{
	/* We update total_bytes */
	total_bytes += size * nmemb;
	return size * nmemb;
}
