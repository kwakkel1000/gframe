//
//
// @ Project : gframe_database
// @ File Name : database.h
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


#ifndef SRC_INCLUDE_DATABASE_DATABASE_H
#define SRC_INCLUDE_DATABASE_DATABASE_H

#define SUCCESS 200
#define ERR201 201
#define ERR202 202
#define ERR203 203

#define MAX_QUERY_LEN 500

#include <vector>

class database
{
    public:
        virtual ~database()=0;
        virtual int connect(const char *host, const char *db, const char *user, const char *pass);
        virtual int connect(const char* filename);
        virtual bool disconnect()=0;
        virtual bool connected()=0;

        virtual std::vector< std::vector< std::string> > get(const char *query)=0;
        virtual bool set(const char *query)=0;

};

#endif // SRC_INCLUDE_DATABASE_DATABASE_H
