//
//
// @ Project : gframe_socket
// @ File Name : socket.h
// @ Date : 29-10-2012
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

#ifndef GFRAME_SOCKET_SOCKET_H
#define GFRAME_SOCKET_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define BUFLEN 16384

class socketbase
{
    public:
        virtual bool set_non_blocking ( const bool );
        virtual bool set_reusable ( const int );
        virtual bool set_v6only ( const int );

        virtual bool is_valid() const { return m_sock != -1; }

        virtual bool bind ( const int port );
        //bool bind ( const std::string ip, const int port );
        /////bool listen() const;
        /////bool accept ( tcpsocket& ) const;

        // Client initialization
        bool connect ( const std::string host, const int port );


        // Data Transimission
        virtual bool send ( const std::string ) const;
        virtual int recv ( std::string& ) const;
        const socketbase& operator << ( const std::string& ) const;
        const socketbase& operator >> ( std::string& ) const;

    protected:

        int m_sock;
#ifdef HAVE_IPV6
        struct sockaddr_in6 m_addr6;
#else
        struct sockaddr_in m_addr;
#endif
};

#endif // GFRAME_SOCKET_SOCKET_H
