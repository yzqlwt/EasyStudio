//
// Created by yzqlwt on 2021/1/7.
//
#include "Handlers.h"
#include "../utils/DirHelper.h"
#include "../utils/RequestHelper.h"
#include "../utils/Tools.h"
#include "../utils/ZipHelper.h"
#include "../utils/PlistParser.h"
#include "../utils/RequestHelper.h"
#include "../utils/ResConfigParser.h"
#include "../CsdSerialize/FlatBuffersSerialize.h"
#include <regex>
#include <stdio.h>

typedef std::map<std::string, std::string> Property;
typedef std::pair<std::string, Property> Item;
typedef std::map<std::string, Property> Resource;

void Handlers::handle_upload(WS& ws, const std::string& message) {
        if (Tools::GetCCSPath() == ""){
            nlohmann::json res;
            res["id"] = "tips";
            res["content"] = "未配置CCS路径";
            res["type"] = "error";
            ws.write(net::buffer(res.dump()));
            return;
        }
        auto steps = std::vector<std::string>{
            "资源分析",
            "生成图集",
            "生成CSB",
            "打包&上传到后台",
            "完成",
        };
        this->send_steps(ws, steps, "资源分析");
        auto data = nlohmann::json::parse(message);
        int template_id = data["template_id"].get<int>();
        int skin_id = data["skin_id"].get<int>();
        nlohmann::json config;
        Resource resource;
        auto unzip_path = data["unzip_path"].get<std::string>();
        auto dir = DirHelper::GetDirByTemplateAndSkin(template_id, skin_id);
        auto resourcePath = DirHelper::GetResourceDir(dir);
        auto output_path = DirHelper::GetOutputDir(dir);
        auto temp_path = DirHelper::GetTempDir(dir);
        auto csd_dir = DirHelper::GetCsdDir(unzip_path);
        std::cout << "csd_dir:" <<csd_dir << std::endl;
        auto csd_files = DirHelper::GetFilesRecursive(csd_dir, ".csd");
        std::string::size_type idx = csd_dir.find("res/ui/image/");
        if ( idx != std::string::npos )
        {
            auto extend_csd = DirHelper::GetFilesRecursive(Tools::ReplaceStr(csd_dir, "res/ui/image/", "res/ui/games/"), ".csd");
            csd_files.insert(csd_files.end(), extend_csd.begin(), extend_csd.end());
        }
        auto png_files = DirHelper::GetFilesRecursive(resourcePath, ".png");
        auto ccs_png_files = DirHelper::GetFilesRecursive(csd_dir, ".png");
        //所有ccs图片添加到config
        for (auto file : ccs_png_files){
            Item item;
            png_files.push_back(file);
            auto new_path = Tools::ReplaceStr(file, csd_dir, "");
            Tools::replace_str(new_path, "\\", "/");
            item.first = new_path;
            Property property;
            property["Name"] = new_path;
            property["Extension"] = ".png";
            property["Tag"] = "plist";
            property["Md5"] = Tools::GetMd5(file);
            item.second = property;
            resource.insert(item);
        }
        DirHelper::cleanDir(temp_path);
        DirHelper::cleanDir(output_path);
        //复制所有图片到temp路径，以md5命名，以备合图
        for (auto file : png_files){
            auto md5 = Tools::GetMd5(file);
            auto new_file = temp_path+"/"+md5+".png";
            if (!std::filesystem::is_regular_file(new_file)){
                std::filesystem::copy_file(file, new_file);
            }
        }

        std::cout  << "config: " << config.dump() << std::endl;
        this->send_steps(ws, steps, "生成图集");
        auto plist_files = Tools::TexturePackage(temp_path, output_path);
        this->send_steps(ws, steps, "生成CSB");
        auto plist_files_name = std::vector<std::string>();
        for (auto file : plist_files){
            plist_files_name.push_back(std::filesystem::path(file).stem().string());
        }
        config["plist"] = plist_files_name;
        config["version"] = "3.0";
        for (auto file : csd_files){
            auto content = Tools::ReadFile(file);
            auto list = Tools::GetRegexStr(content, "Type=\"Normal\" Path=\"res.*png\" Plist=\"\"");
            for(auto elem : list){
                std::regex re("res/ui.*png");
                std::sregex_token_iterator iter(elem.begin(), elem.end(), re);
                std::sregex_token_iterator end;
                for(; iter != end; ++iter)
                {
                    auto part_path = iter->str();
                    auto path = DirHelper::GetCsdDir("")+part_path;
                    auto md5 = Tools::GetMd5(path);
                    auto plist_name = Tools::GetPlistName(plist_files, md5);
                    char str[1024];
                    sprintf(str, "Type=\"PlistSubImage\" Path=\"%s\" Plist=\"%s\"", (md5+".png").c_str(), (unzip_path+"/"+plist_name+".plist").c_str());
                    Tools::replace_str(content, elem, str);
                }
            }
            auto csb_path = resourcePath + "/" + std::filesystem::path(file).stem().string() + ".csb";
            FlatBuffersSerialize::getInstance()->serializeFlatBuffersWithXML(content, csb_path);
        }
        auto total_files = DirHelper::GetFiles(resourcePath, "*");
        for( auto file : total_files){
            std::filesystem::path path(file);
            Item item;
            item.first = path.filename().string();
            Property property;
            property["Name"] = path.filename().string();
            property["Extension"] = path.extension().string();
            if (path.extension() != ".png"){
                if (std::filesystem::is_regular_file(output_path+"/"+path.filename().string())){
                    std::filesystem::remove(output_path+"/"+path.filename().string());
                }
                std::filesystem::copy_file(file, output_path+"/"+path.filename().string());
            }
            property["Md5"] = Tools::GetMd5(path.string());
            item.second = property;
            resource.insert(item);
        }
        config["resource"] = resource;
        auto content = config.dump();
        Tools::WriteFile(output_path+"/ResConfig.json", content);
        auto zip_path = DirHelper::GetZipDir(dir) + "/ResConfig.zip";
        ZipHelper::Zip zip(output_path, zip_path);
        this->send_steps(ws, steps, "打包&上传到后台");
        RequestHelper::UploadFile(zip_path, std::to_string(skin_id));
        this->send_steps(ws, steps, "完成");
        this->handle_get_files(ws, message);
}

