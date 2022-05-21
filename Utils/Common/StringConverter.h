//
// Created by Victor Ruiz Suñe on 20/04/2022.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>


class StringConverter {

public:
    static std::vector<std::string> split( std::string str, int offset);
    static std::string& remove0x(std::string& str);
    static std::string RemoveDecimalOnString(std::string subject, char search);
    static std::string hexToASCII(std::string hex);
    static std::vector<std::string> ASCIIClean(std::string ascii);
};



