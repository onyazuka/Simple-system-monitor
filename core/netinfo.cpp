#include "netinfo.hpp"

NetInfo::NetInfo(const std::string &infoFile)
    : netInfoFile{infoFile}
{
    parseNetInfoFile(netInfoFile);
}

NetInfo NetInfo::fromNetInfoFile(const std::string &infoFile)
{
    NetInfo netinfo(infoFile);
    return netinfo;
}

void NetInfo::update()
{
    parseNetInfoFile(netInfoFile);
}

NetInfo::Keys NetInfo::keys()
{
    Keys keys;
    for(auto kv: ipInfo)
    {
        keys.push_back(kv.first);
    }
    return keys;
}

NetInfo::Values NetInfo::values()
{
    Values values;
    for(auto kv : ipInfo)
    {
        values.push_back(kv.second);
    }
    return values;
}

// Warning! If this function throws out_of_range, than your netstat file is wrong!
uint64_t NetInfo::getIncome()
{
    return ipInfo.at("InOctets");
}

// Warning! If this function throws out_of_range, than your netstat file is wrong!
uint64_t NetInfo::getOutcome()
{
    return ipInfo.at("OutOctets");
}

/*
    Fills IPInfo dictionary;
    Returns 0 on success, 1 on failure.
*/
void NetInfo::parseNetInfoFile(const std::string& file)
{
    // initting
    std::ifstream ifs(file);
    if(!ifs)
    {
        throw NetworkParseError("NetInfo::parseNetInfoFile(): can not open file");
    }
    // memoizing order of keys arrival to write values later
    std::queue<std::string> keysQueue;
    std::string curWord;
    uint64_t curVal;
    const std::string magicWord = "IpExt:";

    // searching for magin word
    bool foundMagicWord = false;
    while(ifs)
    {
        ifs >> curWord;
        if(curWord == magicWord)
        {
            foundMagicWord = true;
            break;
        }
    }
    if(!foundMagicWord)
    {
        throw NetworkParseError("NetInfo::parseNetInfoFile(): invalid netstat file");
    }

    // reading keys
    foundMagicWord = false;
    while(ifs)
    {
        ifs >> curWord;
        if(curWord == magicWord)
        {
            foundMagicWord = true;
            break;
        }
        ipInfo[curWord] = 0;
        keysQueue.push(curWord);
    }
    if(!foundMagicWord)
    {
        ipInfo.clear();
        throw NetworkParseError("NetInfo::parseNetInfoFile(): invalid netstat file");
    }

    // reading values
    while(ifs)
    {
        ifs >> curVal;
        if(!ifs)
        {
            break;
        }
        ipInfo[keysQueue.front()] = curVal;
        keysQueue.pop();
    }
}