void Handlers::handle_open_folder(WS &, const std::string &message) {
    auto data = nlohmann::json::parse(message);
    int template_id = data["template_id"].get<int>();
    int skin_id = data["skin_id"].get<int>();
    auto type = data["type"].get<std::string>();
    auto path = DirHelper::GetDirByTemplateAndSkin(template_id, skin_id);
    auto dir = DirHelper::GetResourceDir(path);
    auto unzip_path = data["unzip_path"].get<std::string>();
    if (type.compare("images")==0){
        dir = DirHelper::GetCsdDir(unzip_path);
    }else if(type.compare("cache")==0){
        dir = DirHelper::GetMangoCacheDir(unzip_path);
    }

    std::string cmdStr = std::string("Win32") == BOOST_PLATFORM ? std::string("start  ")+dir : std::string("open  ")+dir;
    std::cout << cmdStr << std::endl;
    system(cmdStr.c_str());
}

void Handlers::handle_get_files(WS &ws, const std::string &message) {
    auto data = nlohmann::json::parse(message);
    int template_id = data["template_id"].get<int>();
    int skin_id = data["skin_id"].get<int>();
    auto path = DirHelper::GetDirByTemplateAndSkin(template_id, skin_id);
    auto output_dir = DirHelper::GetOutputDir(path);
    std::cout << output_dir << std::endl;
    auto files = DirHelper::GetFiles(output_dir);
    std::vector<Property> filesInfo;
    for (auto file : files){
        Tools::replace_str(file, "\\", "/");
        auto lstTime = fs::last_write_time(file);
        auto elapse = std::chrono::duration_cast<std::chrono::seconds>(std::filesystem::file_time_type::clock::now().time_since_epoch() - std::chrono::system_clock::now().time_since_epoch()).count();
        auto systemTime = std::chrono::duration_cast<std::chrono::seconds>(lstTime.time_since_epoch()).count() - elapse;

        auto path = std::filesystem::path(file);
        Property info;
        info["path"] = file;
        info["type"] = path.extension().string();
        info["name"] = path.filename().string();
        info["size"] = std::to_string(std::filesystem::file_size(file));
        info["time"] = std::to_string(systemTime);
        filesInfo.push_back(info);
    }
    nlohmann::json res;
    res["id"] = "files";
    res["result"] = 0;
    res["reason"] = "";
    res["files"] = filesInfo;
    std::cout << res.dump() << std::endl;
    ws.write(boost::asio::buffer(res.dump()));
}

void Handlers::handleResetCCS(WS &, const std::string &) {
    Tools::init();
}

void Handlers::handle_tiny_png(WS &, const std::string &) {

}

void Handlers::handle_download(WS &ws, const std::string &message) {
    ResConfigParser parser(ws, message);
    parser.Parse();
    this->handle_get_files(ws, message);
}

void Handlers::send_steps(WS &ws, std::vector<std::string> &total, std::string current) {
    nlohmann::json res;
    res["id"] = "steps";
    res["steps"] = total;
    res["current"] = current;
    ws.write(net::buffer(res.dump()));
}

void Handlers::handle_csd(WS &ws, const std::string &message) {

}

void Handlers::handle_open_file(WS &, const std::string &message) {
    auto data = nlohmann::json::parse(message);
    auto path = data["path"].get<std::string>();
    std::string cmdStr = std::string("Win32") == BOOST_PLATFORM ? std::string("start  ")+path : std::string("open  ")+path;
    std::cout << cmdStr << std::endl;
    system(cmdStr.c_str());
}
