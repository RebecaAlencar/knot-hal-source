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
 * MGMT: adapter setup and link layer generated events
 * UDATA: Sequenced and reliable protocol
 */
#define HAL_COMM_NRF24PROTO_MGMT	64
#define HAL_COMM_NRF24PROTO_UDATA	65

/* Serial port */
int hal_comm_open(const char *pathname);

/* nRF24 and other radios */
int hal_comm_socket(int domain, int protocol);

void hal_comm_close(int fd);

ssize_t hal_comm_read(int fd, void *buffer, size_t count);
ssize_t hal_comm_write(int fd, const void *buffer, size_t count);
