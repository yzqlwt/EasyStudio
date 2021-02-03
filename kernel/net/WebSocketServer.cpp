////
//// Created by yzqlwt on 2020/12/28.
////
//
//#include "WebSocketServer.h"
//#include "../utils/DirHelper.h"
//#include "../utils/RequestHelper.h"
//#include "../utils/Tools.h"
//#include "../utils/ZipHelper.h"
//#include <iostream>
//#include <functional>
//#include <filesystem>
//#include <stdlib.h>
//
//
//typedef std::map<std::string, std::string> Property;
//
//void WebSocketServer::init(int port){
//    this->m_server = new Server();
//    try {
//        // Set logging settings
//        this->m_server->set_access_channels(websocketpp::log::alevel::all);
//        this->m_server->clear_access_channels(websocketpp::log::alevel::frame_payload);
//
//        // Initialize Asio
//        this->m_server->init_asio();
//
//        // Register our message handler
//        this->m_server->set_message_handler(std::bind(&WebSocketServer::on_message, this, this->m_server,std::placeholders::_1, std::placeholders::_2));
//
//        // Listen on port 9002
//        this->m_server->listen(port);
//
//        // Start the Server accept loop
//        this->m_server->start_accept();
//
//        // Start the ASIO io_service run loop
//        std::cout << "Server is running. Port:" << port << std::endl;
//        this->m_server->run();
//    } catch (websocketpp::exception const & e) {
//        std::cout << e.what() << std::endl;
//    } catch (std::exception e) {
//        std::cout << "other exception" << e.what()<< std::endl;
//    }
//}
//
//void WebSocketServer::on_message(Server *s, websocketpp::connection_hdl hdl, message_ptr msg) {
//    std::cout << "[message]: " << msg->get_payload() << std::endl;
//    if (nlohmann::json::accept(msg->get_payload())) {
//        nlohmann::json message = nlohmann::json::parse(msg->get_payload());
//        auto msg_id = message["id"].get<std::string>();
//        auto iter = this->m_callbacks.find(msg_id);
//        std::cout << "msg_id: " << msg_id << std::endl;
//        if(iter!=this->m_callbacks.end()){
//            MessageCallback& callback = iter->second;
//            callback(&hdl, message);
//        }else{
//            std::cout << "未定义的id" << std::endl;
//        }
//    }else{
//        std::cout << "非json数据: " << msg->get_payload() << std::endl;
//    }
//}
//
//void WebSocketServer::initCallbacks() {
//    this->m_callbacks["go_to_folder"] = std::bind(&WebSocketServer::handle_openFolder, this, std::placeholders::_1,std::placeholders::_2);
//    this->m_callbacks["open_delete_rename_file"] = std::bind(&WebSocketServer::handle_operateFile, this, std::placeholders::_1,std::placeholders::_2);
//    this->m_callbacks["tiny_png"] = std::bind(&WebSocketServer::handle_tinypng, this, std::placeholders::_1,std::placeholders::_2);
//    this->m_callbacks["upload"] = std::bind(&WebSocketServer::handle_upload, this, std::placeholders::_1,std::placeholders::_2);
//    this->m_callbacks["download"] = std::bind(&WebSocketServer::handle_loadFromNet, this, std::placeholders::_1,std::placeholders::_2);
//    this->m_callbacks["files"] = std::bind(&WebSocketServer::handle_getFiles, this, std::placeholders::_1,std::placeholders::_2);
//    this->m_callbacks["exit"] = std::bind(&WebSocketServer::handle_exit, this, std::placeholders::_1,std::placeholders::_2);
//}
//
//
//void WebSocketServer::handle_openFolder(websocketpp::connection_hdl * conn, nlohmann::json & message) {
//    int template_id = message["template_id"].get<int>();
//    int skin_id = message["skin_id"].get<int>();
//    auto path = DirHelper::GetDirByTemplateAndSkin(template_id, skin_id);
//    auto resourcePath = DirHelper::GetResourceDir(path);
//    auto cmdStr = std::string("start  ")+resourcePath;
//    std::cout << cmdStr << std::endl;
//    system(cmdStr.c_str());
//}
//
//void WebSocketServer::handle_operateFile(websocketpp::connection_hdl * conn, nlohmann::json & message) {
//    auto operation = message["operation"].get<std::string>();
//    auto path = message["path"].get<std::string>();
//    if (operation.compare("open")==0){
//        auto cmdStr = std::string("start  ")+path;
//        std::cout << cmdStr << std::endl;
//        system(cmdStr.c_str());
//    }else if (operation.compare("delete")==0){
//        std::filesystem::remove(path);
//        this->handle_getFiles(conn, message);
//    }else if (operation.compare("rename") == 0){
//        auto new_name = message["new_name"].get<std::string>();
//        std::filesystem::path oldFilePath(path);
//        auto dir = oldFilePath.parent_path();
//        std::filesystem::path newFilePath(dir.string() + "\\" + new_name + oldFilePath.extension().string());
//        std::filesystem::rename(oldFilePath, newFilePath);
//    }
//}
//
//void WebSocketServer::handle_getFiles(websocketpp::connection_hdl * conn, nlohmann::json & message) {
//    std::cout << "获取所有文件" << std::endl;
//    int template_id = message["template_id"].get<int>();
//    int skin_id = message["skin_id"].get<int>();
//    auto path = DirHelper::GetDirByTemplateAndSkin(template_id, skin_id);
//    auto resourcePath = DirHelper::GetResourceDir(path);
//    auto files = DirHelper::GetFiles(resourcePath);
//    std::vector<Property> filesInfo;
//    for (auto file : files){
//        Tools::replace_str(file, "\\", "/");
//        auto lstTime = fs::last_write_time(file);
//        auto elapse = std::chrono::duration_cast<std::chrono::seconds>(std::filesystem::file_time_type::clock::now().time_since_epoch() - std::chrono::system_clock::now().time_since_epoch()).count();
//        auto systemTime = std::chrono::duration_cast<std::chrono::seconds>(lstTime.time_since_epoch()).count() - elapse;
//
//        auto path = std::filesystem::path(file);
//        Property info;
//        info["path"] = file;
//        info["type"] = path.extension().string();
//        info["name"] = path.filename().string();
//        info["size"] = std::to_string(std::filesystem::file_size(file));
//        info["time"] = std::to_string(systemTime);
//        filesInfo.push_back(info);
//    }
//    nlohmann::json res;
//    res["id"] = "files";
//    res["result"] = 0;
//    res["reason"] = "";
//    res["files"] = filesInfo;
//    std::cout << res.dump() << std::endl;
//    this->m_server->send(*conn, res.dump(), websocketpp::frame::opcode::text);
//}
//
//void WebSocketServer::handle_exit(websocketpp::connection_hdl *conn, nlohmann::json &message) {
//    nlohmann::json res;
//    res["id"] = message["id"].get<std::string>();
//    res["result"] = 0;
//    res["reason"] = "";
//    this->m_server->send(*conn, res.dump(), websocketpp::frame::opcode::text);
//    this->m_server->stop();
//}
//
//void WebSocketServer::handle_tinypng(websocketpp::connection_hdl *, nlohmann::json &) {
//
//}
//
//void WebSocketServer::handle_loadFromNet(websocketpp::connection_hdl *conn, nlohmann::json & message) {
//    //下载文件-发送step
//    //解析文件-发送step
//    //完成-发送step
//    //发送文件列表-发送files
//    std::cout << "下载" << std::endl;
//    std::vector<std::string> steps = {"下载中", "解析中", "完成"};
//    std::string current_step = "下载中";
//    this->send_steps(conn, steps, current_step);
//    int template_id = message["template_id"].get<int>();
//    int skin_id = message["skin_id"].get<int>();
//    auto itemId = skin_id == 0 ? template_id : skin_id;
//    auto itemType = skin_id == 0 ? "ActivityTemplate" : "Skin";
//    auto attachments = RequestHelper::GetAttachments(std::to_string(itemId), itemType);
//    auto uri = RequestHelper::GetResZipUri(attachments);
//    if (uri.compare("") == 0){
//        std::cout << "不存在ResConfig" << std::endl;
//        return;
//    }
//    auto path = DirHelper::GetDirByTemplateAndSkin(template_id, skin_id);
//    auto resPath = DirHelper::GetResourceDir(path);
//    auto zipPath = DirHelper::GetZipDir(path);
//    auto tempPath = DirHelper::GetTempDir(path);
//    auto filePath = RequestHelper::DownloadZip(uri, zipPath);
//    current_step = "解析中";
//    this->send_steps(conn, steps, current_step);
//    ZipHelper::UnZip(filePath, tempPath);
////    current_step = "完成";
////    this->send_steps(conn, steps, current_step);
//}
//
//void WebSocketServer::handle_upload(websocketpp::connection_hdl *, nlohmann::json &) {
//
//}
//
//void WebSocketServer::send_steps(websocketpp::connection_hdl *conn, std::vector<std::string> &total, std::string &current) {
//    nlohmann::json res;
//    res["id"] = "steps";
//    res["steps"] = total;
//    res["current"] = current;
//    this->m_server->send(*conn, res.dump(), websocketpp::frame::opcode::text);
//};
//
