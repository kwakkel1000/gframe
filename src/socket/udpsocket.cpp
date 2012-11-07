//
//
// @ Project : gframe_socket
// @ File Name : udpsocket.cpp
// @ Date : 06-11-2012
// @ Author : Gijs Kwakkel
//
//
// Copyright (c) 2012 Gijs Kwakkel
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//


// Implementation of the udpsocket class.


#include <gframe/socket/udpsocket.h>
#include "string.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>


udpsocket::udpsocket()
{
    m_sock = -1;
    memset ( &m_addr, 0, sizeof ( m_addr ) );
}

udpsocket::~udpsocket()
{
    if ( is_valid() )
        ::close ( m_sock );
}

bool udpsocket::create()
{
    m_sock = socket ( AF_INET6, SOCK_DGRAM, IPPROTO_UDP );

    if ( ! is_valid() )
        return false;
    if ( ! set_v6only(0) )
        return false;
    return true;
}
