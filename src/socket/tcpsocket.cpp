//
//
// @ Project : gframe_socket
// @ File Name : tcpsocket.cpp
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
#include <gframe/socket/tcpsocket.h>
#include <gframe/glib.h>
#include <gframe/output.h>


tcpsocket::tcpsocket()
{
}

tcpsocket::~tcpsocket()
{
}

bool tcpsocket::create()
{
#ifdef HAVE_IPV6
    m_Sock = socket ( PF_INET6, SOCK_STREAM, IPPROTO_TCP );
#else
    m_Sock = socket ( PF_INET, SOCK_STREAM, IPPROTO_TCP );
#endif

    if ( ! is_valid() )
        return false;
#ifdef HAVE_IPV6
    if ( ! set_v6only(0) )
        return false;
#endif
    if ( ! set_reusable(1) )
        return false;
    return true;
}


int tcpsocket::recv ( std::string& data ) const
{
    static const int buffersize(256);
    int status = -1;//, total = 0, foundn = 0, foundr = 0;
    data = "";
    int length = 0;
    char buffer[buffersize];
    memset(buffer, '\0', buffersize);

    // Keep reading up to a '\r' or '\n'
    char c = '\0';
    //while ( (c != '\n') && (c != '\r') && (length < buffersize))
    while ( (c != '\n') && (length < buffersize))
    {
        status = ::recv(m_Sock, &c, sizeof(char), 0);
        output::instance().addOutput("status: " + glib::stringFromInt(status)  + " c: " + std::string(&c), 15);
        if ( status == -1 )
        {
            output::instance().addStatus(false, "status == -1   errno == " + glib::stringFromInt(errno) + "  in tcpsocket::recv");
            return 0;
        }
        else if ( status == 0 )
        {
            output::instance().addStatus(false, "status == 0    (TCP)no data in tcpsocket::recv");
            return 0;
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




bool tcpsocket::listen() const
{
    if ( ! is_valid() )
    {
        return false;
    }

    int listen_return = ::listen ( m_Sock, MAXCLIENTS );

    if ( listen_return == -1 )
    {
        return false;
    }
    return true;
}


/*bool tcpsocket::accept ( tcpsocket& new_socket ) const
{
    socklen_t socklen = sizeof(m_Addr6);
    // Accept the new connection
    int num_of_bytes = ::accept(m_Sock, (struct sockaddr *) &m_Addr6, &socklen);
    if (num_of_bytes == -1)
    {
        (void) inet_ntop(m_Addr6.sin6_family, m_Addr6.sin6_addr.s6_addr, buf, BUFLEN);
        (void) snprintf(s, BUFLEN, "V6 Accept failed for %s %d\0", buf, m_Addr6.sin6_port);
        perror(s);
    }

    int addr_length = sizeof ( m_Addr6 );
    new_socket.num_of_bytes = ::accept ( m_Sock, ( sockaddr * ) &m_Addr6, ( socklen_t * ) &addr_length );

    if ( new_socket.m_Sock <= 0 )
    {
        return false;
    }
    else
    {
        return true;
    }
}*/

void tcpsocket::init_listen()
{
    maxfd = (fileno(stdin) > m_Sock ? fileno(stdin) : m_Sock);
    nclients = 0;
}

void tcpsocket::send_client(int nclient, std::string data)
{
    ::send(clients[nclient], data.c_str(), data.size(), MSG_DONTWAIT);
}

int tcpsocket::select_listen(std::string& data, int& nclient)
{
    fd_set read_fds, write_fds, except_fds;
    int rval;
    char buf[BUFLEN];
    char s[BUFLEN];
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&except_fds);
    FD_SET(m_Sock, &read_fds);
    FD_SET(m_Sock, &except_fds);
    for (int nclients_iterator = 0; nclients_iterator < nclients; nclients_iterator++)
    {
        snprintf(s, BUFLEN, "FD_SET %d [%d] for read and exceptions.\n", nclients_iterator, clients[nclients_iterator]);
        FD_SET(clients[nclients_iterator], &read_fds);
        FD_SET(clients[nclients_iterator], &except_fds);
    }
    snprintf(s, BUFLEN, "Entering select with maxfd: %d\n", maxfd);
    /* Wait for someone to do something */
    select(maxfd + 1, &read_fds, &write_fds, &except_fds, NULL);

    /* Process an exception on the socket itself */
    if (FD_ISSET(m_Sock, &except_fds))
    {
        perror("Exception on socket.");
        fprintf(stderr, "Exiting.\n");
        return(7);
    }

    /* A read event on the socket is a new connection */
    if (FD_ISSET(m_Sock, &read_fds))
    {
#ifdef HAVE_IPV6
        socklen_t socklen = sizeof(m_Addr6);
        /* Accept the new connection */
        rval = ::accept(m_Sock, (struct sockaddr *) &m_Addr6, &socklen);
        if (rval == -1)
        {
            (void) inet_ntop(m_Addr6.sin6_family, m_Addr6.sin6_addr.s6_addr, buf, BUFLEN);
            //(void) snprintf(s, BUFLEN, "V6 Accept failed for %s %d\0", buf, m_Addr.sin6_port);
            (void) snprintf(s, BUFLEN, "V6 Accept failed for %s %d", buf, m_Addr6.sin6_port);
            perror(s);
        }
#else
        socklen_t socklen = sizeof(m_Addr);
        /* Accept the new connection */
        rval = ::accept(m_Sock, (struct sockaddr *) &m_Addr, &socklen);
        if (rval == -1)
        {
            (void) inet_ntop(m_Addr.sin_family, &(m_Addr.sin_addr), buf, BUFLEN);  // << faulty
            //(void) snprintf(s, BUFLEN, "V6 Accept failed for %s %d\0", buf, m_Addr.sin6_port);
            (void) snprintf(s, BUFLEN, "V4 Accept failed for %s %d", buf, m_Addr.sin_port);
            perror(s);
        }
#endif
        else
        {
            /* Too many clients? */
            if (nclients == MAXCLIENTS)
            {
                (void) ::send(rval, "Too many clients, please try later.\n",
                strlen("Too many clients, please try later.\n"), MSG_DONTWAIT);
                close(rval);
            }
            else
            {
                /* Add client to list of clients */
                clients[nclients++] = rval;
                if (rval > maxfd) maxfd = rval;
#ifdef HAVE_IPV6
                (void) inet_ntop(m_Addr6.sin6_family, m_Addr6.sin6_addr.s6_addr, buf, BUFLEN);
                snprintf(s, BUFLEN, "Accepted V6 connection from %s %d as %d\n",
                buf, m_Addr6.sin6_port, rval);
#else
                (void) inet_ntop(m_Addr.sin_family, &(m_Addr.sin_addr), buf, BUFLEN); // << faulty
                snprintf(s, BUFLEN, "Accepted V4 connection from %s %d as %d\n",
                buf, m_Addr.sin_port, rval);
#endif
                //snprintf(s, BUFLEN, "You are client %d [%d]. You are now connected.\n\0", nclients, rval);
                snprintf(s, BUFLEN, "You are client %d [%d]. You are now connected.\n", nclients, rval);
                ::send(rval, s, strnlen(s, BUFLEN), MSG_DONTWAIT);
            }
        }
    }
    rval = 0;
    /* Check for events from each client */
    for (int nclients_iterator = 0; nclients_iterator < nclients; nclients_iterator++)
    {
        snprintf(s, BUFLEN,  "Checking client %d [%d] for read indicator.\n",
        nclients_iterator, clients[nclients_iterator]);
        /* Client read events */
        if (FD_ISSET(clients[nclients_iterator], &read_fds))
        {
            snprintf(s, BUFLEN,  "Client %d [%d] marked for read.\n", nclients_iterator, clients[nclients_iterator]);
            perror(s);
            /* Read from client */
            if ((rval = ::recv(clients[nclients_iterator], buf, BUFLEN-1, MSG_DONTWAIT)) < 1)
            {
                //snprintf(s, BUFLEN, "Short recv %d octets from %d [%d]\0",  rval, nclients_iterator, clients[nclients_iterator]);
                snprintf(s, BUFLEN, "Short recv %d octets from %d [%d]",  rval, nclients_iterator, clients[nclients_iterator]);
                perror(s);
                /* Treat a 0 byte receive as an exception */
                FD_SET(clients[nclients_iterator], &except_fds);
            }
            buf[rval]=0;
            snprintf(s, BUFLEN,  "Received: %d (%lu) bytes containing %s", rval, strnlen(buf, BUFLEN), buf);
            data = std::string(buf);
            nclient = nclients_iterator;
            perror(s);
            /*t=time(NULL);
            ts=ctime(&t);
            ts[24]=0;
            snprintf(s, BUFLEN, "Client %d [%d] at %s: %s\0", nclients_iterator, clients[nclients_iterator],
            ts,
            buf);
            snprintf(u, BUFLEN,  "Message Length: %d, %s", strnlen(s, BUFLEN), s);*/
            /* Send the message to every other client */
//            for(j=0; j < nclients; j++)
//            {
//                /* Skip the sender */
//                if (j == i) continue;
//                /* Send the message */
//                send(clients[j], s, strnlen(s, BUFLEN), MSG_DONTWAIT);
//            }
        }
        /* Client eception events */
        if (FD_ISSET(clients[nclients_iterator], &except_fds))
        {
            /* Close the client connection */
            close(clients[nclients_iterator]);
            /* Flag the client as no longer connected */
            clients[nclients_iterator]=-1;
        }
    }
    /* Remove disconnected clients from list and recompute maxfd */
    maxfd = fileno(stdin);
    if (m_Sock > maxfd) maxfd = m_Sock;
    /* Iterate through and condense list of clients */
    for(int nclients_iterator = 0; nclients_iterator < nclients; nclients_iterator++)
    {
        if (clients[nclients_iterator] == -1)
        {
            snprintf(s, BUFLEN,  "Client: %d Removed.\n", nclients_iterator);
            perror(s);
            for(int nclients_iterator2 = nclients_iterator; nclients_iterator2 < nclients-1; nclients_iterator2++)
            {
                clients[nclients_iterator2]=clients[nclients_iterator2+1];
            }
            nclients--;
        }
        if (clients[nclients_iterator] > maxfd) maxfd = clients[nclients_iterator];
            snprintf(s, BUFLEN,  "End of loop %d / %d (%d)\n", nclients_iterator, nclients, maxfd);
        perror(s);
    }
    snprintf(s, BUFLEN,  "Finished removal loop (maxfd: %d).\n", maxfd);
    perror(s);
    return rval;
}

