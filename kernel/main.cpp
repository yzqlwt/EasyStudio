#include <iostream>
#include "websocket/WebSocketServer.h"
#include "utils/ZipHelper.h"
#include "utils/MiniZipHelper.h"
#include "utils/RequestHelper.h"
#include "utils/FlaExtension.h"
#include "utils/Tools.h"

int main() {
    auto t = chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << std::put_time(std::localtime(&t), "%Y-%m-%d %H.%M.%S")<<std::endl;
    Tools::init();
    Tools::CopyJSFL();
    std::thread threadObj([](){
        Tools::AddWatcher();
    });
    std::cout << "Server Listen: 8090" << std::endl;
    auto server = WebSocketServer();
    server.init(8090);
    threadObj.join();
    return 0;
}
