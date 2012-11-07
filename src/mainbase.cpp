//
//
// @ Project : grame
// @ File Name : mainbase.cpp
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

#include <gframe/mainbase.h>



#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <iostream>
#include <vector>

std::string VERSION = __GIT_VERSION;
std::string NAME = "undefined";

mainbase::mainbase()
{
}

void mainbase::init()
{
    m_INeedRoot = false;
    m_Foreground = false;
    m_Name = NAME;
    m_Version = VERSION;
    m_PidFileLocation = "/var/run/" + m_Name + "/";
    m_LogFileLocation = "log/";
    m_IniFile = "conf/" + m_Name + ".ini";
    m_PidFile = m_PidFileLocation + m_Name + ".pid";
    m_LogFile = m_LogFileLocation + m_Name + ".log";

    addHelpItem("Runs the " + m_Name + " (default as " + m_Name + ", " + m_PidFile + ", " + m_LogFile + " " + m_IniFile + ")");
    addHelpItem("USAGE " + m_Name + " [OPTIONS]");
    addHelpItem("Available options:");
    addHelpItem("\t-h, --help List options");
    addHelpItem("\t-v, --version Output version and exit");
    addHelpItem("\t-f, --foreground Don't fork into the background");
    addHelpItem("\t-c, --config Set config file (default: " + m_IniFile + ")");
    addHelpItem("\t-d, --debug Set debug level [1-10] (default: 5)");
    addHelpItem("\t-p, --pid Set Pid file location (default: " + m_PidFileLocation + ")");
    addHelpItem("\t-l, --log Set log file location (default: " + m_LogFileLocation + ")");
    addHelpItem("\t-n, --name Set name for pid/log files (default: " + m_Name + ")");
    addHelpItem("\t--INeedRootPowerz Requered when running as root");
}

void mainbase::parseArgs(std::vector<std::string> args)
{
    for (uint nArg = 0; nArg < args.size(); nArg++)
    {
        if (args[nArg] == "--help" || args[nArg] == "-h")
        {
            showHelp();
            exit(EXIT_SUCCESS);
        }
        if (args[nArg] == "--version" || args[nArg] == "-v")
        {
            std::cout << m_Name << " " << m_Version << std::endl;
            exit(EXIT_SUCCESS);
        }
        if (args[nArg] == "--foreground" || args[nArg] == "-f")
        {
            m_Foreground = true;
        }
        if (args[nArg] == "--config" || args[nArg] == "-c")
        {
            if ((nArg+1) < args.size())
            {
                m_IniFile = args[nArg+1];
            }
        }
        if (args[nArg] == "--debug" || args[nArg] == "-d")
        {
            if ((nArg+1) < args.size())
            {
                int i;
                std::stringstream ss(args[nArg+1]);
                ss >> i;
                output::instance().setDebugLevel(i);
            }
        }
        if (args[nArg] == "--pid" || args[nArg] == "-p")
        {
            if ((nArg+1) < args.size())
            {
                m_PidFileLocation = args[nArg+1];
            }
        }
        if (args[nArg] == "--log" || args[nArg] == "-l")
        {
            if ((nArg+1) < args.size())
            {
                m_LogFileLocation = args[nArg+1];
            }
        }
        if (args[nArg] == "--name" || args[nArg] == "-n")
        {
            if ((nArg+1) < args.size())
            {
                m_Name = args[nArg+1];
            }
        }
        if (args[nArg] == "--INeedRootPowerz")
        {
            m_INeedRoot = true;
        }
    }
}


