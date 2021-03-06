/*
 * Copyright (c) 2016, CESAR.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 *
 */

/* Domain: selects radio technology */
#define HAL_COMM_PF_NRF24		1

/*
 * UDATA: Sequenced and reliable protocol
 */
#define HAL_COMM_NRF24PROTO_UDATA	64

/* Serial port */
int hal_comm_open(const char *pathname);

/*
 * nRF24 and other radios. Returns -ENFILE (limit of resources/pipes has
 * been reached) or an id representing the logical communication channel
 * for a new client.
 */
int hal_comm_socket(int domain, int protocol);

void hal_comm_close(int sockfd);

/* Non-blocking read operation. Returns -EGAIN if there isn't data available */
ssize_t hal_comm_read(int sockfd, void *buffer, size_t count);

/* Blocking write operation. Returns -EBADF if not connected */
ssize_t hal_comm_write(int sockfd, const void *buffer, size_t count);

int hal_comm_listen(int sockfd);

/* Non-blocking operation. Returns -EGAIN if there isn't a new client */
int hal_comm_accept(int sockfd, uint64_t *addr);

/* Blocking operation. Returns -ETIMEOUT */
int hal_comm_connect(int sockfd, uint64_t *addr);
