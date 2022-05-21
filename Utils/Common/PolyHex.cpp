//
// Created by Alberto Romero on 18/4/22.
//

#include "PolyHex.h"

const BaseConverter& PolyHex::HexToDecimalConverter = BaseConverter::HexToDecimalConverter();
const BaseConverter& PolyHex::DecimalToHexConverter = BaseConverter::DecimalToHexConverter();

std::string PolyHex::ToHex(std::string decimalOnString)
{
    return DecimalToHexConverter.Convert(decimalOnString);
}

std::string PolyHex::ToDecimal(std::string hexOnString)
{
    std::transform(hexOnString.begin(), hexOnString.end(),hexOnString.begin(), ::toupper);

    if(hexOnString[0] == '0' && hexOnString[1] == 'X'){
        hexOnString.erase(0,2);
    }
    return HexToDecimalConverter.Convert(hexOnString);
}