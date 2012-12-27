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

#include <gframe/config.h>
#include <gframe/socket/socket.h>
#include <gframe/output.h>
#include <gframe/glib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>

extern "C" void libgframe_socket_is_present(void)
{
}

socketbase::socketbase () : m_Sock(-1)
{
    std::lock_guard<std::mutex> addrlock(m_AddrMutex);
#ifdef HAVE_IPV6
    memset(&m_Addr6, 0, sizeof(m_Addr6));
#else
    memset ( &m_Addr, 0, sizeof ( m_Addr ) );
#endif
}

socketbase::~socketbase ()
{
    if ( is_valid() )
        ::close ( m_Sock );
}

bool socketbase::bind ( const int port )
{
    std::lock_guard<std::mutex> addrlock(m_AddrMutex);
    if ( ! is_valid() )
    {
        return false;
    }
#ifdef HAVE_IPV6
    m_Addr6.sin6_family = AF_INET6;
    m_Addr6.sin6_addr = in6addr_any;
    m_Addr6.sin6_port = htons ( port );

    int bind_return = ::bind ( m_Sock, ( struct sockaddr * ) &m_Addr6, sizeof ( m_Addr6 ) );
#else
    m_Addr.sin_family = AF_INET;
    m_Addr.sin_addr.s_addr = INADDR_ANY;
    m_Addr.sin_port = htons ( port );

    int bind_return = ::bind ( m_Sock, ( struct sockaddr * ) &m_Addr, sizeof ( m_Addr ) );
#endif

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

    m_Addr.sin6_family = AF_INET6;
    m_Addr.sin6_addr = ip.c_str();
    m_Addr.sin6_port = htons ( port );

    int bind_return = ::bind ( m_Sock,
                              ( struct sockaddr * ) &m_Addr,
                              sizeof ( m_Addr ) );

    if ( bind_return == -1 )
    {
        return false;
    }
    return true;
}*/



bool socketbase::send ( const std::string s ) const
{
    int status = ::send ( m_Sock, s.c_str(), s.size(), MSG_NOSIGNAL );
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
    static const int buffersize(256);
    int status = -1;//, total = 0, foundn = 0, foundr = 0;
    data = "";
    int length = 0;
    char buffer[buffersize];
    memset(buffer, '\0', buffersize);

    // Keep reading up to a '\r' or '\n'
    char c = '\0';
    while ( (c != '\n') && (length < buffersize))
    {
        status = ::recv(m_Sock, &c, sizeof(char), 0);
        if ( status == -1 )
        {
            output::instance().addStatus(false, "status == -1   errno == " + glib::stringFromInt(errno) + "  in socketbase::recv");
            return 0;
        }
        else if ( status == 0 && !m_RecvNullOk)
        {
            return 0;
        }
        else if ( status == 0 && m_RecvNullOk)
        {
            buffer[length] = c;
            data = buffer;
            return 1;
        }
        if ((c != '\n') && (c != '\r'))
        {
            buffer[length] = c;
            length++;
            if (length == buffersize-1)
            {
                data += std::string(buffer);
                memset(buffer, '\0', buffersize);
                length = 0;
            }
        }
    }
    data += buffer;
    return status;
}

const socketbase& socketbase::operator >> ( std::string& data ) const
{
    if ( ! socketbase::recv ( data ) )
    {
        output::instance().addStatus(false, "Could not read from socket.");
        throw "Could not read from socket.";
    }
    return *this;
}

const socketbase& socketbase::operator << ( const std::string& data ) const
{
    if ( ! socketbase::send ( data ) )
    {
        output::instance().addStatus(false, "Could not write to socket.");
        throw "Could not write to socket.";
    }
    return *this;
}


bool socketbase::connect ( const std::string host, const int port )
{
    std::string service = glib::stringFromInt(port);
    if ( ! is_valid() ) return false;

    struct addrinfo *res, *aip;
    struct addrinfo hints;
    int error;

    // Get host address. Any type of address will do
    bzero(&hints, sizeof(hints));
    hints.ai_flags = AI_ALL|AI_ADDRCONFIG;
    hints.ai_socktype = SOCK_STREAM;

    error = getaddrinfo(host.c_str(), service.c_str(), &hints, &res);
    if (error != 0)
    {
        //throw Exception("Could not get address info", errno);
        return false;
    }

    // Try all returned addresses until one works
    for (aip = res; aip != NULL; aip = aip->ai_next)
    {
        // Open socket. The address type depends on what getaddrinfo() gave us
        m_Sock = socket(aip->ai_family, aip->ai_socktype, aip->ai_protocol);
        if (m_Sock == -1)
        {
            m_Sock = 0;
            freeaddrinfo(res);
            //throw Exception("Could not ceate socket", errno);
            return false;
        }

        // Connect to the host
        if (::connect(m_Sock, aip->ai_addr, aip->ai_addrlen) == -1)
        {
            close(m_Sock);
            m_Sock = 0;
        }
        else
        {
            // Store connectiond data
            /*
cout << "Connected to " << aip->ai_canonname << "! flags=" << aip->ai_flags
<< " family=" << aip->ai_family << " socktype=" << aip->ai_socktype
<< " protocol=" << aip->ai_protocol << endl;
*/
            /*
struct addrinfo {
int ai_flags;
int ai_family;
int ai_socktype;
int ai_protocol;
size_t ai_addrlen;
struct sockaddr *ai_addr;
char *ai_canonname;
struct addrinfo *ai_next;
};
m_hostname = phost->h_name;
m_port = port;
*/

            break; // break out of for loop, because we managed to connect!
        }
    }

    // Clean up :+
    freeaddrinfo(res);

    if (!m_Sock)
    {
        //throw Exception("Could not connect to host");
        return false;
    }
    return true;




/*


#ifdef HAVE_IPV6
    m_Addr6.sin6_family = AF_INET6;
    m_Addr6.sin6_port = htons ( port );

    int status = inet_pton ( AF_INET6, host.c_str(), &m_Addr6.sin6_addr );

    if ( errno == EAFNOSUPPORT ) return false;

    status = ::connect ( m_Sock, ( sockaddr * ) &m_Addr6, sizeof ( m_Addr6 ) );
#else
    m_Addr.sin_family = AF_INET;
    m_Addr.sin_port = htons ( port );

    int status = inet_pton ( AF_INET, host.c_str(), &m_Addr.sin_addr );

    if ( errno == EAFNOSUPPORT ) return false;

    status = ::connect ( m_Sock, ( sockaddr * ) &m_Addr, sizeof ( m_Addr ) );
#endif
    if ( status == 0 )
        return true;
    else
        return false;*/
}

bool socketbase::disconnect()
{
    if (m_Sock)
    {
        close(m_Sock);
        m_Sock = 0;
        return true;
    }
    return false;
}

bool socketbase::set_non_blocking ( const bool b )
{
    int opts;

    opts = fcntl ( m_Sock, F_GETFL );

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
    fcntl ( m_Sock, F_SETFL, opts );
    return true;
}

bool socketbase::set_reusable ( const int so_optval )
{
    if (setsockopt(m_Sock, SOL_SOCKET, SO_REUSEADDR, (char *) &so_optval, sizeof(SO_REUSEADDR)) < 0)
        return false;
    return true;
}

bool socketbase::set_v6only ( const int v6only )
{
    if ( setsockopt ( m_Sock, IPPROTO_IPV6, IPV6_V6ONLY, &v6only, sizeof(v6only)) < 0)
        return false;
    return true;
}
