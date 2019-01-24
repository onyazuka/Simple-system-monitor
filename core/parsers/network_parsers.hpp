#pragma once
#include <unordered_map>
#include <vector>
#include <queue>
#include "parser.hpp"

class NetworkParser : public Parser
{
public:
    typedef std::unique_ptr<NetworkParser> pointer;
    virtual void parse() = 0;
    virtual void update() = 0;
    virtual uint64_t getIncome() const = 0;
    virtual uint64_t getOutcome() const = 0;
};

class NetstatNetworkParser : public NetworkParser
{
public:
    typedef std::unordered_map<std::string, uint64_t> IPInfo;
    typedef std::vector<std::string> Keys;
    typedef std::vector<uint64_t> Values;
    typedef std::unique_ptr<NetstatNetworkParser> pointer;
    NetstatNetworkParser(const std::string& _netstatFile);
    void parse() final;
    void update() final;
    inline uint64_t getIncome() const final { return ipinfo.at("InOctets"); }
    inline uint64_t getOutcome() const final { return ipinfo.at("OutOctets"); }
    Keys keys();
    Values values();
private:
    std::string netstatFile;
    IPInfo ipinfo;
};
