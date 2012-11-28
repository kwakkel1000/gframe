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

#include <gframe/database/databasedata.h>
#include <gframe/output.h>
#include <chrono>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

databasedata::databasedata() : wait_time(300), m_last_query_time(time (NULL))
{
}

databasedata::~databasedata()
{
    output::instance().addOutput("databasedata::~databasedata", 10);
    a_Run = false;
    m_SqlAvailable.notify_all();
    m_CounterAvailableCondition.notify_all();
    output::instance().addOutput("databasedata::~databasedata  m_QueryThread->join();", 10);
    m_QueryThread->join();
    output::instance().addOutput("databasedata::~databasedata  m_CounterThread->join();", 10);
    m_CounterThread->join();
    delete m_db;
    output::instance().addOutput("databasedata::~databasedata  done", 10);
}

void databasedata::init(database* db, std::string hostname, std::string databasename, std::string username, std::string password)
{
    std::unique_lock<std::mutex> settingslock(m_SettingsMutex);
    m_db = db;
    m_HostName = hostname;
    m_DatabaseName = databasename;
    m_UserName = username;
    m_Pass = password;
    a_Run = true;
    m_CounterThread = std::shared_ptr<std::thread>(new std::thread(std::bind(&databasedata::db_timer, this)));
    m_QueryThread = std::shared_ptr<std::thread>(new std::thread(std::bind(&databasedata::query_run, this)));
    settingslock.unlock();
    m_SettingsAvailableCondition.notify_all();
}
void databasedata::init(database* db, std::string filename)
{
    std::unique_lock<std::mutex> settingslock(m_SettingsMutex);
    m_db = db;
    m_FileName = filename;
    a_Run = true;
    m_CounterThread = std::shared_ptr<std::thread>(new std::thread(std::bind(&databasedata::db_timer, this)));
    m_QueryThread = std::shared_ptr<std::thread>(new std::thread(std::bind(&databasedata::query_run, this)));
    settingslock.unlock();
    m_SettingsAvailableCondition.notify_all();
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
    add_sql_queue(_sSqlString);
    return true;
    return false;
}

std::vector< std::vector< std::string > > databasedata::raw_sql(std::string query)
{
    std::lock_guard<std::mutex> lock1(m_SqlMutex);
    std::vector< std::vector< std::string > > sql_result;
    while (!m_db->connected())
    {
        output::instance().addOutput("databasedata::raw_sql open connection", 10);
        std::vector<const char *> Arguments;
        Arguments.clear();
        Arguments.push_back(m_HostName.c_str());
        Arguments.push_back(m_DatabaseName.c_str());
        Arguments.push_back(m_UserName.c_str());
        Arguments.push_back(m_Pass.c_str());
        state = m_db->connect(Arguments);
        if (state == 0)
        {
            output::instance().addOutput("wrong amount of arguments, 4 needed (hostname, databasename, username, password)", 2);
            exit (0);
        }
    }
    if (state == 200 && m_db->connected())
    {
        sql_result = m_db->get(query.c_str());
        std::lock_guard<std::mutex> lock2(m_CounterMutex);
        m_last_query_time = time (NULL);
        if (sql_queue.empty() && a_Run)
        {
            {
                m_CounterAvailableCondition.notify_one();
            }
        }
    }
    else
    {
        //Output::Instance().addStatus(false, "DB Fail [" + convertInt(state) + "] " + mHostName + " " + mDatabaseName + " " + mUserName);
    }
    return sql_result;
}

void databasedata::add_sql_queue(std::string query)
{
    std::lock_guard<std::mutex> lock(m_SqlMutex);
    sql_queue.push(query);
    m_SqlAvailable.notify_one();
}

void databasedata::query_run()
{
    std::unique_lock<std::mutex> settingslock(m_SettingsMutex);
    m_SettingsAvailableCondition.wait(settingslock);
    settingslock.unlock();
    output::instance().addOutput("databasedata::query_run QueryRun started", 7);
    while (a_Run)
    {
        std::unique_lock<std::mutex> lock1(m_SqlMutex);
        while (sql_queue.empty() && a_Run)
        {
            m_SqlAvailable.wait(lock1);
        }
        while (!m_db->connected())
        {
            output::instance().addOutput("databasedata::query_run open connection", 10);
            std::vector<const char *> Arguments;
            Arguments.clear();
            Arguments.push_back(m_HostName.c_str());
            Arguments.push_back(m_DatabaseName.c_str());
            Arguments.push_back(m_UserName.c_str());
            Arguments.push_back(m_Pass.c_str());
            state = m_db->connect(Arguments);
            if (state == 0)
            {
                output::instance().addOutput("wrong amount of arguments, 4 needed (hostname, databasename, username, password)", 2);
                exit (0);
            }
        }
        if (state == 200 && m_db->connected())
        {
            while (!sql_queue.empty() && a_Run)
            {
                std::string temp = sql_queue.front();
                sql_queue.pop();
                if (state == 200 && m_db->connected())
                {
                    std::cout << temp << std::endl;
                    m_db->set(temp.c_str());
                    std::lock_guard<std::mutex> lock2(m_CounterMutex);
                    m_last_query_time = time (NULL);
                }
                else
                {
                    break;
                }
            }
            if (sql_queue.empty() && a_Run)
            {
                {
                    m_CounterAvailableCondition.notify_one();
                }
            }
        }
        lock1.unlock();
    }
}

void databasedata::db_timer()
{
    while (a_Run)
    {
        std::unique_lock<std::mutex> db_timer_lock(m_CounterMutex);
        if (time (NULL) - m_last_query_time >= wait_time)
        {
            output::instance().addOutput("databasedata::db_timer connection closed", 10);
            m_db->disconnect();
            m_CounterAvailableCondition.wait(db_timer_lock);
        }
        db_timer_lock.unlock();
        std::this_thread::sleep_for( std::chrono::seconds(1) );
    }
}
