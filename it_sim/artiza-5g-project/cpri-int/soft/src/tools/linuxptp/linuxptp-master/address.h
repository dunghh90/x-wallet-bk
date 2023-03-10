/**
 * @file address.h
 * @brief Definition of a structure to hold an address.
 * @note Copyright (C) 2014 Red Hat, Inc., Jiri Benc <jbenc@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#ifndef HAVE_ADDRESS_H
#define HAVE_ADDRESS_H

#include <netinet/in.h>
#include <netpacket/packet.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <net/if_arp.h>

struct address {
	socklen_t len;
	union {
		struct sockaddr_storage ss;
		struct sockaddr_ll sll;
		struct sockaddr_in sin;
		struct sockaddr_in6 sin6;
		struct sockaddr_un sun;
		struct sockaddr sa;
	};
};

#endif
