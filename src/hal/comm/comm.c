/*
 * Copyright (c) 2016, CESAR.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 *
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef ARDUINO
#include <avr_errno.h>
#include <avr_unistd.h>
#else
#include <errno.h>
#include <unistd.h>
#endif
#include "include/comm.h"
#include "src/comm_private.h"

static struct phy_driver *driver = NULL;
int comm_domain;

int hal_comm_init(int domain)
{
	comm_domain = domain;
	switch(domain){
		/*init nrf24l01 */
	case HAL_COMM_PF_NRF24:
		driver = &nrf24l01;
		break;
	}
}

int hal_comm_open(const char *pathname) //"/dev/spidev0.0"
{
	int retval, fd;

	switch(comm_domain){
	case HAL_COMM_PF_NRF24:
		/* FIXME: pass 'spi' to driver */
		retval= driver->probe();
		if (retval < 0)
			return retval;

		/* nRF24 config for broadcast */
		fd = driver->listen(0);
		if (fd < 0) {
			driver->remove();
			return fd;
		}
		break;
	}

	return fd;
}

ssize_t hal_comm_read(int sockfd, void *buffer, size_t count)
{

	return driver->recv(sockfd, buffer, count);
}
