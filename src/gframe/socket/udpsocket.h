//
//
// @ Project : gframe_socket
// @ File Name : udpsocket.h
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

#ifndef GFRAME_SOCKET_UDPSOCKET_H
#define GFRAME_SOCKET_UDPSOCKET_H

#include <gframe/socket/socket.h>
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
//#include <iostream>


class udpsocket : public socketbase
{
    public:
        udpsocket();
        virtual ~udpsocket();

        // Server initialization
        bool create();
};


#endif // GFRAME_SOCKET_UDPSOCKET_H