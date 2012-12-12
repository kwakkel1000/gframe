//
//
// @ Project : gframe_socket
// @ File Name : socket.cpp
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

#include <gframe/socket/socket.h>
#include "string.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>

void libgframe_socket_is_present(void){}

bool socketbase::bind ( const int port )
{
    if ( ! is_valid() )
    {
        return false;
    }

    m_addr.sin6_family = AF_INET6;
    m_addr.sin6_addr = in6addr_any;
    m_addr.sin6_port = htons ( port );

    int bind_return = ::bind ( m_sock, ( struct sockaddr * ) &m_addr, sizeof ( m_addr ) );

    if ( bind_return == -1 )
    {
        return false;
    }
    return true;
}

/*
bool socket::bind ( const std::string ip, const int port )
{
    if ( ! is_valid() )
    {
        return false;
    }

    m_addr.sin6_family = AF_INET6;
    m_addr.sin6_addr = ip.c_str();
    m_addr.sin6_port = htons ( port );

    int bind_return = ::bind ( m_sock,
                              ( struct sockaddr * ) &m_addr,
                              sizeof ( m_addr ) );

    if ( bind_return == -1 )
    {
        return false;
    }
    return true;
}*/



bool socketbase::send ( const std::string s ) const
{
    int status = ::send ( m_sock, s.c_str(), s.size(), MSG_NOSIGNAL );
    if ( status == -1 )
    {
        return false;
    }
    else
    {
        return true;
    }
}

int socketbase::recv ( std::string& data ) const
{
    int n = 1, total = 0, found = 0;
    //char c;
    char temp[1024*1024];

    // Keep reading up to a '\n'

    while (!found) {
        n = ::recv(m_sock, &temp[total], sizeof(temp) - total - 1, 0);
        if (n == -1) {
            /* Error, check 'errno' for more details */
            break;
        }
        total += n;
        temp[total] = '\0';
        found = (strchr(temp, '\n') != 0);
    }

    data = temp;
    return 1;
}

const socketbase& socketbase::operator >> ( std::string& data ) const
{
    if ( ! socketbase::recv ( data ) )
    {
        throw "Could not read from socket.";
    }
    return *this;
}

const socketbase& socketbase::operator << ( const std::string& data ) const
{
    if ( ! socketbase::send ( data ) )
    {
        throw "Could not write to socket.";
    }
    return *this;
}


bool socketbase::connect ( const std::string host, const int port )
{
    if ( ! is_valid() ) return false;

    m_addr.sin6_family = AF_INET6;
    m_addr.sin6_port = htons ( port );

    int status = inet_pton ( AF_INET6, host.c_str(), &m_addr.sin6_addr );

    if ( errno == EAFNOSUPPORT ) return false;

    status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

    if ( status == 0 )
        return true;
    else
        return false;
}

bool socketbase::set_non_blocking ( const bool b )
{
    int opts;

    opts = fcntl ( m_sock, F_GETFL );

    if ( opts < 0 )
    {
      return false;
    }

    if ( b )
    {
        opts = ( opts | O_NONBLOCK );
    }
    else
    {
        opts = ( opts & ~O_NONBLOCK );
    }
    fcntl ( m_sock, F_SETFL, opts );
    return true;
}

bool socketbase::set_reusable ( const int so_optval )
{
    if (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char *) &so_optval, sizeof(SO_REUSEADDR)) < 0)
        return false;
    return true;
}

bool socketbase::set_v6only ( const int v6only )
{
    if ( setsockopt ( m_sock, IPPROTO_IPV6, IPV6_V6ONLY, &v6only, sizeof(v6only)) < 0)
        return false;
    return true;
}
