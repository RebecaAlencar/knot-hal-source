/*
 * This file is part of the KNOT Project
 *
 * Copyright (c) 2016, CESAR. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of the CESAR nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL CESAR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/eventfd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "comm_private.h"
#include "physerial.h"

struct serial_opts {
	char	tty[24];
	int	vmin;
	int	virtualfd;
	int	realfd;
	int	knotd;
};

static struct serial_opts serial_opts;

/* Check if tty path is available*/
static int serial_probe(void)
{
	struct stat st;
	int err = 0;

	if (stat(serial_opts.tty, &st) < 0) {
		err = errno;
		printf("serial stat(): %s(%d)\n", strerror(err), err);
	}

	return -err;
}


/* Resturns last character from the pathname, like 0 in /tty/ttyUSB0 */
static int serial_open(const char *pathname)
{
	return serial_opts.tty[strlen(serial_opts.tty)-1] - '0';
}

static void serial_remove(void)
{
}

static int serial_listen(int sock)
{
	struct termios term;
	int ttyfd;

	memset(&term, 0, sizeof(term));

	term.c_iflag = ~(IXON | IXOFF | IXANY);
	term.c_oflag = ~OPOST;
	term.c_cflag = CRTSCTS | CS8 | CLOCAL | CREAD;
	term.c_lflag = ~(ICANON | ECHO | ECHOE | ISIG);
	term.c_cc[VMIN] = serial_opts.vmin;
	term.c_cc[VTIME] = 0;

	cfsetospeed(&term, B9600);
	cfsetispeed(&term, B9600);

	ttyfd = open(serial_opts.tty, O_RDWR | O_NOCTTY);
	if (ttyfd < 0)
		return -errno;

	tcsetattr(sock, TCSANOW, &term);

	return ttyfd;
}

static int serial_accept(int srv_sockfd)
{
	/* Map virtual fd to pipe */
	return 0;
}

static int serial_connect(int sock, uint8_t to_addr)
{
	/*TODO*/
	return 0;
}

static ssize_t serial_recv(int sockfd, void *buffer, size_t len)
{
	return read(sockfd, buffer, len);
}

static ssize_t serial_send(int sockfd, const void *buffer, size_t len)
{
	return write(sockfd, buffer, len);
}

/* close no virtual marca na tabela */
/* close no real realmente fecha */
static void serial_close(int fd)
{
	close(fd);
}

struct phy_driver phy_serial = {
	.name = "Serial",
	.open = serial_open,
	.probe = serial_probe,
	.remove = serial_remove,
	.close = serial_close,
	.listen = serial_listen,
	.accept = serial_accept,
	.connect = serial_connect,
	.recv = serial_recv,
	.send = serial_send
};

int serial_load_config(const char *tty)
{
	memset(&serial_opts, 0, sizeof(serial_opts));
	strncpy(serial_opts.tty, tty, sizeof(serial_opts.tty));
	serial_opts.vmin = 8; /* 8 octets */

	return 0;
}
