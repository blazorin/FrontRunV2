//
// Created by Alberto Romero on 17/4/22.
//

#pragma once

/// Utility class to manage Json methods and keep reader Instance

#include <fstream>
#include <iostream>
#include <json/json.h>
#include <json/reader.h>
#include "../../mapper/include/struct_mapping/struct_mapping.h"
#include "../../Structs/Structs.h"

class PolyJson {

public:
    static Json::Reader *Reader;

    static const std::string FilesPath;
    static std::string ReadJsonFrom(const std::string&& file); // para que pasar un rvalue? pues lvalue => para el mapper
    static Json::Value ReadJsonFromRaw(const std::string&& file); // sin mapper
    static std::istringstream GetStringStream(std::string& string);

    static void RegisterMappings();
    static std::string RemoveChar(std::string&& str, char c);
};
