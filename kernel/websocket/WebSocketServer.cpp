//
// Created by yzqlwt on 2021/1/7.
//

#include "WebSocketServer.h"
#include <nlohmann/json.hpp>

void WebSocketServer::do_session(tcp::socket socket)
{
    try
    {
        // Construct the stream by moving in the socket
        websocket::stream<tcp::socket> ws{std::move(socket)};

        // Set a decorator to change the Server of the handshake
        ws.set_option(websocket::stream_base::decorator(
                [](websocket::response_type& res)
                {
                    res.set(http::field::server,
                            std::string(BOOST_BEAST_VERSION_STRING) +
                            " websocket-server-sync");
                }));

        // Accept the websocket handshake
        ws.accept();

        for(;;)
        {
            // This buffer will hold the incoming message
            beast::flat_buffer buffer;

            // Read a message
            ws.read(buffer);

            // Echo the message back
            ws.text(ws.got_text());
            this->handlers.handle(ws, beast::buffers_to_string(buffer.data()));
        }
    }
    catch(beast::system_error const& se)
    {
        // This indicates that the session was closed
        if(se.code() != websocket::error::closed)
            std::cerr << "Error: " << se.code().message() << std::endl;
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void WebSocketServer::init(unsigned short port){
    auto const address = net::ip::make_address("127.0.0.1");

    // The io_context is required for all I/O
    net::io_context ioc{1};

    // The acceptor receives incoming connections
    tcp::acceptor acceptor{ioc, {address, port}};
    for(;;)
    {
        // This will receive the new connection
        tcp::socket socket{ioc};

        // Block until we get a connection
        acceptor.accept(socket);

        // Launch the session, transferring ownership of the socket
        std::thread(
                std::bind(&WebSocketServer::do_session, this, std::placeholders::_1),
                std::move(socket)).detach();
    }
}