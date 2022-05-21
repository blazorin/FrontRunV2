//
// Created by Alberto Romero on 18/4/22.
//

#pragma once

#include "../../lib/baseconverter/baseconverter.h"
#include <algorithm>
#include <string>

class PolyHex {

    const static BaseConverter& HexToDecimalConverter;
    const static BaseConverter& DecimalToHexConverter;

    public:

        static std::string ToHex(std::string decimalOnString);
        static std::string ToDecimal(std::string hexOnString);
};
