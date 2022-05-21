//
// Created by Alberto Romero on 19/4/22.
//

#include "WebSocket.h"

typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

std::string WebSocket::onCloseFallbackUri;

WebSocket::WebSocket(std::string&& uri) {
    this->uri = uri;

    WebSocket::onCloseFallbackUri = uri;
    this->Prepare();



    // Config

}

void WebSocket::onOpen (client* c, websocketpp::connection_hdl hdl) {

    std::cout << "Websocket opened" << std::endl << std::endl;

    std::string msg = "{\"id\": 1, \"method\": \"eth_subscribe\", \"params\": [\"newPendingTransactions\"]}";
    c->send(hdl,msg,websocketpp::frame::opcode::text);
    c->get_alog().write(websocketpp::log::alevel::app, "Sent eth_subscribe Message: "+msg);

}

void WebSocket::onFail(websocketpp::client<websocketpp::config::asio_client>* c, websocketpp::connection_hdl hdl) {
    std::cout << "Something has failed on websocket" << std::endl;
}

void WebSocket::onClose(client* c, websocketpp::connection_hdl hdl) {

    std::cout << "Websocket closed. Reconnecting to: " << WebSocket::onCloseFallbackUri << std::endl;

    // websocketpp::client<websocketpp::config::asio_client> is very limited and explicitly does not allow to copy instances, so:


    try {
        websocketpp::lib::error_code ec;
        client::connection_ptr con = c->get_connection(WebSocket::onCloseFallbackUri, ec);
        c->connect(con);

    } catch (const std::exception & e) {
        std::cout << e.what() << std::endl;
    } catch (websocketpp::lib::error_code& e) {
        std::cout << e.message() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }

}

/*
void WebSocket::onMessage(client* c, websocketpp::connection_hdl hdl, message_ptr msg) {
    c->get_alog().write(websocketpp::log::alevel::app, "Received Reply: "+msg->get_payload());
   // c->close(hdl,websocketpp::close::status::normal,"");
}
 */

// ws://15.236.212.149:8546
bool WebSocket::Prepare() {

    std::cout << "Im on Websocket::Start, client memory address: " << &c << std::endl;

    try {
        // set logging policy if needed
        WebSocket::c.clear_access_channels(websocketpp::log::alevel::frame_header);
        WebSocket::c.clear_access_channels(websocketpp::log::alevel::frame_payload);
        //c.set_error_channels(websocketpp::log::elevel::none);

        // Initialize ASIO
        c.init_asio();


        // Register our handlers
        c.set_open_handler(bind(&onOpen, &c, ::_1));


        c.set_fail_handler(bind(&onFail,&c,::_1));
        //c.set_message_handler(bind(&onMessage,&c,::_1,::_2));
        c.set_close_handler(bind(&onClose,&c,::_1));


    } catch (const std::exception & e) {
        std::cout << e.what() << std::endl;
    } catch (websocketpp::lib::error_code e) {
        std::cout << e.message() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }

    return true;
}


bool WebSocket::Start() {

    try {
        // Create a connection to the given URI and queue it for connection once
        // the event loop starts
        websocketpp::lib::error_code ec;
        client::connection_ptr con = c.get_connection(this->uri, ec);
        c.connect(con);

        // Start the ASIO io_service run loop
        c.run();

    } catch (const std::exception & e) {
        std::cout << e.what() << std::endl;
    } catch (websocketpp::lib::error_code e) {
        std::cout << e.message() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }

    return true;
}


