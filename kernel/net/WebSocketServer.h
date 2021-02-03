////
//// Created by yzqlwt on 2020/12/28.
////
//
//#ifndef EASYSTUDIO_WEBSOCKETSERVER_H
//#define EASYSTUDIO_WEBSOCKETSERVER_H
//
//#include <map>
//#include <websocketpp/config/asio_no_tls.hpp>
//#include <websocketpp/server.hpp>
//#include <nlohmann/json.hpp>
//
//typedef websocketpp::server<websocketpp::config::asio> Server;
//typedef std::function<void(websocketpp::connection_hdl*, nlohmann::json& )> MessageCallback;
//typedef Server::message_ptr message_ptr;
//
//class WebSocketServer
//{
//private:
//    WebSocketServer() {
//        this->initCallbacks();
//    };
//    ~WebSocketServer() { };
//    WebSocketServer(const WebSocketServer&);
//    WebSocketServer& operator=(const WebSocketServer&);
//    void initCallbacks();
//    Server* m_server;
//    std::map<std::string, MessageCallback> m_callbacks;
//
//private:
//    void handle_openFolder(websocketpp::connection_hdl*, nlohmann::json& );
//    void handle_operateFile(websocketpp::connection_hdl*, nlohmann::json& );
//    void handle_getFiles(websocketpp::connection_hdl*, nlohmann::json& );
//    void handle_exit(websocketpp::connection_hdl*, nlohmann::json& );
//    void handle_tinypng(websocketpp::connection_hdl*, nlohmann::json& );
//    void handle_loadFromNet(websocketpp::connection_hdl*, nlohmann::json& );
//    void handle_upload(websocketpp::connection_hdl*, nlohmann::json& );
//    void send_steps(websocketpp::connection_hdl*, std::vector<std::string>&, std::string&);
//public:
//    static WebSocketServer& Instance()
//    {
//        static WebSocketServer instance;
//        return instance;
//    }
//    void init(int port);
//
//    void on_message(Server* s, websocketpp::connection_hdl hdl, message_ptr msg);
//};
//
//
//#endif //EASYSTUDIO_WEBSOCKETSERVER_H
