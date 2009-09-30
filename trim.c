
/* 
 * trim.c --- short description
 * 
 * Copyright  (C)  2009  Stefano Canepa <sc@linux.it>
 * 
 * Version: 1.0
 * Keywords: 
 * Author: Stefano Canepa <sc@linux.it>
 * Maintainer: Stefano Canepa <sc@linux.it>
 * URL: http://
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY	; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Commentary: 
 * The following code is a modification of the Util_ltrim, Util_rtrim
 * and Util_trim functions of util.{c,h} of the monit project
 * http://savannah.nongnu.org/projects/monit/ released with the same
 * license as this project
 *
 * 
 * 
 * 
 * Code:
 */


/**
 * Remove leading char from string
 *
 * @param s A string
 * @param d The char to remove
 *
 * @return s with leading spaces removed
 */

char *ltrim(char *s, const int d)
{
	 char *t= s;

	 if (d == ' ')
		  while(*t==' ' || *t=='\t' || *t=='\r' || *t=='\n') t++;
	 else
		  while(*t==d) t++;
	 if(t!=s) {
		  char *r= s;
		  do {
			   *r++= *t;
		  }
		  while(*t++);
	 }
	 return s;
}

/**
 * Remove trailing char from the string
 *
 * @param s A string
 * @param d The char to remove
 *
 * @return s with trailing spaces removed
 */
char *rtrim(char *s, const int d) {

	char *t= s;


	while(*s) s++;
	if (d == ' ')
		while(*--s==' ' || *s=='\t' || *s=='\r' || *s=='\n') *s= 0;
	else
		while(*--s==d) *s= 0;
	return t;

}

/**
 * Remove leading and trailing space from the string
 * @param s A string
 * @return s with leading and trailing spaces removed 
 */
char *trim(char *s, const char d) {

	char *t,*r;
	
	t = ltrim(s,d);
	r= rtrim(t,d);

	return r;
  
}














