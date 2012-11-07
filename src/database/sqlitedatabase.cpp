//
//
// @ Project : gframe_database
// @ File Name : sqlitedatabase.cpp
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

//#include "../include/database/sqlitedatabase.h"
#include <gframe/database/sqlitedatabase.h>

/**************
* Purpose: default constructor, initialize class values
* Precondition: none
* Postcondition: none
**************/
sqlitedatabase::sqlitedatabase()
{
}

/**************
* Purpose: connect to the appropriate database
* Precondition: host, database, user name and password
* Postcondition: connected to that database
**************/
int sqlitedatabase::connect(const char *filename)
{
    if(sqlite3_open(filename, &db) == SQLITE_OK)
		return SUCCESS;

	return 0;
}

/**************
* Purpose: disconnect from the database
* Precondition: none
* Postcondition: socket closed
**************/
bool sqlitedatabase::disconnect()
{
    if(db)
        sqlite3_close(db);

    db = NULL;
    // database disconnected
    return true;
}

/************
*return true if connected
************/
bool sqlitedatabase::connected()
{
    return true;
}

std::vector< std::vector< std::string > > sqlitedatabase::get(const char *query)
{



	sqlite3_stmt *statement;
	vector<vector<string> > results;
	std::vector< std::vector< std::string > > sql_result;

	if(sqlite3_prepare_v2(db, query, -1, &statement, 0) == SQLITE_OK)
	{
		int cols = sqlite3_column_count(statement);
		int result = 0;
		while(true)
		{
			result = sqlite3_step(statement);

			if(result == SQLITE_ROW)
			{
				std::vector< std::string > values;
				for(int col = 0; col < cols; col++)
				{
					values.push_back((char*)sqlite3_column_text(statement, col));
				}
				sql_result.push_back(values);
			}
			else
			{
				break;
			}
		}

		sqlite3_finalize(statement);
	}

	std::string error = sqlite3_errmsg(db);
	if(error != "not an error") std::cout << query << " " << error << std::endl;

	return sql_result;

    /*

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
    }*/
}


/**************
* Purpose: update the database no result returned
* Precondition: the query string
* Postcondition: false if failed, true if suceess
**************/
bool sqlitedatabase::set(const char *query)
{
    /*if (!mysql_query(sock, query))
        return false; // failed query
    else*/
        return true; // successful query
}
