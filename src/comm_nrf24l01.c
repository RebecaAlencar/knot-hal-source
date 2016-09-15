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

#include "comm_private.h"
#include "nrf24l01.h"

static int nrf24l01_probe(void)
{
	return nrf24l01_init();
}

static void nrf24l01_remove(void)
{

}

static int nrf24l01_open(const char *pathname)
{
	/*
	 * Considering 16-bits to address adapter and logical
	 * channels. The most significant 4-bits will be used to
	 * address the local adapter, the remaining (12-bits) will
	 * used to address logical channels(clients/pipes).
	 * eg: For nRF24 '0' will be mapped to pipe0.
	 * TODO: Implement addressing
	 */

	return 0;
}

static size_t nrf24l01_recv(int sockfd, void *buffer, size_t len)
{
	uint8_t pipe;
	size_t length = -1;

	/* TODO: check if the data received is fragmented or not */

	/* If the pipe is available */
	if (nrf24l01_prx_pipe_available() == sockfd)
		/* Copy data to buffer */
		length = nrf24l01_prx_data(&buffer, len);

	/*
	 * On success, the number of bytes read is returned
	 * Otherwise, -1 is returned.
	 */
	return length;
}

static size_t nrf24l01_send(int sockfd, const void *buffer, size_t len)
{
	int err;

	/* TODO: break the buffer in parts if the len > NRF24_PW_SIZE*/

	nrf24l01_set_ptx(sockfd);
	nrf24l01_ptx_data(&buffer, len, true);
	err = nrf24l01_ptx_wait_datasent();

	nrf24l01_set_prx();

	return err;
}

struct phy_driver nrf24l01 = {
	.name = "nRF24L01",
	.probe = nrf24l01_probe,
	.remove = nrf24l01_remove,
	.open = nrf24l01_open,
	.recv = nrf24l01_recv,
	.send = nrf24l01_send
};
