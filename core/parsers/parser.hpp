#pragma once
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include "parseerror.hpp"

class Parser
{
public:
    typedef std::unique_ptr<Parser> pointer;
    virtual void parse() = 0;
    virtual void update() = 0;
};
