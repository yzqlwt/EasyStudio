//
// Created by yzqlwt on 2021/1/6.
//

#ifndef EASYSTUDIO_HANDLERS_H
#define EASYSTUDIO_HANDLERS_H

//#include "StdAfx.h"
//#include "../utils/DirHelper.h"
//#include "../utils/RequestHelper.h"
//#include "../utils/Tools.h"
//#include "../utils/ZipHelper.h"

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <boost/regex.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


typedef websocket::stream<tcp::socket> WS;
typedef std::function<void(WS&, const std::string&)> MessageCallback;
typedef std::map<std::string, std::string> Property;
typedef std::pair<std::string, Property> Item;
typedef std::map<std::string, Property> Resource;

class Handlers {
public:

    void init(){
        this->m_callbacks["go_to_folder"] = std::bind(&Handlers::handle_open_folder, this, std::placeholders::_1,std::placeholders::_2);
        this->m_callbacks["open_file"] = std::bind(&Handlers::handle_open_file, this, std::placeholders::_1,std::placeholders::_2);
        this->m_callbacks["upload"] = std::bind(&Handlers::handle_upload, this, std::placeholders::_1,std::placeholders::_2);
        this->m_callbacks["download"] = std::bind(&Handlers::handle_download, this, std::placeholders::_1,std::placeholders::_2);
        this->m_callbacks["files"] = std::bind(&Handlers::handle_get_files, this, std::placeholders::_1,std::placeholders::_2);
        this->m_callbacks["csd"] = std::bind(&Handlers::handle_csd, this, std::placeholders::_1,std::placeholders::_2);
        this->m_callbacks["ccs"] = std::bind(&Handlers::handleResetCCS, this, std::placeholders::_1, std::placeholders::_2);
    }
    std::vector<Property> getFilesInfo(const std::string& path);
    Handlers(){
        init();
    }
private:
    std::map<std::string, MessageCallback> m_callbacks;

public:

    void handle(WS &ws, const std::string &message) {
         if (nlohmann::json::accept(message)){
             auto data = nlohmann::json::parse(message);
             auto msg_id = data["id"].get<std::string>();
             auto iter = this->m_callbacks.find(msg_id);
             std::cout << "msg_id: " << msg_id << std::endl;
             if(iter!=this->m_callbacks.end()){
                 MessageCallback& callback = iter->second;
                 callback(ws, message);
             }else{
                 std::cout << "未定义的id" << std::endl;
             }
         }else{
             std::cout << "非JSON数据" << std::endl;
         }

    }

    void handle_open_folder(WS&, const std::string& message);
    void handle_open_file(WS&, const std::string& message);

    void handle_get_files(WS&, const std::string& ) ;

    void handleResetCCS(WS&, const std::string& );

    void handle_tiny_png(WS&, const std::string& );

    void handle_download(WS&, const std::string& );

    void handle_upload(WS& ws, const std::string& message);
    void send_steps(WS& ws, std::vector<std::string> &total, std::string current);
    void handle_csd(WS& ws, const std::string& message);
};


#endif //EASYSTUDIO_HANDLERS_H
