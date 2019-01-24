#include "network_parsers.hpp"

NetstatNetworkParser::NetstatNetworkParser(const std::string &_netstatFile)
    : netstatFile{_netstatFile} {}

/*
    Fills IPInfo dictionary;
    Throws on failure.
*/
void NetstatNetworkParser::parse()
{
    // initting
    std::ifstream ifs(netstatFile);
    if(!ifs)
    {
        throw NetworkParseError("NetstatNetworkParser::parse(): can not open file");
    }
    // memoizing order of keys arrival to write values later
    std::queue<std::string> keysQueue;
    std::string curWord;
    uint64_t curVal;
    const std::string magicWord = "IpExt:";

    // searching for magic word
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
        throw NetworkParseError("NetstatNetworkParser::parse(): invalid netstat file");
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
        ipinfo[curWord] = 0;
        keysQueue.push(curWord);
    }
    if(!foundMagicWord)
    {
        ipinfo.clear();
        throw NetworkParseError("NetstatNetworkParser::parse(): invalid netstat file");
    }
    // "InOctets" and "OutOctets" keys must be presented
    if(ipinfo.find("InOctets") == ipinfo.end() || ipinfo.find("OutOctets") == ipinfo.end())
    {
        ipinfo.clear();
        throw NetworkParseError("NetstatNetworkParser::parse(): invalid netstat file");
    }

    // reading values
    while(ifs)
    {
        ifs >> curVal;
        if(!ifs)
        {
            break;
        }
        ipinfo[keysQueue.front()] = curVal;
        keysQueue.pop();
    }
}

void NetstatNetworkParser::update()
{
    parse();
}

NetstatNetworkParser::Keys NetstatNetworkParser::keys()
{
    Keys keys;
    for(auto kv: ipinfo)
    {
        keys.push_back(kv.first);
    }
    return std::move(keys);
}

NetstatNetworkParser::Values NetstatNetworkParser::values()
{
    Values values;
    for(auto kv : ipinfo)
    {
        values.push_back(kv.second);
    }
    return std::move(values);
}
