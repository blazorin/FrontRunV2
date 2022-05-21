//
// Created by Alberto Romero on 18/4/22.
//

#pragma once

#include <string>
#include <map>
#include "../../lib/BigDecimal/BigDecimal.h"



typedef std::string addr;
struct Server {
    std::string config_name;
    std::string ip;
    std::string port;
    std::string webPort;
    bool secure;
    std::string polygonscanApiKey;
    bool enableSmartContract;
    addr quickswapSmartContractAddress;
    addr sushiswapSmartContractAddress;
    double gasLimit;
    double gasLimitMinimum;
    double gasMultiplier;
    double gasMultiplierApprove;
    double maxGasPriceGwei;
    double minGasPriceGwei;
    double gasUsedPromedium;
    double gasLimitPromedium;
    double gasLimitMaximum;
    bool always0x2ForSell;
    double gasWaveTresholdGwei;
    double slippageUsage;
    std::string chain;
    double maxPercentageBelowStandardGas;
};

struct Matic {
    double FastPriceGwei;
    double StandardPriceGwei;
    double CoinPrice;
};

struct Token {
    addr token_addr;
    std::string symbol;
    int decimals;

    BigDecimal balance;
    addr wallet_addr;


};

struct Wallet {
    addr wallet_addr;
    std::map<addr, Token> tokens;
    BigDecimal balance;

    bool attack_running;


    int nonce;
};

struct GasResult {
    float GasPriceOnGasCoin;
    float GasPriceOnUsd;
    float UsdCostPromedium;

    int GasLimit;
};

struct PairData {

    addr pair_adr;

    addr token0;
    addr token1;

    BigDecimal reserve0;
    BigDecimal reserve1;

    int decimals;
};

struct GasParams {

    BigDecimal gasPrice;
    BigDecimal gasLimit;

    BigDecimal maxPriorityFeePerGas;
    BigDecimal maxFeePerGas;
};


//Start: Struct new transaction received from websocket.
struct rpc_result_new_pending_transaction {

    std::string type;
    std::string nonce;
    std::string gasPrice;
    std::string maxPriorityFeePerGas;
    std::string maxFeePerGas;
    std::string gas;
    std::string value;
    std::string input;
    std::string v;
    std::string r;
    std::string s;
    std::string to;
    std::string hash;

};

struct rpc_params_new_pending_transaction {
    std::string subscription;
    rpc_result_new_pending_transaction result;
};

struct rpc_message_new_pending_transaction
{
    std::string jsonrpc;
    std::string method;
    rpc_params_new_pending_transaction params;
};


struct Operation{
    addr op_addr;
    int nonce;
    GasParams gas;
    bool maxPriority;
    addr victim;

    BigDecimal ammount;
    addr token0;
    addr token1;

    std::string transaction_type;

    bool isPrincipal;

};

//End ws new pending trans.
