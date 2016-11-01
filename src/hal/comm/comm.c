/*
 * Copyright (c) 2016, CESAR.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 *
 */

#include "comm_private.h"

static struct phy_driver *driver = NULL;

int hal_comm_init(int domain)
{
	switch(domain){
	case HAL_COMM_PF_NRF24:
		driver = &nrf24l01;
		break;
	}
}
