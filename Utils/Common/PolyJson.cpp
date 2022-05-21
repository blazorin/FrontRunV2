//
// Created by Alberto Romero on 17/4/22.
//

#include "PolyJson.h"

Json::Reader* PolyJson::Reader = new Json::Reader();

const std::string PolyJson::FilesPath = "json_data/";

std::string PolyJson::ReadJsonFrom(const std::string&& file) {

    std::stringstream path;
    path << "../" << FilesPath << file;

    std::ifstream jsonFile(path.str());
    std::cout << path.str() << std::endl;



    Json::Value jsonInstance;

    PolyJson::Reader->parse(jsonFile, jsonInstance);

    return jsonInstance.toStyledString();
}

Json::Value PolyJson::ReadJsonFromRaw(const std::string&& file) {

    std::stringstream path;
    path << "../" << FilesPath << file;

    std::ifstream jsonFile(path.str());
    std::cout << path.str() << std::endl;



    Json::Value jsonInstance;

    PolyJson::Reader->parse(jsonFile, jsonInstance);

    return jsonInstance;
}

std::istringstream PolyJson::GetStringStream(std::string &string) {
    std::istringstream stringStream(string);
    return stringStream;
}

void PolyJson::RegisterMappings() {

    // server.json
    struct_mapping::reg(&Server::config_name, "config_name");
    struct_mapping::reg(&Server::ip, "ip");
    struct_mapping::reg(&Server::port, "port");
    struct_mapping::reg(&Server::webPort, "webPort");
    struct_mapping::reg(&Server::secure, "secure");
    struct_mapping::reg(&Server::polygonscanApiKey, "polygonscanApiKey");
    struct_mapping::reg(&Server::enableSmartContract, "enableSmartContract");
    struct_mapping::reg(&Server::quickswapSmartContractAddress, "quickswapSmartContractAddress");
    struct_mapping::reg(&Server::sushiswapSmartContractAddress, "sushiswapSmartContractAddress");
    struct_mapping::reg(&Server::gasLimit, "gasLimit");
    struct_mapping::reg(&Server::gasLimitMinimum, "gasLimitMinimum");
    struct_mapping::reg(&Server::gasMultiplier, "gasMultiplier");
    struct_mapping::reg(&Server::gasMultiplierApprove, "gasMultiplierApprove");
    struct_mapping::reg(&Server::maxGasPriceGwei, "maxGasPriceGwei");
    struct_mapping::reg(&Server::minGasPriceGwei, "minGasPriceGwei");
    struct_mapping::reg(&Server::gasUsedPromedium, "gasUsedPromedium");
    struct_mapping::reg(&Server::gasLimitPromedium, "gasLimitPromedium");
    struct_mapping::reg(&Server::gasLimitMaximum, "gasLimitMaximum");
    struct_mapping::reg(&Server::always0x2ForSell, "always0x2ForSell");
    struct_mapping::reg(&Server::gasWaveTresholdGwei, "gasWaveTresholdGwei");
    struct_mapping::reg(&Server::chain, "chain");
    struct_mapping::reg(&Server::maxPercentageBelowStandardGas, "maxPercentageBelowStandardGas");
    struct_mapping::reg(&Server::slippageUsage, "slippageUsage");

}

std::string PolyJson::RemoveChar(std::string&& str, char c)
{
    std::string result;
    for (size_t i = 0; i < str.size(); i++)
    {
        char currentChar = str[i];
        if (currentChar != c)
            result += currentChar;
    } // -50 129 46
    return result;
}
