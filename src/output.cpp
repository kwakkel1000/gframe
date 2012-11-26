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
#include <gframe/lib.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>

#include <fstream>

void output::setDebugLevel(int iLevel)
{
    iOutputLevel = iLevel;
}
void output::setLogFile(std::string msLogFile)
{
    sLogFile = msLogFile;
}

void output::addStatus(bool bSuccess, std::string sOutput)
{
    if (bSuccess)
    {
        if (2 <= iOutputLevel)
        {
            std::cout << "\033[1m\033[34m[\033[32m ok \033[34m]\033[39m\033[22m" << sOutput << std::endl;
            appendLog("[ ok ] " + sOutput, 2);
        }
    }
    else
    {
        std::cout << "\033[1m\033[34m[\033[31m !! \033[34m]\033[39m\033[22m" << sOutput << std::endl;
        appendLog("[ !! ] " + sOutput, 1);
    }
}

void output::addOutput(std::string sOutput)
{
    addOutput(sOutput, 5);
}

void output::addOutput(std::string sOutput, int iLevel)
{
    if (iLevel <= iOutputLevel)
    {
        std::cout << "\033[1m\033[34m[\033[33m ** \033[34m] [\033[32m" << lib::stringFromInt(iLevel) << "\033[34m] \033[0m" << sOutput << std::endl;
    }
    appendLog("[ ** ] " + sOutput, iLevel);
}

void output::appendLog(std::string sOutput)
{
    appendLog(sOutput, 5);
}

void output::appendLog(std::string sOutput, int iLevel)
{
    if (sLogFile != "")
    {
        if (iLevel <= iLogLevel)
        {
            fLogFile << "[" << sFormatTime("%d-%m-%Y %H:%M:%S") << "] [" << lib::stringFromInt(iLevel) << "] " << sOutput << std::endl;
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

void output::init()
{
    fLogFile.open(sLogFile.c_str(), std::ios::app);
    fLogFile << "\r\n\r\n\r\n\r\nopen logfile\r\n";
}

output::output()
{
    iLogLevel = 10;
    iOutputLevel = 5;
    sLogFile = "";
}

output::~output()
{
    fLogFile << "closing logfile \r\n\r\n\r\n\r\n";
    fLogFile.close();
}

