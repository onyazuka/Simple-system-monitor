#pragma once
#include <string>

/*
    Parsing exception classes
*/

class Error
{
public:
    Error(const std::string& error)
        : err{error} {}
    const std::string& what() const {return err;}
private:
        std::string err;
};

class ParseError : public Error
{
public:
    ParseError(const std::string& error)
        : Error{error} {}
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

class HddParseError : public ParseError
{
public:
    HddParseError(const std::string& error)
        : ParseError{error} {}
};

class ProcessesParseError : public ParseError
{
public:
    ProcessesParseError(const std::string& error)
        : ParseError{error} {}
};

class NotImplementedError : public Error
{
public:
    NotImplementedError(const std::string& error)
        : Error{error} {}
};
