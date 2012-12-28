//
//
// @ Project : gframe_socket
// @ File Name : tcpsocket.h
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

#ifndef GFRAME_SOCKET_TCPSOCKET_H
#define GFRAME_SOCKET_TCPSOCKET_H

#include <gframe/socket/socket.h>
#include <string>

#define MAXCLIENTS 512

class tcpsocket : public socketbase
{
    public:
        tcpsocket();
        virtual ~tcpsocket();

        // Server initialization
        bool create();

        virtual int recv (std::string&) const;

        bool listen() const;
        bool accept ( tcpsocket& ) const;

        void init_listen();
        int select_listen(std::string&, int&);
        void send_client(int, std::string);
    private:
        int nclients;
        int maxfd;
        int clients[MAXCLIENTS];
};


#endif // GFRAME_SOCKET_TCPSOCKET_H

