//
// Created by Alberto Romero on 19/4/22.
//

#pragma once

#include <map>
#include <set>
#include <fstream>
#include <exception>
#include  "../../Structs/Structs.h"
#include "../../Utils/Common/PolyHex.h"
#include "../../Utils/Common/PolyJson.h"
#include "../../Utils/Common/StringConverter.h"
#include "../Http/Http.h"



class Config {
    typedef std::string addr;

    static bool CacheLoaded;
    static bool WalletsLoaded;

    static void GetAndSavePairs(std::string filename);

    public:

        static Server server;
        static Matic matic;
        static std::map<std::string,Wallet> wallets;
        static std::map<std::string,Wallet*> token_to_wallet_map;

        static std::map<addr, addr> approvedTokensMap;

        static std::set<addr> addrBlacklist;
        static std::set<addr> tempAddrBlacklist;

        static rpc_message_new_pending_transaction PendingTransaction;

        // sync
        static void StartupCache();
        static void StartupWallets();
        static void StartupApprovedTokens();
        static void StartupBlacklist();


        static bool SaveApprovedToken(addr routerAddr, addr token);
        static bool SaveToBlacklist(addr address);
        static void SaveMainCache(); // main cache to json files

        static int getNumberOfTransactions(Wallet* wallet);
        static void PedroFromBlacklist(addr address);


};
