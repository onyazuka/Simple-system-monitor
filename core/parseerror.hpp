#pragma once
#include <string>

/*
    Parsing exception classes
*/

class ParseError
{
public:
    ParseError(const std::string& error)
        : err{error} {}
    const std::string& what() const {return err;}
private:
        std::string err;
};

class CPUParseError : public ParseError
{
public:
    CPUParseError(const std::string& error)
        : ParseError{error} {}
};

class MemoryParseError : public ParseError
{
public:
    MemoryParseError(const std::string& error)
        : ParseError{error} {}
};

class NetworkParseError : public ParseError
{
public:
    NetworkParseError(const std::string& error)
        : ParseError{error} {}
};
