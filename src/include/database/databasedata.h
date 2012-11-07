//
//
// @ Project : gframe_database
// @ File Name : databasedata.h
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


#ifndef SRC_INCLUDE_DATABASE_DATABASEDATA_H
#define SRC_INCLUDE_DATABASE_DATABASEDATA_H

#include <boost/bind.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>

//#include "database.h"
#include <gframe/database/database.h>

#include <string>
#include <vector>
#include <queue>
#include <map>
#include <time.h>

class databasedata
{
    public:
        static databasedata& instance()
        {
            static databasedata obj;
            return obj;
        }

        void init(database* db, std::string hostname, std::string databasename, std::string username, std::string password);
        void init(database* db, std::string filename);

        bool insert(std::string msWhere, std::string msKey, std::string msValue);
        bool insert(std::string msWhere, std::vector< std::string > mvKeys, std::vector< std::string > mvValues);

        bool update(std::string msWhere, std::string msKey, std::string msValue, std::string msCondition);
        bool update(std::string msWhere, std::vector< std::string > mvKeys, std::vector< std::string > mvValues, std::string msCondition);

        bool del(std::string msWhere, std::string msCondition);

        std::vector< std::string > get(std::string msWhere, std::string msKey, std::string msCondition);
        //std::vector< std::map< std::string, std::string > get(std::string msWhere, std::vector< std::string > mvKeys, std::string msCondition);

    private:
        databasedata();
        ~databasedata();

        database* m_db;

        // vars
        bool m_Run;
        int state;
        std::queue< std::string > sql_queue;
        unsigned int wait_time;
        time_t m_last_query_time;

        // config vars
        std::string m_FileName;
        std::string m_HostName;
        std::string m_DatabaseName;
        std::string m_UserName;
        std::string m_Pass;

        // thread vars
        boost::shared_ptr< boost::thread > m_QueryThread;
        boost::shared_ptr< boost::thread > m_CounterThread;
        boost::condition m_SqlAvailable;
        boost::condition m_CounterAvailableCondition;
        boost::mutex m_SqlMutex;
        boost::mutex m_CounterMutex;

        // functions
        void add_sql_queue(std::string query);
        std::vector< std::vector< std::string > > raw_sql(std::string query);
        void query_run();
        void db_timer();
};

#endif // SRC_INCLUDE_DATABASE_DATABASEDATA_H
