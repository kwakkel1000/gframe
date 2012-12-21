//
//
// @ Project : gframe
// @ File Name : mainbase.h
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

#ifndef SRC_INCLUDE_MAINBASE_H
#define SRC_INCLUDE_MAINBASE_H

#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#include <string>
#include <vector>

#include <gframe/glib.h>
#include <gframe/output.h>
#include <gframe/configreader.h>

void gdb_sighandler(int i_num, siginfo_t * i_info, void * i_val);
void SegFaultAction(int i_num, siginfo_t * i_info, void * i_val);
void TermAction(int i_num, siginfo_t * i_info, void * i_val);
void Usr1Action(int i_num, siginfo_t * i_info, void * i_val);

class mainbase
{
    public:
        mainbase(std::string name);
        ~mainbase();

        void parseArgs(std::vector<std::string> args);
        int run();

        void showHelp();
        void addHelpItem(std::string HelpItem);

    private:

        void SetupSignal();
        bool isRoot();
        bool dropRoot();

        bool createDirectory(std::string directory);

        bool readPidFile();
        void writePidFile(int Pid);

        int daemonize(bool Daemonize);

        bool m_Forever;
        bool m_INeedRoot;
        bool m_DropRoot;
        bool m_Foreground;
        std::string m_Uid;
        std::string m_Gid;
        std::string m_Name;
        std::string m_Syslog;
        std::string m_LogFileLocation;
        std::string m_PidFileLocation;
        std::string m_IniFile;
        std::string m_PidFile;
        std::string m_LogFile;
        std::vector< std::string > m_HelpItems;
};

#endif // SRC_INCLUDE_MAINBASE_H
