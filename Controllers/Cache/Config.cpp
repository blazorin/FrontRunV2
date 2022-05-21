//
// Created by Alberto Romero on 19/4/22.
//

#include "Config.h"
#include "../../Utils/Common/StringConverter.h"

Server Config::server;
Matic Config::matic;

rpc_message_new_pending_transaction Config::PendingTransaction;
bool Config::CacheLoaded = false;
bool Config::WalletsLoaded = false;

std::map<std::string,Wallet> Config::wallets;
std::map<std::string,Wallet*> Config::token_to_wallet_map;
std::map<addr,addr> Config::approvedTokensMap;

std::set<addr> Config::addrBlacklist;
std::set<addr> Config::tempAddrBlacklist;


void Config::StartupCache() {

    if (Config::CacheLoaded)
    {
        std::cout << "Startup cache is already loaded" << std::endl << std::endl;
        return;
    }

    // server

    std::string&& serverJson = PolyJson::ReadJsonFrom("server.json");
    std::istringstream serverStrStream = PolyJson::GetStringStream(serverJson);

    struct_mapping::map_json_to_struct(Config::server, serverStrStream);

    Config::CacheLoaded = true;

    if (Config::server.enableSmartContract && (Config::server.quickswapSmartContractAddress.empty() || Config::server.sushiswapSmartContractAddress.empty()) ) {
        throw std::runtime_error("enableSmartContract is true but there is no smartContractAddress (quick/sushi (or both) are empty)");
    }
}
void Config::StartupWallets(){

    if (Config::WalletsLoaded) {
        std::cout << "/!\\ Wallets were already loaded" << std::endl << std::endl;
        return;
    }


    Json::Value tokensJson = PolyJson::ReadJsonFromRaw("tokens.json");

    if (tokensJson["tokens"].empty()) return;

    for (auto& pair : tokensJson["tokens"]) {
        //std::cout << pair["symbol"].asString() << std::endl;
        Token tempToken;
        if(pair["wallet_addr"] == ""){
            tempToken.wallet_addr = tokensJson["default_wallet"].asString();
        }else{
            tempToken.wallet_addr = pair["wallet_addr"].asString();
        }

        tempToken.token_addr = pair["token_addr"].asString();
        tempToken.symbol = pair["symbol"].asString();
        tempToken.decimals = pair["decimals"].asInt();
        tempToken.balance = 0;


        //Here we check if the wallet is already created if it isn't we create one.
        Wallet tempWallet;
        if(wallets.find(tempToken.wallet_addr) == wallets.end()) {

            tempWallet.wallet_addr = tempToken.wallet_addr;
            tempWallet.attack_running = false;
            tempWallet.balance = BigDecimal(0);
            tempWallet.nonce = 0;
            wallets.emplace(tempToken.wallet_addr, tempWallet);
        }

        //Here we put the token to the wallet.
        wallets[tempToken.wallet_addr].tokens.emplace(tempToken.token_addr, tempToken);

        //Here we save the token to a map that is going to be used o acces the wallet that owns the token in a fast way.
        token_to_wallet_map.emplace(tempToken.token_addr, &wallets[tempToken.wallet_addr]);

    }


    Config::WalletsLoaded = true;
}

void Config::SaveMainCache() {

};

void Config::StartupApprovedTokens() {

    std::ifstream stream("../json_data/approved_tokens.txt");
    if (!stream.is_open()) {
        throw std::runtime_error("approved_tokens.txt is missing (IN)");
        std::cout << std::endl << "Where is approved_tokens.txt (IN) ?" << std::endl;

    }

    std::string tempLine;

    while (std::getline(stream, tempLine)) {
        if (tempLine.empty() || std::all_of(tempLine.begin(),tempLine.end(),isspace))
            continue;

        Config::approvedTokensMap.emplace(tempLine.substr(0, 42), tempLine.substr(43,  tempLine.length() - 1));
    }


    stream.close();
}

