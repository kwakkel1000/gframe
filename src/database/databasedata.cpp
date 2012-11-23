//
//
// @ Project : gframe_database
// @ File Name : databasedata.cpp
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

//#include "../include/database/databasedata.h"
#include <gframe/database/databasedata.h>

#include <boost/algorithm/string.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

databasedata::databasedata()
{
    wait_time = 300;
}

databasedata::~databasedata()
{
    std::cout << "databasedata::~databasedata()" << std::endl;
    m_Run = false;
    m_SqlAvailable.notify_all();
    m_CounterAvailableCondition.notify_all();
    std::cout << "databasedata::~databasedata() m_QueryThread->join();" << std::endl;
    m_QueryThread->join();
    std::cout << "databasedata::~databasedata() m_CounterThread->join();" << std::endl;
    m_CounterThread->join();
    std::cout << "databasedata::~databasedata() delete m_db;" << std::endl;
    delete m_db;
}

void databasedata::init(database* db, std::string hostname, std::string databasename, std::string username, std::string password)
{
    m_db = db;
    m_HostName = hostname;
    m_DatabaseName = databasename;
    m_UserName = username;
    m_Pass = password;
    m_Run = true;
    assert(!m_CounterThread);
    m_CounterThread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&databasedata::db_timer, this)));
    assert(!m_QueryThread);
    m_QueryThread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&databasedata::query_run, this)));
}
void databasedata::init(database* db, std::string filename)
{
    m_db = db;
    m_FileName = filename;
    m_Run = true;
    assert(!m_CounterThread);
    m_CounterThread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&databasedata::db_timer, this)));
    assert(!m_QueryThread);
    m_QueryThread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&databasedata::query_run, this)));
}


/**
* get
* Create sql string from input vars and execute query
* @param msWhere Which datasource to get the data from (database in this class)
* @param msKey Which key to get the data from (colom in this class)
* @param msCondition Which conditions has the data to fullfill to get in the return (Where in this class)
* @return result of data in a vector
*
*/
std::vector< std::string > databasedata::get(std::string msWhere, std::string msKey, std::string msCondition)
{
    std::string _sSqlString;
    _sSqlString = _sSqlString + "SELECT ";
    _sSqlString = _sSqlString + " `";
    _sSqlString = _sSqlString + msKey;
    _sSqlString = _sSqlString + "` FROM `";
    _sSqlString = _sSqlString + msWhere;
    _sSqlString = _sSqlString + "` WHERE ";
    _sSqlString = _sSqlString + msCondition;
    std::cout << _sSqlString << std::endl;
    //Output::Instance().addOutput(_sSqlString, 5);
    std::vector< std::vector< std::string > > _vSqlResult;
    std::vector< std::string > _vDataResult;
    _vSqlResult= raw_sql(_sSqlString);
    for (unsigned int _uiSqlResultIndex = 0; _uiSqlResultIndex < _vSqlResult.size(); _uiSqlResultIndex++)
    {
        if (_vSqlResult[_uiSqlResultIndex].size() == 1)
        {
            _vDataResult.push_back(_vSqlResult[_uiSqlResultIndex][0]);
        }
    }
    return _vDataResult;
}


bool databasedata::del(std::string msWhere, std::string msCondition)
{
    std::string _sSqlString;
    _sSqlString = _sSqlString + "DELETE FROM `";
    _sSqlString = _sSqlString + msWhere;
    _sSqlString = _sSqlString + "` WHERE ";
    _sSqlString = _sSqlString + msCondition;
    add_sql_queue(_sSqlString);
    return true;
    return false;
}

bool databasedata::insert(std::string msWhere, std::string msKey, std::string msValue)
{
    std::string _sSqlString;
    _sSqlString = _sSqlString + "INSERT into ";
    _sSqlString = _sSqlString + msWhere;
    _sSqlString = _sSqlString + " (`";
    _sSqlString = _sSqlString + msKey;
    _sSqlString = _sSqlString + "`) VALUES ('";
    _sSqlString = _sSqlString + msValue;
    _sSqlString = _sSqlString + "')";
    add_sql_queue(_sSqlString);
    return true;
    return false;
}

bool databasedata::insert(std::string msWhere, std::vector< std::string > mvKeys, std::vector< std::string > mvValues)
{
    std::string _sSqlString;
    _sSqlString = _sSqlString + "INSERT into ";
    _sSqlString = _sSqlString + msWhere;
    _sSqlString = _sSqlString + " (`";
    for (unsigned int uiKeysIndex = 0; uiKeysIndex < mvKeys.size() -1; uiKeysIndex++)
    {
        _sSqlString = _sSqlString + mvKeys[uiKeysIndex];
        _sSqlString = _sSqlString + "`, `";
    }
    if (mvKeys.size() >= 1)
    {
        _sSqlString = _sSqlString + mvKeys[mvKeys.size() -1];
    }
    _sSqlString = _sSqlString + "`) VALUES (";
    for (unsigned int uiValuesIndex = 0; uiValuesIndex < mvValues.size() -1; uiValuesIndex++)
    {
        _sSqlString = _sSqlString + "'" + mvValues[uiValuesIndex];
        _sSqlString = _sSqlString + "', ";
    }
    if (mvValues.size() >= 1)
    {
        _sSqlString = _sSqlString + "'" + mvValues[mvValues.size() -1] + "')";
    }
    //Output::Instance().addOutput(_sSqlString, 5);
    add_sql_queue(_sSqlString);
    return true;
    return false;
}

