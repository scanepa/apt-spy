/* apt-spy (c) Steven Holmes, 2003.
 * (c) Stefano Canepa, 2008, 2009
 * 
 * This software is licensed as detailed in the COPYRIGHT file. 
 */

#ifndef __BENCHMARK_H
#define __BENCHMARK_H

int benchmark(server_t *current, char *proxy, int timeout, char *file);
void decide_best(server_t *current, server_t *best);
size_t null_write_function(double *bytes, size_t size, size_t nmemb, void *stream);

#endif
