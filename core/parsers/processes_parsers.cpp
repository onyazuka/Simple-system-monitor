#include "processes_parsers.hpp"

#ifdef __linux__

void LinuxProcessesParser::parse()
{
    proceccInfoMap.clear();
    findAllProcesses();
    parseProcesses();
}

void LinuxProcessesParser::update()
{
    parse();
}

/*
    all directories like /proc/PID
*/
void LinuxProcessesParser::findAllProcesses()
{
    for(auto dir : fs::directory_iterator("/proc/"))
    {
        if(!fs::is_directory(dir.path()))
        {
            continue;
        }
        std::string procName = dir.path();
        // splitting PID from path
        size_t lastSlashIndex = procName.rfind('/');
        // not separate processes
        if(lastSlashIndex == procName.npos || lastSlashIndex == procName.size() - 1)
        {
            continue;
        }
        procName = procName.substr(lastSlashIndex + 1, procName.npos);
        PID pid;
        try
        {
            pid = std::stoi(procName);
        }
        // not separate processes
        catch(std::invalid_argument&)
        {
            continue;
        }
        proceccInfoMap[pid] = LinuxProcessInfo();
        proceccInfoMap[pid].procinfo.path = dir.path();
    }
    if(proceccInfoMap.size() == 0)
    {
        throw ProcessesParseError("Processes parser: not found any process");
    }
}

void LinuxProcessesParser::parseProcesses()
{
    // for deleting system processes
    std::stack<int> deleteStack;
    for(auto& process : proceccInfoMap)
    {
        if(!parseProcessName(process.second.procinfo.path, process.first))
        {
            deleteStack.push(process.first);
        }
        else    // if we will delete process anyways, we are not have to parse its stats
        {
            parseProcessStats(process.second.procinfo.path, process.first);
            parseIO(process.second.procinfo.path, process.first);
        }
    }
    // nothing has been parsed - throwing
    if(proceccInfoMap.size() == 0)
    {
        throw ProcessesParseError("Processes Parser: processes was not parsed");
    }
    // clearing system processes
    while(!deleteStack.empty())
    {
        proceccInfoMap.erase(deleteStack.top());
        deleteStack.pop();
    }
}

/*
    looking full process name in /proc/PID/cmdline
*/
bool LinuxProcessesParser::parseProcessName(const std::string& path, PID pid)
{
    std::string cmdlinePath = path + "/cmdline";
    std::ifstream ifs(cmdlinePath);
    if(!ifs)
    {
        // we can simply not have rights to read file
        return false;
    }
    std::string procPath;
    ifs >> procPath;
    // getting file name only
    // if cmdline is empty, it means that the process is system and we will not show it
    if(procPath.empty())
    {
        return false;
    }
    procPath = procPath.substr(0, procPath.find('\0'));
    std::string procName = procPath.substr(procPath.rfind('/') + 1, procPath.npos);
    proceccInfoMap[pid].procinfo.procName = procName;
    return true;
}

/*
    looking stats in /proc/PID/stat
*/
void LinuxProcessesParser::parseProcessStats(const std::string& path, PID pid)
{
    std::string statPath = path + "/stat";
    std::ifstream ifs(statPath);
    if(!ifs)
    {
        // we can simply not have rights to read file
        return;
    }
    LinuxProcInfo pinfo;
    ifs >> pinfo.pid;

    std::string exName;
    std::string exNameWord;
    ifs >> exNameWord;
    exName += exNameWord;
    // in stat file exName always starts with '(' and ends with ')'
    while(exName[exName.size() - 1] != ')')
    {
        ifs >> exNameWord;
        exName += " " + exNameWord;
    }
    // exName must be at most 15 characters + \0
    int toCopy = exName.size() > 15 ? 15 : exName.size();
    std::memcpy(pinfo.exName, &exName[0], toCopy);
    pinfo.exName[toCopy] = 0;
    ifs >>  pinfo.state >>
            pinfo.ppid >>
            pinfo.pgrp >>
            pinfo.session >>
            pinfo.tty >>
            pinfo.tpgid >>
            pinfo.flags >>
            pinfo.minflt >>
            pinfo.cminflt >>
            pinfo.majflt >>
            pinfo.cmajflt >>
            pinfo.utime >>
            pinfo.stime >>
            pinfo.cutime >>
            pinfo.cstime >>
            pinfo.counter >>
            pinfo.priority >>
            pinfo.timeout >>
            pinfo.itrealvalue >>
            pinfo.starttime >>
            pinfo.vsize >>
            pinfo.rss >>
            pinfo.rlim >>
            pinfo.startcode >>
            pinfo.endcode >>
            pinfo.startstack >>
            pinfo.kstkesp >>
            pinfo.kstkeip >>
            pinfo.signal >>
            pinfo.blocked >>
            pinfo.sigignore >>
            pinfo.sigcatch >>
            pinfo.wchan;
    pinfo.procName = proceccInfoMap[pid].procinfo.procName;
    pinfo.path = proceccInfoMap[pid].procinfo.path;
    proceccInfoMap[pid].procinfo = std::move(pinfo);
}

/*
    Looks for processes io activities in /proc/PID/io
*/
void LinuxProcessesParser::parseIO(const std::string& path, PID pid)
{
    std::string iopath = path + "/io";
    std::ifstream ifs(iopath);
    if(!ifs)
    {
        // we can simply not have rights to read file
        return;
    }
    LinuxIoInfo ioInfo;
    std::string key;
    ifs >> key >> ioInfo.rchar >> key >> ioInfo.wchar >> key >> ioInfo.syscr >>
            key >> ioInfo.syscw >> key >> ioInfo.read_bytes >> key >> ioInfo.write_bytes >>
            key >> ioInfo.cancelled_write_bytes;
    if(ifs.fail() && !ifs.eof())
    {
        throw ProcessesParseError("Processes Parser: cannot parse process io of " + path);
    }
    proceccInfoMap[pid].ioinfo = std::move(ioInfo);
}

#endif
