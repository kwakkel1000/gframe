//
//
// @ Project : gframe_database
// @ File Name : mysqldatabase.h
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


#ifndef SRC_INCLUDE_MYSQLDATABASE_H
#define SRC_INCLUDE_MYSQLDATABASE_H

//#include "database.h"
#include <gframe/database/database.h>

#include <mysql/mysql.h>
#include <string>
#include <vector>

// Meh! I changed all the char* to const char* but it would be easier if they where std::string's
class mysqldatabase : public database
{
    public:
        // class constructor
        mysqldatabase();
        ~mysqldatabase(){};

        // connection & disconnect
        //int connect(const char *host, const char *db, const char *user, const char *pass); // connect to the database
        int connect(std::vector<const char*>); // connect to the database
        bool disconnect(); // disconnect from the database
        bool connected();

        // querying
        //MYSQL_RES *query(const char *query); // return result set
        bool set(const char *query); // updates, returns true if update went through
        std::vector< std::vector< std::string> > get(const char* query);
        // free the query results
        //bool free();

    private:
        MYSQL *sock;
        MYSQL_RES *result;
        MYSQL_ROW row;
        MYSQL_FIELD *field;
        char *qbuf[MAX_QUERY_LEN];
};

const char *dberror(int errorcode); // return a string for this error message

#endif // SRC_INCLUDE_MYSQLDATABASE_H