int mainbase::run()
{
    if (readPidFile())
    {
        return 1;
    }
    output::instance().setLogFile(m_LogFile);
    output::instance().init();
    if (isRoot())
    {
        fprintf(stdout, "Your are running %s as root!\n", m_Name.c_str());
        fprintf(stdout, "this is dangerouse and can cause great damage!\n");
        if (!m_INeedRoot) {
            return 1;
        }
        fprintf(stdout, "You have been warned.\n");
        fprintf(stdout, "Hit CTRL+C now if you don't want to run %s as root.\n", m_Name.c_str());
        fprintf(stdout, "%s will start in 15 seconds.\n", m_Name.c_str());
        sleep(15);
    }

    int DaemonizeStatus = daemonize(!m_Foreground);
    if (DaemonizeStatus != -1)
    {
        return DaemonizeStatus;
    }
    if(!configreader::instance().readFile(m_IniFile))
    {
        return 1;
    }
    std::string startBlock = "+++++++++++++++++++++++++++++++++";
    for (unsigned int m_Name_Iterator = 0; m_Name_Iterator < m_Name.size(); m_Name_Iterator++)
    {
        startBlock = startBlock + "+";
    }
    output::instance().addOutput(startBlock, 2);
    output::instance().addOutput("+ Start " + m_Name + " on " + output::instance().sFormatTime("%d-%m-%Y %H:%M:%S") + " +", 2);
    output::instance().addOutput(startBlock, 2);
    usleep(2000000);
    return -1;
}

bool mainbase::readPidFile()
{
    std::string sPid = "-2";
    int iFilePid = -2;

    // check if process in pidfile is still running
    std::ifstream ifFilePid (m_PidFile.c_str());
    if (ifFilePid.is_open())
    {
        while ( ifFilePid.good() )
        {
            getline (ifFilePid,sPid);
            //std::cout << sPid << std::endl;
        }
        ifFilePid.close();
    }
    std::stringstream ss(sPid);
    ss >> iFilePid;
    if (kill(iFilePid, 0) != -1)
    {
        printf("still running \r\n");
        return true;
        exit(EXIT_FAILURE);
    }
    return false;
    // end check
}

void mainbase::writePidFile(int Pid)
{
    // write current pid to pidfile
    if (Pid < 0) {
        printf("pid < 0 FAIL \r\n");
        exit(EXIT_FAILURE);
    }

    std::ofstream ofPidFile (m_PidFile.c_str());
    printf("%s\n", m_PidFile.c_str());
    if (ofPidFile.is_open())
    {
        ofPidFile << Pid;
        ofPidFile.close();
    }
    else
    {
        printf("cant open pid file \r\n");
        exit(EXIT_FAILURE);
    }
    // end writing pid file
}

bool mainbase::isRoot()
{
    // User root? If one of these were root, we could switch the others to root, too
    return (geteuid() == 0 || getuid() == 0);
}

void mainbase::showHelp()
{
    for (unsigned int HelpItems_iterator = 0; HelpItems_iterator < m_HelpItems.size(); HelpItems_iterator++)
    {
        std::cout << m_HelpItems[HelpItems_iterator] << std::endl;
    }
}

void mainbase::addHelpItem(std::string HelpItem)
{
    m_HelpItems.push_back(HelpItem);
}

int mainbase::daemonize(bool Daemonize)
{
    if (!Daemonize)
    {
        int Pid = getpid();
        writePidFile(Pid);
        fprintf(stdout, "Staying open for debugging\n");
        fprintf(stdout, "pid [%d]\n", Pid);
    }
    else
    {
        fprintf(stdout, "Forking into the background\n");

        int Pid = fork();

        if (Pid == -1) {
            fprintf(stderr, "%s\n", strerror(errno));
            return 1;
        }

        if (Pid > 0) {
            // We are the parent. We are done and will go to bed.
            writePidFile(Pid);
            fprintf(stdout, "pid [%d]\n", Pid);
            return 0;
        }
        // Redirect std in/out/err to /dev/null
        close(0); open("/dev/null", O_RDONLY);
        close(1); open("/dev/null", O_WRONLY);
        close(2); open("/dev/null", O_WRONLY);

        // We are the child. There is no way we can be a process group
        // leader, thus setsid() must succeed.
        setsid();
        // Now we are in our own process group and session (no
        // controlling terminal). We are independent!
        fprintf(stdout, "child running\n");
    }
    return -1;
}
