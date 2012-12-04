//
//
// @ Project : gframe
// @ File Name : output.h
// @ Date : 31-10-2012
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


#ifndef SRC_INCLUDE_OUTPUT_H
#define SRC_INCLUDE_OUTPUT_H

#include <mutex>
#include <thread>

#include <string>

#include <fstream>

class output
{
    public:
        static output& instance()
        {
            static output obj;
            return obj;
        }

        void setDebugLevel(int level);
        void setLogFile(std::string msLogFile);

        void openLog();
        void closeLog();

        void addStatus(bool bSuccess, std::string sOutput);
        void addOutput(std::string sOutput);
        void addOutput(std::string sOutput, int iLevel);
        void appendLog(std::string sOutput);
        void appendLog(std::string sOutput, int iLevel);

        std::string sFormatTime(std::string format);

    private:
        output();
        ~output();

        int iLogLevel;
        int iOutputLevel;

        std::string sLogFile;
        std::ofstream fLogFile;

        std::mutex m_outputMutex;
        std::mutex m_logMutex;
};

#endif // SRC_INCLUDE_OUTPUT_H_
