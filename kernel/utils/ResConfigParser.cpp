//
// Created by yzqlwt on 2020/12/29.
//

#include "ResConfigParser.h"
#include "./DirHelper.h"
#include "./ZipHelper.h"
#include "RequestHelper.h"
#include "PlistParser.h"

ResConfigParser::ResConfigParser(WS& ws, const std::string& message): m_ws(ws)
{
    auto data = nlohmann::json::parse(message);
    auto steps = std::vector<std::string>{
            "下载",
            "解压",
            "拆分plist",
            "完成",
    };
    int template_id = data["template_id"].get<int>();
    int skin_id = data["skin_id"].get<int>();
    auto itemId = skin_id == 0 ? template_id : skin_id;
    auto itemType = skin_id == 0 ? "ActivityTemplate" : "Skin";
    auto attachments = RequestHelper::GetAttachments(std::to_string(itemId), itemType);
    auto uri = RequestHelper::GetResZipUri(attachments);
    if (uri.compare("") == 0){
        std::cout << "不存在ResConfig" << std::endl;
        return;
    }
    this->m_uri = uri;
    this->m_skin_path = DirHelper::GetDirByTemplateAndSkin(template_id, skin_id);
    this->m_version = "2.0";
//    auto resPath = DirHelper::GetResourceDir(path);
//    auto zipPath = DirHelper::GetZipDir(path);
//    auto output_path = DirHelper::GetOutputDir(path);
//    auto download_dir = DirHelper::GetDownloadDir(path);
//    auto temp_path = DirHelper::GetTempDir(path);
//    DirHelper::cleanDir(download_dir);
//    auto filePath = RequestHelper::DownloadZip(uri, zipPath);
//    ZipHelper::UnZip(filePath, download_dir);
//    auto files = DirHelper::GetFiles(download_dir, "*");
}

void ResConfigParser::Parse() {
    auto temp_path = DirHelper::GetTempDir(this->m_skin_path);
    DirHelper::cleanDir(temp_path);
    this->Download();
    auto ret = this->CheckResources();
    if (ret){
        this->HandleResources();
    }else {
        this->SendSteps("完成");
    }
}

void ResConfigParser::Download() {
    this->SendSteps("下载");
    auto zipPath = DirHelper::GetZipDir(this->m_skin_path);
    auto download_dir = DirHelper::GetDownloadDir(this->m_skin_path);
    DirHelper::cleanDir(download_dir);
    auto filePath = RequestHelper::DownloadZip(this->m_uri, zipPath);
    ZipHelper::UnZip(filePath, download_dir);
    this->m_files = DirHelper::GetFilesRecursive(download_dir, "*");
}


void ResConfigParser::SendSteps(std::string current) {
    nlohmann::json res;
    res["id"] = "steps";
    res["steps"] = std::vector<std::string>{"下载", "处理资源", "完成"};
    res["current"] = current;
    this->m_ws.write(net::buffer(res.dump()));
}

void ResConfigParser::HandleResources() {
    auto download_dir = DirHelper::GetDownloadDir(this->m_skin_path);
    auto temp_path = DirHelper::GetTempDir(this->m_skin_path);
    auto output_path = DirHelper::GetOutputDir(this->m_skin_path);
    this->SendSteps("处理资源");
    for(auto file : this->m_files){
        auto path = std::filesystem::path(file);
        auto extension = path.extension();
        auto filename = path.filename();
        if(extension == ".csb" || extension == ".mp3" || extension == ".swf" || extension == ".json"){
            auto new_path = this->m_skin_path + "/" + filename.string();
            if (std::filesystem::is_regular_file(new_path)){
                std::filesystem::remove(new_path);
            }
            std::filesystem::copy_file(file, new_path);
        }
        if (extension == ".plist"){
            auto name = path.stem();
            auto image_path = download_dir + "/" +name.string() + ".png";
            PlistParser parser;
            parser.load(image_path, file);
            parser.save(temp_path);
        }
        auto new_path = output_path + "/" + filename.string();
        if (std::filesystem::is_regular_file(new_path)){
            std::filesystem::remove(new_path);
        }
        std::filesystem::copy_file(file, new_path);
    }
    this->SendSteps("完成");
}


bool ResConfigParser::CheckResources() {
    auto download_dir = DirHelper::GetDownloadDir(this->m_skin_path);
    auto path = std::filesystem::path(download_dir);
    auto config_path = path / "ResConfig.json";
    if (std::filesystem::is_regular_file(config_path)){
        auto content = Tools::ReadFile(config_path.string());
        if (nlohmann::json::accept(content)){
            auto config = nlohmann::json::parse(content);
            if (config.contains("version")){
                return true;
            }
            this->SendTips("不兼容的版本，请使用EasyStudio2.0");
            return false;
        }
    }
    this->SendTips("Error: 不存在ResConfig.json");
    return false;
}

void ResConfigParser::SendTips(std::string tip) {
    nlohmann::json res;
    res["id"] = "tips";
    res["content"] = tip;
    this->m_ws.write(net::buffer(res.dump()));
}
