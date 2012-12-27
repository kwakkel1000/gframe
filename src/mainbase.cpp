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
#include <gframe/config.h>
#include <gframe/versions.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <vector>
#include <grp.h>
#include <pwd.h>

extern "C" void libgframe_is_present(void)
{
}

std::string gNAME = PACKAGE;
std::string gVERSION = VERSION;
std::string gGITVERSION = __GIT_VERSION;

void gdb_sighandler(int i_num, siginfo_t * i_info, void * i_val)
{
#ifdef GDBDEBUG
    char exe[1024];
    ssize_t len = ::readlink("/proc/self/exe", exe, sizeof(exe)-1);
    if (len == -1)
    {
         perror("readlink");
         exit(EXIT_FAILURE);
    }
    exe[len] = '\0';

    char pid[16];
    snprintf(pid, sizeof(pid), "%d", getpid());

    pid_t p = fork();
    if (p == 0) {
        execl("/usr/bin/gdb", "gdb", "-ex", "cont",
            exe, pid, NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    }
    else if (p < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Allow a little time for GDB to start before
        // dropping into the default signal handler
        struct timespec req, rem;
        req.tv_sec = 1;
        req.tv_nsec = 0;
        rem.tv_sec = 0;
        rem.tv_nsec = 0;
        nanosleep(&req, &rem);
        signal(i_num, SIG_DFL);
        throw * i_info;
    }
#endif
}

void SegFaultAction(int i_num, siginfo_t * i_info, void * i_val)
{
    output::instance().addStatus(false, "SegFault Action");
    output::instance().addOutput("SegFault Signal " + glib::stringFromInt(i_num) + " caught...", 4);
    output::instance().addOutput("si_signo:   " + glib::stringFromInt(i_info->si_signo) + "    = Signal number", 4);
    output::instance().addOutput("si_pid:     " + glib::stringFromInt(i_info->si_pid) + "    = Sending process ID", 4);
    output::instance().addOutput("si_errno:   " + glib::stringFromInt(i_info->si_errno) + "    = An errno value", 4);
    output::instance().addOutput("si_code:    " + glib::stringFromInt(i_info->si_code) + "    = Signal code", 4);
    output::instance().addOutput("si_uid:     " + glib::stringFromInt(i_info->si_uid) + "     = Real user ID of sending process", 4);
    output::instance().addOutput("si_status:  " + glib::stringFromInt(i_info->si_status) + "    = Exit value or signal", 4);

#if defined(linux) || defined(__linux) || defined(__linux__)
    std::ostringstream ost_ptr;
    ost_ptr << i_info->si_ptr;
    std::string ssi_ptr = ost_ptr.str();
    output::instance().addOutput("si_utime:   " + glib::stringFromInt(i_info->si_utime) + "    = User time consumed", 4);
    output::instance().addOutput("si_stime:   " + glib::stringFromInt(i_info->si_stime) + "    = System time consumed", 4);
    output::instance().addOutput("si_int:     " + glib::stringFromInt(i_info->si_int) + "    = POSIX.1b signal", 4);
    output::instance().addOutput("si_ptr:     " + ssi_ptr + "    = POSIX.1b signal", 4);
#endif
// SegFault specific (probably)
    std::ostringstream ost_addr;
    ost_addr << i_info->si_addr;
    std::string ssi_addr=ost_addr.str();
    output::instance().addOutput("si_addr:    " + ssi_addr + "    = Memory location which caused fault", 4);
    output::instance().addOutput("si_band:    " + glib::stringFromInt(i_info->si_band) + "    = Band event", 4);
#if defined(linux) || defined(__linux) || defined(__linux__)
    output::instance().addOutput("si_fd:      " + glib::stringFromInt(i_info->si_fd) + "    = File descriptor", 4);
#endif

    throw * i_info;
}

void TermAction(int i_num, siginfo_t * i_info, void * i_val)
{
    output::instance().addStatus(false, "Term Action");
    output::instance().addOutput("Term Signal " + glib::stringFromInt(i_num) + " caught...", 4);
    output::instance().addOutput("si_signo:   " + glib::stringFromInt(i_info->si_signo) + "    = Signal number", 4);
    output::instance().addOutput("si_pid:     " + glib::stringFromInt(i_info->si_pid) + "    = Sending process ID", 4);
    output::instance().addOutput("si_errno:   " + glib::stringFromInt(i_info->si_errno) + "    = An errno value", 4);
    output::instance().addOutput("si_code:    " + glib::stringFromInt(i_info->si_code) + "    = Signal code", 4);
    output::instance().addOutput("si_uid:     " + glib::stringFromInt(i_info->si_uid) + "     = Real user ID of sending process", 4);
    output::instance().addOutput("si_status:  " + glib::stringFromInt(i_info->si_status) + "    = Exit value or signal", 4);

#if defined(linux) || defined(__linux) || defined(__linux__)
    std::ostringstream ost_ptr;
    ost_ptr << i_info->si_ptr;
    std::string ssi_ptr = ost_ptr.str();
    output::instance().addOutput("si_utime:   " + glib::stringFromInt(i_info->si_utime) + "    = User time consumed", 4);
    output::instance().addOutput("si_stime:   " + glib::stringFromInt(i_info->si_stime) + "    = System time consumed", 4);
    output::instance().addOutput("si_int:     " + glib::stringFromInt(i_info->si_int) + "    = POSIX.1b signal", 4);
    output::instance().addOutput("si_ptr:     " + ssi_ptr + "    = POSIX.1b signal", 4);
#endif
    throw * i_info;
}

void Usr1Action(int i_num, siginfo_t * i_info, void * i_val)
{
    output::instance().addStatus(true, "USR1 Action (logrotate)");
    output::instance().addOutput("User Signal " + glib::stringFromInt(i_num) + " caught...", 4);
    output::instance().addOutput("si_signo:   " + glib::stringFromInt(i_info->si_signo) + "    = Signal number", 4);
    output::instance().addOutput("si_pid:     " + glib::stringFromInt(i_info->si_pid) + "    = Sending process ID", 4);
    output::instance().addOutput("si_errno:   " + glib::stringFromInt(i_info->si_errno) + "    = An errno value", 4);
    output::instance().addOutput("si_code:    " + glib::stringFromInt(i_info->si_code) + "    = Signal code", 4);
    output::instance().addOutput("si_uid:     " + glib::stringFromInt(i_info->si_uid) + "     = Real user ID of sending process", 4);
    output::instance().addOutput("si_status:  " + glib::stringFromInt(i_info->si_status) + "    = Exit value or signal", 4);

#if defined(linux) || defined(__linux) || defined(__linux__)
    std::ostringstream ost_ptr;
    ost_ptr << i_info->si_ptr;
    std::string ssi_ptr = ost_ptr.str();
    output::instance().addOutput("si_utime:   " + glib::stringFromInt(i_info->si_utime) + "    = User time consumed", 4);
    output::instance().addOutput("si_stime:   " + glib::stringFromInt(i_info->si_stime) + "    = System time consumed", 4);
    output::instance().addOutput("si_int:     " + glib::stringFromInt(i_info->si_int) + "    = POSIX.1b signal", 4);
    output::instance().addOutput("si_ptr:     " + ssi_ptr + "    = POSIX.1b signal", 4);
#endif
    output::instance().closeLog();
    struct timespec req, rem;
    req.tv_sec = 1;
    req.tv_nsec = 0;
    rem.tv_sec = 0;
    rem.tv_nsec = 0;
    nanosleep(&req, &rem);
    output::instance().openLog();
    std::string startBlock = "+++++++++++++++++++++++++++++++++++++++++++++++";
    output::instance().addOutput(startBlock, 2);
    output::instance().addOutput("+ SIGUSR1 (rotate log) on " + output::instance().sFormatTime("%d-%m-%Y %H:%M:%S") + " +", 2);
    output::instance().addOutput(startBlock, 2);
}

void mainbase::SetupSignal()
{
    struct sigaction new_action;
    struct sigaction old_action;
    /* Set up the structure to specify the new action. */
    sigemptyset (&new_action.sa_mask);
    new_action.sa_flags = SA_SIGINFO
#if defined(linux) || defined(__linux) || defined(__linux__)
      | SA_NOMASK
#endif
      ;

// usr (rotate log) (signal 10)
    new_action.sa_sigaction = Usr1Action;
    sigaction (SIGUSR1, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction (SIGUSR1, &new_action, NULL);

// termination
    new_action.sa_sigaction = TermAction;
    sigaction (SIGINT, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction (SIGINT, &new_action, NULL);

    sigaction (SIGHUP, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction (SIGHUP, &new_action, NULL);

    sigaction (SIGTERM, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction (SIGTERM, &new_action, NULL);

#ifdef GDBDEBUG
    new_action.sa_flags = SA_SIGINFO
      | SA_RESTART
#if defined(linux) || defined(__linux) || defined(__linux__)
      | SA_NOMASK
#endif
      ;
    new_action.sa_sigaction = gdb_sighandler;

    sigaction (SIGSEGV, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGSEGV, &new_action, NULL);

    sigaction (SIGFPE, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGFPE, &new_action, NULL);

    sigaction (SIGBUS, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGBUS, &new_action, NULL);

    sigaction (SIGKILL, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGILL, &new_action, NULL);

    sigaction (SIGABRT, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGABRT, &new_action, NULL);
#else
// SegFault
    new_action.sa_sigaction = SegFaultAction;
    sigaction (SIGSEGV, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction (SIGSEGV, &new_action, NULL);

#endif

}


mainbase::mainbase(std::string name) :
m_INeedRoot(false),
m_DropRoot(false),
m_Foreground(false)
{
    m_Name = name;
    m_Syslog = m_Name;
#ifdef USE_FILELOG
    m_LogFileLocation = "log/";
    m_LogFile = m_LogFileLocation + m_Name + ".log";
#endif
    m_PidFileLocation = "/var/run/" + m_Name + "/";
    m_PidFile = m_PidFileLocation + m_Name + ".pid";
    m_IniFile = "conf/" + m_Name + ".ini";
    SetupSignal();
    char *CopyrightChar = "\xa9";
    versions::instance().addVersion("Copyright " + std::string(CopyrightChar) + " 2012 Gijs Kwakkel");
    versions::instance().addVersion("GNU Version 2");
    versions::instance().addVersion(gNAME + " " + gVERSION + " " + gGITVERSION);
    addHelpItem("Runs the " + m_Name +
        " (default as " + m_Name +
        ", " + m_PidFile +
#ifdef USE_FILELOG
        ", " + m_LogFile +
#endif
        ", " + m_IniFile +
        ")");
    addHelpItem("USAGE " + m_Name + " [OPTIONS]");
    addHelpItem("Available options:");
    addHelpItem("\t-h, --help List options");
    addHelpItem("\t-v, --version Output version and exit");
    addHelpItem("\t-f, --foreground Don't fork into the background (automaticly if debug)");
    addHelpItem("\t-dr, --droproot [username] [group] run as");
    addHelpItem("\t-c, --config Set config file (default: " + m_IniFile + ")");
    addHelpItem("\t-d, --debug Set debug level [1-10] (default: 5) (implies foreground)");
    addHelpItem("\t-p, --pid Set Pid file location (default: " + m_PidFileLocation + ")");
#ifdef USE_SYSLOG
    addHelpItem("\t-s, --syslog Set syslog name (default: " + m_Syslog + ")");
#endif
#ifdef USE_FILELOG
    addHelpItem("\t-l, --log Set log file location (default: " + m_LogFileLocation + ")");
#endif
    addHelpItem("\t-n, --name Set name for pid/log files (default: " + m_Name + ")");
    addHelpItem("\t--INeedRootPowerz Requered when running as root (not needed when droproot is specified)");
}

mainbase::~mainbase()
{
    remove(m_PidFile.c_str());
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
        else if (args[nArg] == "--version" || args[nArg] == "-v")
        {
            versions::instance().showVersion();
            exit(EXIT_SUCCESS);
        }
        else if (args[nArg] == "--foreground" || args[nArg] == "-f")
        {
            m_Foreground = true;
        }
        else if (args[nArg] == "--droproot" || args[nArg] == "-dr")
        {
            if ((++nArg) < args.size())
            {
                if ((++nArg) < args.size())
                {
                    m_Uid = args[nArg-1];
                    m_Gid = args[nArg];
                    m_DropRoot = true;
                }
            }
        }
        else if (args[nArg] == "--config" || args[nArg] == "-c")
        {
            if ((++nArg) < args.size())
            {
                m_IniFile = args[nArg];
            }
        }
        else if (args[nArg] == "--debug" || args[nArg] == "-d")
        {
            if ((++nArg) < args.size())
            {
                int i;
                std::stringstream ss(args[nArg]);
                ss >> i;
                output::instance().setDebugLevel(i);
                m_Foreground = true;
            }
        }
        else if (args[nArg] == "--pid" || args[nArg] == "-p")
        {
            if ((++nArg) < args.size())
            {
                m_PidFileLocation = args[nArg];
            }
        }
#ifdef USE_SYSLOG
        else if (args[nArg] == "--syslog" || args[nArg] == "-s")
        {
            if ((++nArg) < args.size())
            {
                m_Syslog = args[nArg];
            }
        }
#endif
#ifdef USE_FILELOG
        else if (args[nArg] == "--log" || args[nArg] == "-l")
        {
            if ((++nArg) < args.size())
            {
                m_LogFileLocation = args[nArg];
            }
        }
#endif
        else if (args[nArg] == "--name" || args[nArg] == "-n")
        {
            if ((++nArg) < args.size())
            {
                m_Name = args[nArg];
            }
        }
        else if (args[nArg] == "--INeedRootPowerz")
        {
            m_INeedRoot = true;
        }
    }
}

int mainbase::run()
{
    m_PidFile = m_PidFileLocation + m_Name + ".pid";
#ifdef USE_FILELOG
    m_LogFile = m_LogFileLocation + m_Name + ".log";
#endif
    versions::instance().showVersion();
    if (readPidFile())
    {
        return 1;
    }
#ifdef USE_FILELOG
    createDirectory(m_LogFileLocation);
#endif
    createDirectory(m_PidFileLocation);
    if (isRoot() && !m_DropRoot)
    {
        output::instance().addOutput("You are running " + m_Name + "as root!", 4);
        output::instance().addOutput("This is dangerouse and can cause great damage!", 4);
        if (!m_INeedRoot) {
            return 1;
        }
        output::instance().addOutput("You have been warned", 4);
        output::instance().addOutput("Hit CTRL+C now if you don't want to run " + m_Name + " as root.", 4);
        output::instance().addOutput(m_Name + " will start in 15 seconds.", 4);
        sleep(15);
    }
    if (m_DropRoot)
    {
        output::instance().addOutput("dropping root", 2);
        if (!dropRoot())
        {
            return 1;
        }
    }
    int DaemonizeStatus = daemonize(!m_Foreground);
    if (DaemonizeStatus != -1)
    {
        return DaemonizeStatus;
    }
#ifdef USE_FILELOG
    output::instance().setLogFile(m_LogFile);
#endif
    output::instance().setSyslog(m_Syslog);
    output::instance().openLog();
    std::string startBlock = "+++++++++++++++++++++++++++++++++";
    for (size_t m_Name_Iterator = 0; m_Name_Iterator < m_Name.size(); m_Name_Iterator++)
    {
        startBlock = startBlock + "+";
    }
    output::instance().addOutput(startBlock, 2);
    output::instance().addOutput("+ Start " + m_Name + " on " + output::instance().sFormatTime("%d-%m-%Y %H:%M:%S") + " +", 2);
    output::instance().addOutput(startBlock, 2);
    configreader::instance().set_ConfigFile(m_IniFile);
    if(!configreader::instance().readFile())
    {
        return 1;
    }
    struct timespec req, rem;
    req.tv_sec = 2;
    req.tv_nsec = 0;
    rem.tv_sec = 0;
    rem.tv_nsec = 0;
    nanosleep(&req, &rem);
    return -1;
}

bool mainbase::createDirectory(std::string directory)
{
    struct stat sb;
    if (stat(directory.c_str(), &sb) != 0)
    {
        umask(0);
        output::instance().addStatus(true, "creating " + directory);
        mkdir(directory.c_str(), S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO);
        return true;
    }
    else
    {
        if (!S_ISDIR(sb.st_mode))
        {
            output::instance().addStatus(false, directory + "exists but is not a directory!");
            return false;
        }
    }
    return true;
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
        output::instance().addStatus(false, "Still running");
        return true;
        //exit(EXIT_FAILURE);
    }
    return false;
    // end check
}

void mainbase::writePidFile(int Pid)
{
    // write current pid to pidfile
    if (Pid < 0) {
        output::instance().addStatus(false, "pid <0 FAIL");
        exit(EXIT_FAILURE);
    }

    std::ofstream ofPidFile (m_PidFile.c_str());
    output::instance().addOutput(m_PidFile);
    if (ofPidFile.is_open())
    {
        ofPidFile << Pid;
        ofPidFile.close();
    }
    else
    {
        output::instance().addStatus(false, "cant open pid file");
        exit(EXIT_FAILURE);
    }
    // end writing pid file
}

bool mainbase::isRoot()
{
#if defined(linux) || defined(__linux) || defined(__linux__)
    // User root? If one of these were root, we could switch the others to root, too
    if ((geteuid() == 0) || (getuid() == 0) || (getegid() == 0) || (getgid()== 0))
    {
        return true;
    }
    else
    {
        return false;
    }
    //return (geteuid() == 0 || getuid() == 0);
#else
//#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__)
    return false;
#endif
}

bool mainbase::dropRoot()
{
    struct passwd pwd;
    struct passwd *pwdresult;
    struct group grp;
    struct group *grpresult;
    char *pwdbuf;
    char *grpbuf;
    size_t pwdbufsize;
    size_t grpbufsize;
    int pwdstatus;
    int grpstatus;
    long int pwsizemax;
    long int grsizemax;

    pwsizemax = sysconf(_SC_GETPW_R_SIZE_MAX);
    if ( pwsizemax == -1 )         /* Value was indeterminate */
    {
        pwdbufsize = 16384;        /* Should be more than enough */
    }
    pwdbufsize = (size_t)pwsizemax;

    pwdbuf = (char*) malloc(pwdbufsize);
    if (pwdbuf == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    grsizemax = sysconf(_SC_GETGR_R_SIZE_MAX);
    if ( grsizemax == -1 )         /* Value was indeterminate */
    {
        grpbufsize = 16384;        /* Should be more than enough */
    }
    grpbufsize = (size_t)grsizemax;

    grpbuf = (char*) malloc(grpbufsize);
    if (grpbuf == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    pwdstatus = getpwnam_r(m_Uid.c_str(), &pwd, pwdbuf, pwdbufsize, &pwdresult);
    if ( pwdresult == NULL )
    {
        if ( pwdstatus == 0 )
        {
            output::instance().addStatus(false, "User not found");
        }
        else
        {
            output::instance().addStatus(false, "Error and User not found?");
        }
        return false;
    }

    grpstatus = getgrnam_r(m_Gid.c_str(), &grp, grpbuf, grpbufsize, &grpresult);
    if ( grpresult == NULL )
    {
        if ( grpstatus == 0 )
        {
            output::instance().addStatus(false, "Group not found");
        }
        else
        {
            output::instance().addStatus(false, "Error and Group not found?");
        }
        return false;
    }

    int gid = grp.gr_gid;
    int uid = pwd.pw_uid;
    if ( gid == 0 || uid == 0 )
    {
        output::instance().addStatus(false, "Cannot run as root");
        return false;
    }
    if ((geteuid() == 0) || (getuid() == 0) || (getegid() == 0) || (getgid()== 0))
    {
        int u, eu, g, eg, sg;
        sg = setgroups(0, NULL);
        if (sg < 0)
        {
            output::instance().addStatus(false, "Could not remove supplementary groups!");
            return false;
        }
        g = setgid(grp.gr_gid);
        eg = setegid(grp.gr_gid);
        if ((g < 0) || (eg < 0))
        {
            output::instance().addStatus(false, "Could not switch group id!");
            return false;
        }
        u = setuid(pwd.pw_uid);
        eu = seteuid(pwd.pw_uid);
        if ((u < 0) || (eu < 0))
        {
            output::instance().addStatus(false, "Could not switch user id!");
            return false;
        }
        return true;
    }
    output::instance().addStatus(false, "End of dropRoot, are you root?");
    return false;
}

void mainbase::showHelp()
{
    for (size_t HelpItems_iterator = 0; HelpItems_iterator < m_HelpItems.size(); HelpItems_iterator++)
    {
        output::instance().addOutput(m_HelpItems[HelpItems_iterator]);
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
        output::instance().addOutput("Staying open for debugging");
        output::instance().addOutput("pid [" + glib::stringFromInt(Pid) + "]");
    }
    else
    {
        output::instance().addOutput("Forking into the background");

        int Pid = fork();

        if (Pid == -1) {
            output::instance().addStatus(false, strerror(errno));
            return 1;
        }

        if (Pid > 0) {
            // We are the parent. We are done and will go to bed.
            writePidFile(Pid);
            output::instance().addStatus(true, "pid [" + glib::stringFromInt(Pid) + "]");
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
        output::instance().addStatus(true, "child running");
    }
    return -1;
}
