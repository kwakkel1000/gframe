
//
//
// @ Project : gframe_database
// @ File Name : mysqldatabase.cpp
// @ Date : 05-11-2012
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

#include <iostream>
#include <cstddef>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

//#include "../include/database/mysqldatabase.h"
#include <gframe/database/mysqldatabase.h>

/**************
* Purpose: default constructor, initialize class values
* Precondition: none
* Postcondition: none
**************/
mysqldatabase::mysqldatabase()
{
    // initilize the mysql fields
    sock = NULL;
    row = NULL;
    result = NULL;
    field = NULL;
}

/**************
* Purpose: connect to the appropriate database
* Precondition: host, database, user name and password
* Postcondition: connected to that database
**************/
//int mysqldatabase::connect(const char *host, const char *db, const char *user, const char *pass)
int mysqldatabase::connect(std::vector<const char*> Arguments)
{
    if (Arguments.size() == 4)
    {
        const char *host = Arguments[0];
        const char *db = Arguments[1];
        const char *user = Arguments[2];
        const char *pass = Arguments[3];
        if (sock) // already connected to another database
            disconnect(); // disconnect from that one

        // initilize the socket
        sock = mysql_init(0);

        // something went wrong with the socket
        if (!sock)
            return ERR201;

        // try connecting to the database
        if (!mysql_real_connect(sock, host, user, pass, db, 0, NULL, 0))
            return ERR202;

        // successfully connected to the database
        return SUCCESS;
    }
    return 0;
}

/**************
* Purpose: disconnect from the database
* Precondition: none
* Postcondition: socket closed
**************/
bool mysqldatabase::disconnect()
{
    if (sock) // they have a socket open
        mysql_close(sock);

    sock = NULL;

    // we dont use result anymore
    // release result data
    //if (result)
    //{
    //    mysql_free_result(result);
    //}
    //free();

    // database disconnected
    return true;
}

/************
*return true if connected
************/
bool mysqldatabase::connected()
{
    if (sock)
    {
        return true;
    }
    return false;
}


/**************
* Purpose: free the results from the database query
* Precondition: none
* Postcondition: results no longer there
**************/
/*bool mysqldatabase::free()
{
    if (result)
    {
        mysql_free_result(result);
        return true;
    }

    return false;
}*/

/**************
* Purpose: return the approprite error message
* Precondition: error code
* Postcondition: string with the error returned
**************/
static std::string suc_str = "SUCCESS";
static std::string e201_str = "201 SOCKET ERROR: SOCKET FAILURE";
static std::string e202_str = "202 CONNECTION ERROR: CANNOT ACCESS THE SERVER";
static std::string e203_str = "203 DATABASE ERROR: QUERY FAILED";
const char *dberror(int errorcode)
{
    // display the appropriate error message for this error
    switch (errorcode)
    {
        case SUCCESS: return suc_str.c_str();
        break;

        case ERR201: return e201_str.c_str();
        break;

        case ERR202: return e202_str.c_str();
        break;

        case ERR203: return e203_str.c_str();
        break;
    }

    return NULL; // no error, return null char
}

/**************
* Purpose: return the result set of the query
* Precondition: the query string
* Postcondition: result set returned (or null)
**************/
/*MYSQL_RES *mysqldatabase::query(const char *query)
{
    // query the database
    mysql_query(sock, query);

    // store the results
    result = mysql_store_result(sock);

    return result;
}*/

std::vector< std::vector< std::string > > mysqldatabase::get(const char *query)
{
    std::vector< std::vector< std::string > > sql_result;
    int query_state = mysql_query(sock, query);
    if (!query_state)
    {
        result = mysql_store_result(sock);
        int num_fields;
        int i;
        num_fields = mysql_num_fields(result);
        while ( ( row = mysql_fetch_row(result)) != NULL )
        {
            std::vector< std::string > tmp;
            for (i = 0; i < num_fields; i++)
            {
                if (row[i] != NULL)
                {
                    tmp.push_back(row[i]);
                }else{
                    tmp.push_back("-1");
                }
            }
            sql_result.push_back(tmp);
        }
        return sql_result;
    }
    else
    {
        std::vector< std::vector< std::string > > dummy;
        std::vector< std::string > tmp;
        tmp.push_back("NULL");
        dummy.push_back(tmp);
        std::cout << query_state << std::endl;
        return dummy;
    }
}


/**************
* Purpose: update the database no result returned
* Precondition: the query string
* Postcondition: false if failed, true if suceess
**************/
bool mysqldatabase::set(const char *query)
{
    if (!mysql_query(sock, query))
        return false; // failed query
    else
        return true; // successful query
}