bool databasedata::update(std::string msWhere, std::string msKey, std::string msValue, std::string msCondition)
{
    std::string _sSqlString;
    _sSqlString = _sSqlString + "UPDATE ";
    _sSqlString = _sSqlString + msWhere;
    _sSqlString = _sSqlString + " SET `";
    _sSqlString = _sSqlString + msKey;
    _sSqlString = _sSqlString + "`='";
    _sSqlString = _sSqlString + msValue;
    _sSqlString = _sSqlString + "' WHERE ";
    _sSqlString = _sSqlString + msCondition;
    //Output::Instance().addOutput(_sSqlString, 5);
    add_sql_queue(_sSqlString);
    return true;
    return false;
}

bool databasedata::update(std::string msWhere, std::vector< std::string > mvKeys, std::vector< std::string > mvValues, std::string msCondition)
{
    std::string _sSqlString;
    _sSqlString = _sSqlString + "UPDATE ";
    _sSqlString = _sSqlString + msWhere;
    _sSqlString = _sSqlString + " SET `";
    for (unsigned int uiKeysIndex = 0; uiKeysIndex < mvKeys.size() -1; uiKeysIndex++)
    {
        _sSqlString = _sSqlString + mvKeys[uiKeysIndex];
        _sSqlString = _sSqlString + "`='";
        _sSqlString = _sSqlString + mvValues[uiKeysIndex];
        _sSqlString = _sSqlString + "', `";
    }
    if (mvKeys.size() >= 1 && mvValues.size() >= 1)
    {
        _sSqlString = _sSqlString + mvKeys[mvKeys.size() -1];
        _sSqlString = _sSqlString + "`='";
        _sSqlString = _sSqlString + mvValues[mvKeys.size() -1];
        _sSqlString = _sSqlString + "'";
    }
    _sSqlString = _sSqlString + " WHERE ";
    _sSqlString = _sSqlString + msCondition;
    //Output::Instance().addOutput(_sSqlString, 5);
    add_sql_queue(_sSqlString);
    return true;
    return false;
}

void databasedata::add_sql_queue(std::string query)
{
    //std::cout << query << std::endl;
    boost::mutex::scoped_lock lock(m_SqlMutex);
    sql_queue.push(query);
    std::cout << "databasedata::add_sql_queue m_SqlAvailable.notify_one();" << std::endl;
    m_SqlAvailable.notify_one();
}

void databasedata::query_run()
{
    while (!m_Run)
    {
        usleep(100000);
    }
    std::cout << "QueryRun started" << std::endl;
    while (m_Run)
    {
        boost::mutex::scoped_lock lock(m_SqlMutex);
        while (sql_queue.empty() && m_Run)
        {
            std::cout << "databasedata::query_run m_SqlAvailable.wait(lock);" << std::endl;
            m_SqlAvailable.wait(lock);
        }
        while (!m_db->connected())
        {
            std::cout << "open connection" << std::endl;
            state = m_db->connect(m_HostName.c_str(), m_DatabaseName.c_str(), m_UserName.c_str(), m_Pass.c_str());
        }
        if (state == 200 && m_db->connected())
        {
            while (!sql_queue.empty() && m_Run)
            {
                std::string temp = sql_queue.front();
                sql_queue.pop();
                if (state == 200 && m_db->connected())
                {
                    std::cout << temp << std::endl;
                    m_db->set(temp.c_str());
                    boost::mutex::scoped_lock lock(m_CounterMutex);
                    m_last_query_time = time (NULL);
                }
                else
                {
                    break;
                }
            }
            if (sql_queue.empty() && m_Run)
            {
                {
                    /*boost::mutex::scoped_lock lock(m_CounterMutex);
                    counter = 0;*/
                    m_CounterAvailableCondition.notify_one();/**/
                }
            }
        }
    }
    /*if (m_db)
    {
        m_db->disconnect();
    }*/
}

void databasedata::db_timer()
{
    while (m_Run)
    {
        {
            if (time (NULL) - m_last_query_time >= wait_time)
            {
                boost::mutex::scoped_lock lock(m_CounterMutex);
                std::cout << "connection closed" << std::endl;
                m_db->disconnect();
                m_CounterAvailableCondition.wait(lock);
            }
        }
        usleep(1000000);
        /*while (counter < wait_time && m_Run)
        {
            usleep(1000000);
            {
                //boost::mutex::scoped_lock lock(m_CounterMutex);
                counter++;
            }
        }
        // mutex?
        boost::mutex::scoped_lock lock(m_CounterMutex);
        if (counter == wait_time && m_db->connected())
        {
            std::cout << "connection closed" << std::endl;
            m_db->disconnect();
            counter++;
        }*/
    }
    /*if (m_db->connected())
    {
        std::cout << "connection closed" << std::endl;
        m_db->disconnect();
    }*/
}

std::vector< std::vector< std::string > > databasedata::raw_sql(std::string query)
{
    boost::mutex::scoped_lock lock(m_SqlMutex);
    std::vector< std::vector< std::string > > sql_result;
    while (!m_db->connected())
    {
        //std::cout << "open connection" << std::endl;
        state = m_db->connect(m_HostName.c_str(), m_DatabaseName.c_str(), m_UserName.c_str(), m_Pass.c_str());
    }
    if (state == 200 && m_db->connected())
    {
        sql_result = m_db->get(query.c_str());
        boost::mutex::scoped_lock lock(m_CounterMutex);
        m_last_query_time = time (NULL);
        if (sql_queue.empty() && m_Run)
        {
            {
                /*boost::mutex::scoped_lock lock(m_CounterMutex);
                counter = 0;*/
                m_CounterAvailableCondition.notify_one();/**/
            }
        }
    }
    else
    {
        //Output::Instance().addStatus(false, "DB Fail [" + convertInt(state) + "] " + mHostName + " " + mDatabaseName + " " + mUserName);
    }
    return sql_result;
}
