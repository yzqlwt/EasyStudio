//
// Created by yzqlwt on 2020/12/29.
//

#ifndef EASYSTUDIO_RESCONFIGPARSER_H
#define EASYSTUDIO_RESCONFIGPARSER_H

#include <string>
#include "../websocket/WebSocketServer.h"

class ResConfigParser {

public:
    ResConfigParser(WS& ws, const std::string& message);
    void Parse();
    void Download();
private:
    WS& m_ws;
    std::string m_skin_path;
    std::string m_uri;
    std::string m_version;
    std::vector<std::string> m_files;
    void SendSteps(std::string current);
    void SendTips(std::string tip);
    void HandleResources();
    bool CheckResources();
    void ReadCCSFiles();
};


#endif //EASYSTUDIO_RESCONFIGPARSER_H
