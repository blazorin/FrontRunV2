#include <iostream>
#include "Controllers/Cache/Config.h"
#include <iostream>
#include <string>
#include <future>

int main() {
    std::cout << "\n\nFrontRunV2 se acaba de iniciar\n\n" << std::endl;

    PolyJson::RegisterMappings();
    Config::StartupCache();
    Config::StartupWallets();
    Config::StartupApprovedTokens();
    Config::StartupBlacklist();

    std::cout << "Config loaded" << std::endl << std::endl;
    std::cout << "Test wallet from token USDC" << Config::token_to_wallet_map["0x2791bca1f2de4661ed88a30c99a7a9449aa84174"]->wallet_addr << std::endl;

    // Stuff starts here



    // Keep main
    std::cout << std::endl << std::endl << "STATUS: RUNNING" << std::endl;
    std::cin.ignore();

    return 0;
}
