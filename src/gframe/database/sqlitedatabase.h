//
//
// @ Project : gframe_database
// @ File Name : sqlitedatabase.h
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


#ifndef SRC_INCLUDE_SQLITEDATABASE_H
#define SRC_INCLUDE_SQLITEDATABASE_H

//#include "database.h"
#include <gframe/database/database.h>

#include <sqlite3.h>
#include <string>
#include <vector>

class sqlitedatabase : public database
{
    public:
        // class constructor
        sqlitedatabase();

        // connection & disconnect
        int connect(const char *filename); // connect to the database
        bool disconnect(); // disconnect from the database
        bool connected();

        // querying
        bool set(const char *query); // updates, returns true if update went through
        std::vector< std::vector< std::string> > get(const char* query);

    private:
        sqlite3 *db;
};

#endif // SRC_INCLUDE_SQLITEDATABASE_H
