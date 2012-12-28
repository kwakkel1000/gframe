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

#include <gframe/config.h>
#include <gframe/socket/udpsocket.h>
#include <gframe/glib.h>
#include <gframe/output.h>

udpsocket::udpsocket()
{
}

udpsocket::~udpsocket()
{
}

bool udpsocket::create()
{
#ifdef HAVE_IPV6
    m_Sock = socket ( AF_INET6, SOCK_DGRAM, IPPROTO_UDP );
#else
    m_Sock = socket ( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
#endif
    if ( ! is_valid() )
        return false;
#ifdef HAVE_IPV6
    if ( ! set_v6only(0) )
        return false;
#endif
    return true;
}

int udpsocket::recv ( std::string& data ) const
{
    int status = -1, total = 0, found = 0;
    data = "";
    char buffer[1024*1024];

    // Keep reading up to a '\r' or '\n'

    while ( !found ) {
        status = ::recv(m_Sock, &buffer[total], sizeof(buffer) - total - 1, 0);
        if ( status == -1 ) {
            output::instance().addStatus(false, "status == -1 errno == " + glib::stringFromInt(errno) + " in udpsocket::recv");
            /* Error, check 'errno' for more details */
            //break;
            return 0;
        }
        else if ( status == 0)
        {
            total += status;
            buffer[total] = '\0';
            data = buffer;
            return 1;
        }
        total += status;
        buffer[total] = '\0';
        found = (strchr(buffer, '\r') != 0 || strchr(buffer, '\n') != 0);
    }
    data = buffer;
    return status;
}