void Config::StartupBlacklist() {

    std::ifstream stream("../json_data/blacklist.txt");
    if (!stream.is_open()) {
        throw std::runtime_error("blacklist.txt is missing (IN)");
        std::cout << std::endl << "Where is blacklist.txt (IN) ?" << std::endl;
    }

    std::string tempLine;

    while (std::getline(stream, tempLine)) {
        if (tempLine.empty() || std::all_of(tempLine.begin(),tempLine.end(),isspace))
            continue;

        Config::addrBlacklist.emplace(tempLine);
    }


    stream.close();
}

bool Config::SaveApprovedToken(addr routerAddr, addr token) {

    std::ofstream stream("../json_data/approved_tokens.txt", std::ios_base::app);
    if (!stream.is_open()) {
        throw std::runtime_error("approved_tokens.txt is missing (OUT)");
        std::cout << std::endl << "Where is approved_tokens.txt (OUT) ?" << std::endl;

        return false;
    }

    Config::approvedTokensMap.emplace(token, routerAddr);

    stream << token << "/" << routerAddr + "\n";

    stream.close();
    return true;
}

bool Config::SaveToBlacklist(addr address) {

    std::ofstream stream("../json_data/blacklist.txt", std::ios_base::app);
    if (!stream.is_open()) {
        throw std::runtime_error("blacklist.txt is missing (IN)");
        std::cout << std::endl << "Where is blacklist.txt (IN) ?" << std::endl;

        return false;
    }

    Config::addrBlacklist.emplace(address);

    stream << "\n" + address;

    stream.close();
    return true;
}

int Config::getNumberOfTransactions(Wallet* wallet) {

    //Wallet::nonce

    Http client("http://" + Config::server.ip + ":" + Config::server.webPort + "/",
                HttpMethod::Post, {},"{\n"
                                     "\t\"jsonrpc\":\"2.0\",\n"
                                     "\t\"method\":\"eth_getTransactionCount\",\n"
                                     "\t\"params\":[\n"
                                     "    \""+wallet->wallet_addr+"\",\n"
                                                            "    \"latest\"\n"
                                                            "    ],\n"
                                                            "\t\"id\": 80\n"
                                                                                      "}");

    std::string jsonResponse = client.WebRequest();

    std::cout << "Get nonce data: " << jsonResponse << std::endl;
    //Parse the data and delete the json reader object.
    Json::Reader* jsonReader = new Json::Reader();
    Json::Value jsonInstance;
    jsonReader->parse(jsonResponse, jsonInstance);
    delete jsonReader;


    std::string result = jsonInstance["result"].asString();
    if(result.empty()) {
        std::cout << "WARNING!:: Error while fetching transaction count of wallet: " << wallet->wallet_addr << std::endl;
        return 0;
    }

    return stoi( PolyHex::ToDecimal(StringConverter::remove0x(result)) );
}

void Config::PedroFromBlacklist(addr address) {

    if (Config::tempAddrBlacklist.count(address) == 0) {
        std::cout << std::endl << "(!) Pedro da un ultimatum al From: " << address << std::endl;
        Config::tempAddrBlacklist.emplace(address);

        return;
    }

     std::cout << std::endl << "(!) Pedro ha castigado al From: " << address << std::endl;
     Config::SaveToBlacklist(address);
}

void Config::GetAndSavePairs(std::string filename) {


    int fromIndex = 0;
    int toPosition = 10;

    std::string request =  "0xe86d80d300000000000000000000000000000000000000000000000000000000000000" + PolyHex::ToHex(to_string(fromIndex)) + "00000000000000000000000000000000000000000000000000000000000000" + PolyHex::ToHex(to_string(toPosition));

    std::string gasParams;
    gasParams += "        \"gas\": \"0x" + PolyHex::ToHex("850000") + "\",\n";

    Http client("http://" + Config::server.ip + ":" + Config::server.webPort + "/",
                HttpMethod::Post, {},"{\n"
                                     "\t\"jsonrpc\":\"2.0\",\n"
                                     "\t\"method\":\"eth_call\",\n"
                                     "\t\"params\":[\n"
                                     "        {\n"
                                    // "        \"to\": \""+ Token + "\",\n"
                                     +gasParams+
                                     "\"data\": \""+request+"\"\n"
                                     "        }, \n"
                                     "    \"latest\"\n"
                                     "    ],\n"
                                     "\t\"id\":10\n"
                                     "}");
}