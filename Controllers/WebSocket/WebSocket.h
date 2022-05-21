//
// Created by Alberto Romero on 19/4/22.
//

#pragma once

#include <iostream>
#include <string>

#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>

#include "../../Utils/Common/PolyHex.h"
#include "../../lib/BigDecimal/BigDecimal.h"

typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

class WebSocket {





    public:
        explicit WebSocket(std::string&& uri);

        bool Prepare();
        bool Start();


    client c;
    std::string uri;

    static std::string onCloseFallbackUri;

    static void onOpen(client* c, websocketpp::connection_hdl hdl);
    static void onFail(client* c, websocketpp::connection_hdl hdl);
    static void onClose(client* c, websocketpp::connection_hdl hdl);

    /*
    static void onMessage(client* c, websocketpp::connection_hdl hdl, message_ptr msg);
    */

};