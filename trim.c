/*
 * This file license has an history.
 * It was published by mistake with the following copyright note
 *
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
 Then after Paul Wise pointed out in his email on debian-mentors mailing list:
 http://lists.debian.org/debian-mentors/2011/12/msg00433.html

 I requested copyright holders the permission to use it with the following email:

 From: Stefano Canepa <sc@linux.it>
 To: Jan-Henrik Haukeland <hauk@tildeslash.com>,
 Martin Pala <martinp@tildeslash.com>,
 Christian Hopp <chopp@iei.tu-clausthal.de>,
 Michael Amster <mamster@webeasy.com>
 Subject: Need your help
 Organization: Linux.it + ALID.it
 User-Agent: Gnus/5.13 (Gnus v5.13) Emacs/23.3 (gnu/linux)
 Date: Sun, 25 Dec 2011 23:36:47 +0100
 Message-ID: <87liq0s2gw.fsf@lepre.stefanocanepa.home>
 MIME-Version: 1.0
 Content-Type: text/plain; charset=utf-8
 Content-Transfer-Encoding: quoted-printable

 Dear Sirs,
 I'm writing to you becouse I must admit I used your code in a wrong way
 by mistake, but I did.

 I'm trying to maintain apt-spy that is a little program that does some
 tests on debian mirrors and then writes a source file with the fastest
 mirror in it. The code I contribute to this program is not so much for
 me to retain the copyright so I must comply with old developer decision
 to have the code released under BSD three clause lincese.

 Indeed I did two mistakes:
 1) I used some of your code, with very little modification, from your
 util.h/util.c (ltrim, rtrim, trim) creating trim.{h,c} [1] releasing
 it under GPL but stripping stripped your copyright
 2) I linked this code with a BSD based code using OpenSSL enabled curl.

 Just now, after 2 years, under the guidance of Paul Wise of Debian
 understand this is all wrong.

 The solution to 2) should be to link curl without OpenSSL as apt-spy
 does not need OpenSSL at all

 But solution for 1) is not so easy.

 I understand that you can ask me to remove this portion of code that
 violate your rights and I'll do if you want but I prefer to not write
 this code again and I'm asking you for your permission to use your code
 with correct copyright in my project with correct copyright.

 Please accept my apologies.

 Best regards
 Stefano Canepa

 [1] https://github.com/scanepa/apt-spy/blob/master/trim.c

 To which I received the following reply:
 Delivered-To: scanepa@gmail.com
 Received: by 10.204.230.132 with SMTP id jm4cs221281bkb;
 Mon, 26 Dec 2011 04:32:25 -0800 (PST)
 Received: by 10.14.3.200 with SMTP id 48mr9409831eeh.94.1324902742348;
 Mon, 26 Dec 2011 04:32:22 -0800 (PST)
 Received-SPF: softfail (google.com: best guess record for domain of transitioning
 hauk@tildeslash.com does not designate 62.109.39.247 as permitted sender)
 client-ip=62.109.39.247;
 Received: by 10.14.153.77 with POP3 id e53mf10227544eek.29;
 Mon, 26 Dec 2011 04:32:22 -0800 (PST)
 X-Gmail-Fetch-Info: sc@linux.it 1 picard.linux.it 995 sc
 Return-Path: <hauk@tildeslash.com>
 X-Original-To: sc@linux.it
 Delivered-To: sc@vmbox.linux.it
 Received: from www.tildeslash.com (tildeslash.com [62.109.39.247])
 by picard.linux.it (Postfix) with ESMTP id 0AC263E64AA
 for <sc@linux.it>; Mon, 26 Dec 2011 13:30:13 +0100 (CET)
 Received: by www.tildeslash.com (Postfix, from userid 1003)
 id 420631D02AC; Mon, 26 Dec 2011 13:30:12 +0100 (CET)
 X-Spam-Checker-Version: SpamAssassin 3.2.5 (2008-06-10) on picard.linux.it
 X-Spam-Level: **
 X-Spam-Status: No, score=2.2 required=5.0 tests=DEAR_SOMETHING
 autolearn=disabled version=3.2.5
 Received: from [192.168.10.101] (120.79-161-188.customer.lyse.net [79.161.188.120])
 by www.tildeslash.com (Postfix) with ESMTPSA id 95EDA1D026A;
 Mon, 26 Dec 2011 13:30:02 +0100 (CET)
 Subject: Re: Need your help
 Mime-Version: 1.0 (Apple Message framework v1251.1)
 Content-Type: text/plain; charset=iso-8859-1
 From: Jan-Henrik Haukeland <hauk@tildeslash.com>
 In-Reply-To: <87liq0s2gw.fsf@lepre.stefanocanepa.home>
 Date: Mon, 26 Dec 2011 13:30:02 +0100
 Cc: Martin Pala <martinp@tildeslash.com>,
 Christian Hopp <chopp@iei.tu-clausthal.de>
 Content-Transfer-Encoding: quoted-printable
 Message-Id: <3483D682-2F4C-414F-9EC4-4AE2A8DF26EA@tildeslash.com>
 References: <87liq0s2gw.fsf@lepre.stefanocanepa.home>
 To: Stefano Canepa <sc@linux.it>
 X-Mailer: Apple Mail (2.1251.1)

 Permission granted. You may also change the license to BSD for this =
 particular code if you prefer.=20

 Best regards
 --
 Jan-Henrik Haukeland
 http://tildeslash.com/=20

 So at the end this file is licensed as follows:
 /*
 * (c) 2009, Stefano Canepa <sc@linux.it>
 *
 * This software is licensed as detailed in the COPYRIGHT file
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














