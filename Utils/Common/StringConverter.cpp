//
// Created by Victor Ruiz Suñe on 20/04/2022.
//

#include "StringConverter.h"

std::vector<std::string> StringConverter::split(std::string str, int offset) {
    if( std::all_of( std::begin(str), std::end(str), [] ( char c ) { return std::isspace(c) ; } ) ){
        return {} ; // empty string or string with all spaces, return an empty vector
    }

    if( (str.length() % offset) != 0){
        return {}; // If the provided string length is not divisible by the offset then we return an empy string.
    }

    std::vector<std::string> result;
    for (int i = 0; i < (str.length()/offset); ++i) {
        result.push_back(str.substr(i*offset, offset));
    }


    return result;
}

std::string& StringConverter::remove0x(std::string& str) {

    if( str[0] == '0' && (str[1] == 'X' || str[1] == 'x') ){
        str.erase(0,2);
    }

    return str;
}


std::string StringConverter::RemoveDecimalOnString(std::string subject, char search)
{
    return subject.substr(0, subject.find(search, 0));
}

std::string StringConverter::hexToASCII(std::string hex)
{
    std::string ascii = "";
    for (size_t i = 0; i < hex.length(); i += 2)
    {
        std::string part = hex.substr(i, 2);
        char ch = stoul(part, nullptr, 16);

        ascii += ch;
    }
    return ascii;
}

std::vector<std::string> StringConverter::ASCIIClean(std::string ascii) {

    std::vector<std::string> asciiSplit = StringConverter::split(ascii, 64);

    asciiSplit.erase(asciiSplit.begin(), asciiSplit.begin() + 6); // del 0 al index 6

    //std::string te = R"(0\)";
    //te = te.substr(0, te.length() -1 );

    int count = 0;
    for (std::string& elem : asciiSplit) {

        /*if ((elem.find("0x") <= elem.size() - 1) && (elem.find('U') <= elem.size() - 1)) {
            elem = elem.substr(elem.find("0x"), elem.size() - 1);
         }*/

        switch (count) {

            case 0:
                elem = elem.substr(elem.find("0x"), elem.size());
                break;
            case 1:
                asciiSplit[0] += elem;
                elem = "";
                break;
            case 2:
                asciiSplit[0] += elem.substr(0, 31);
                elem = "";
                break;
            case 3:
                asciiSplit[1] += elem;
                elem = "";
                break;
            case 4:
                asciiSplit[1] += elem.substr(0, elem.size() - 1);
                elem = "";
                break;
            case 5:
                asciiSplit[2] += elem.substr(elem.find("0x"), elem.size());
                elem = "";
                break;
            case 6:
                asciiSplit[2] += elem;
                elem = "";
                break;
            case 7:
                asciiSplit[2] += elem.substr(0, 31);
                elem = "";
                break;
            case 8:
                asciiSplit[3] += elem;
                elem = "";
                break;
            case 9:
                asciiSplit[3] += elem.substr(0, elem.size() - 1);
                elem = "";
                break;
            case 10:
                asciiSplit[4] += elem.substr(elem.find("0x"), elem.size());
                elem = "";
                break;
            case 11:
                asciiSplit[4] += elem;
                elem = "";
                break;
            case 12:
                asciiSplit[4] += elem.substr(0, 31);
                elem = "";
                break;
            case 13:
                asciiSplit[5] += elem;
                elem = "";
                break;
            case 14:
                asciiSplit[5] +=  elem.substr(0, elem.size() - 1);
                elem = "";
                break;
            case 15:
                asciiSplit[6] += elem.substr(elem.find("0x"), elem.size());
                elem = "";
                break;
            case 16:
                asciiSplit[6] += elem;
                elem = "";
                break;
            case 17:
                asciiSplit[6] += elem.substr(0, 31);
                elem = "";
                break;
            case 18:
                asciiSplit[7] += elem;
                elem = "";
                break;
            case 19:
                asciiSplit[7] += elem.substr(0, elem.size() - 1);
                elem = "";
                break;
            case 20:
                asciiSplit[8] += elem.substr(elem.find("0x"), elem.size());
                elem = "";
                break;
            case 21:
                asciiSplit[8] += elem;
                elem = "";
                break;
            case 22:
                asciiSplit[8] += elem.substr(0, 31);
                elem = "";
                break;
            case 23:
                asciiSplit[9] += elem;
                elem = "";
                break;
            case 24:
                asciiSplit[9] +=  elem.substr(0, elem.size() - 1);
                elem = "";
                break;
            default:
                break;
        }


        count++;
    }

    for (std::string& i : asciiSplit) {
        if (i.empty())
            i.erase();
    }

    return asciiSplit;
}

