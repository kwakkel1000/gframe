//
//
// @ Project : gframe
// @ File Name : output.cpp
// @ Date : 29-10-2012
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

#include <gframe/output.h>
#include <gframe/glib.h>
#include <gframe/config.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>

#include <fstream>
#ifdef USE_SYSLOG
    #include <syslog.h>
#endif

void output::setDebugLevel(int iLevel)
{
    std::mutex setdebugmutex;
    std::lock_guard<std::mutex> setdebuglock(setdebugmutex);
    iOutputLevel = iLevel;
}

void output::setSyslog(std::string msSyslog)
{
    std::mutex setsyslogmutex;
    std::lock_guard<std::mutex> setsysloglock(setsyslogmutex);
    sSyslog = msSyslog;
}

void output::setLogLevel(int iLevel)
{
    std::mutex setlogmutex;
    std::lock_guard<std::mutex> setloglock(setlogmutex);
    iLogLevel = iLevel;
}

void output::setLogFile(std::string msLogFile)
{
    std::mutex setlogmutex;
    std::lock_guard<std::mutex> setloglock(setlogmutex);
    sLogFile = msLogFile;
}

void output::addStatus(bool bSuccess, std::string sOutput)
{
    if (bSuccess)
    {
        if (2 <= iOutputLevel)
        {
            std::cout << "\033[1m\033[34m[\033[32m ok \033[34m]\033[39m\033[22m " << sOutput << std::endl;
#ifdef USE_SYSLOG
            appendSyslog("[ ok ] " + sOutput, 5);
#endif
            appendLog("[ ok ] " + sOutput, 2);
        }
    }
    else
    {
        std::cout << "\033[1m\033[34m[\033[31m !! \033[34m]\033[39m\033[22m " << sOutput << std::endl;
#ifdef USE_SYSLOG
        appendSyslog("[ !! ] " + sOutput, 3);
#endif
        appendLog("[ !! ] " + sOutput, 1);
    }
}

void output::addOutput(std::string sOutput)
{
    addOutput(sOutput, 5);
}

void output::addOutput(std::string sOutput, int iLevel)
{
    std::lock_guard<std::mutex> outputlock(m_outputMutex);
    if (iLevel <= iOutputLevel)
    {
        std::cout << "\033[1m\033[34m[\033[33m ** \033[34m] [\033[32m" << glib::stringFromInt(iLevel) << "\033[34m] \033[0m" << sOutput << std::endl;
    }
    appendLog("[ ** ] " + sOutput, iLevel);
    appendSyslog("[ ** ] " + sOutput, iLevel);
}


void output::appendSyslog(std::string sOutput)
{
    appendSyslog(sOutput, 6);
}

void output::appendSyslog(std::string sOutput, int iLevel)
{
    std::lock_guard<std::mutex> sysloglock(m_syslogMutex);
#ifdef USE_SYSLOG
    if (sSyslog != "")
    {
        unsigned int facility;
        switch (iLevel)
        {
            case 0:
                facility = LOG_EMERG;
                break;
            case 1:
                facility = LOG_ALERT;
                break;
            case 2:
                facility = LOG_CRIT;
                break;
            case 3:
                facility = LOG_ERR;
                break;
            case 4:
                facility = LOG_WARNING;
                break;
            case 5:
                facility = LOG_NOTICE;
                break;
            case 6:
                facility = LOG_INFO;
                break;
            default:
                facility = LOG_DEBUG;
        }
        if (iLevel <= iLogLevel)
        {
            syslog(facility, sOutput.c_str());
        }
    }
#endif
}

void output::appendLog(std::string sOutput)
{
    appendLog(sOutput, 5);
}

void output::appendLog(std::string sOutput, int iLevel)
{
    std::lock_guard<std::mutex> loglock(m_logMutex);
    if (sLogFile != "")
    {
        if (iLevel <= iLogLevel)
        {
            fLogFile << "[" << sFormatTime("%d-%m-%Y %H:%M:%S") << "] [" << glib::stringFromInt(iLevel) << "] " << sOutput << std::endl;
        }
    }
}

std::string output::sFormatTime(std::string sFormat)
{
    time_t rawtime;
    struct tm * timeinfo;
    char cBuffer [80];

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    strftime(cBuffer, 80, sFormat.c_str(), timeinfo);
    return cBuffer;
}

void output::openLog()
{
#ifdef USE_SYSLOG
    if (sSyslog != "")
    {
        openlog(sSyslog.c_str(), LOG_PID, LOG_USER);
    }
#endif
    if (sLogFile != "")
    {
        fLogFile.open(sLogFile.c_str(), std::ios::app);
        fLogFile << "\r\n\r\n\r\n\r\nopen logfile\r\n";
    }
}

void output::closeLog()
{
#ifdef USE_SYSLOG
    closelog();
#endif
    fLogFile << "closing logfile \r\n\r\n\r\n\r\n";
    fLogFile.close();
}

output::output() : iLogLevel(10), iOutputLevel(5), sSyslog(""), sLogFile("")
{
}

output::~output()
{
    closeLog();
}

