//
// Created by yzqlwt on 2021/1/7.
//

#ifndef EASYSTUDIO_WEBSOCKETSERVER_H
#define EASYSTUDIO_WEBSOCKETSERVER_H

#include "Handlers.h"
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <thread>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class WebSocketServer {
public:
    void init(unsigned short port);
    void do_session(tcp::socket socket);
    void stop();

private:
    Handlers handlers;
    net::io_context* m_ioc;
    tcp::acceptor* m_acceptor;
};


#endif //EASYSTUDIO_WEBSOCKETSERVER_H
