//
// Created by yzqlwt on 2020/11/28.
//
#include "Tools.h"
#include "RequestHelper.h"
#include "DirHelper.h"
#include "DirWatcher.h"
#include "FlaExtension.h"
#include <iostream>
#include <sstream>
#include <openssl/md5.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <fstream>
#include <regex>
#include <tchar.h>

namespace fs = std::filesystem;

static  std::string CCS = "";

std::string Tools::GetMd5(const std::string& file){
    if(std::filesystem::is_regular_file(file)){
        std::ifstream inf(file,std::ios::in|std::ios::binary) ;
        std::stringstream in ;
        in << inf.rdbuf() ;
        std::string data(in.str());
        unsigned char md[16];
        int i;
        char tmp[3]={'\0'},buf[33]={'\0'};
        MD5((unsigned char*)data.c_str(),(size_t)data.size(),md);
        for (i = 0; i < 16; i++){
            sprintf(tmp,"%2.2x",md[i]);
            strcat(buf,tmp);
        }
        return buf;
    }
    return nullptr;
}

void Tools::replace_str(std::string& str, const std::string& before, const std::string& after)
{
    for (std::string::size_type pos(0); pos != std::string::npos; pos += after.length())
    {
        pos = str.find(before, pos);
        if (pos != std::string::npos)
            str.replace(pos, before.length(), after);
        else
            break;
    }
}

std::string Tools::ReplaceStr(std::string& str, const std::string& before, const std::string& after)
{
    std::string s = std::string(str);
    for (std::string::size_type pos(0); pos != std::string::npos; pos += after.length())
    {
        pos = s.find(before, pos);
        if (pos != std::string::npos)
            s.replace(pos, before.length(), after);
        else
            break;
    }
    return s;
}

void Tools::SaveToken(std::string& token){
    nlohmann::json json = nlohmann::json::parse(token);
    auto time = json["expires_in"].get<long>();
    auto ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    );
    json["expires_in"] = ms.count() + time;
    auto cacheDir = DirHelper::GetCacheDir();
    auto path = fs::path(cacheDir);
    path = path / "token.json";
    std::ofstream ofs(path, std::ios::out);
    ofs << json.dump(4);
    ofs.close();
}

std::string Tools::RefreshToken(){
    auto cacheDir = DirHelper::GetCacheDir();
    auto path = fs::path(cacheDir);
    path = path / "token.json";
    if (fs::is_regular_file(path)){
        std::ifstream ifs(path, std::ios::in);
        std::stringstream ss;
        ss << ifs.rdbuf();
        ifs.close();
        nlohmann::json json = nlohmann::json::parse(ss.str());
        auto refreshToken = json["refresh_token"].get<std::string>();
        auto res = RequestHelper::RefreshToken(refreshToken);
        json = nlohmann::json::parse(res);
        Tools::SaveToken(res);
        return json["access_token"].get<std::string>();
    }
    return nullptr;
}

std::string Tools::GetToken(){
    auto cacheDir = DirHelper::GetCacheDir();
    auto path = fs::path(cacheDir);
    path = path / "token.json";
    if (fs::is_regular_file(path)){
        std::ifstream ifs(path, std::ios::in);
        std::stringstream ss;
        ss << ifs.rdbuf();
        ifs.close();
        nlohmann::json json = nlohmann::json::parse(ss.str());

        auto time = json["expires_in"].get<long>();
        auto ms = std::chrono::duration_cast< std::chrono::milliseconds >(
                std::chrono::system_clock::now().time_since_epoch()
        );
        std::cout << ms.count()  << "\t" << time << std::endl;
        if (time - ms.count() > 36000){
            return json["access_token"].get<std::string>();
        }else{
            return RefreshToken();
        }
    }
    return nullptr;
}

std::string Tools::ReadFile(std::string path) {
    std::cout << path << std::endl;
    std::ifstream ifs(std::filesystem::u8path(path), std::ios::in);
    std::stringstream ss;
    ss << ifs.rdbuf();
    ifs.close();
    return ss.str();
}

std::vector<std::string> Tools::GetRegexStr(const std::string &content, const std::string &regex) {
    std::regex re(regex);
    std::sregex_token_iterator iter(content.begin(), content.end(), re);
    std::sregex_token_iterator end;
    std::vector<std::string> list;
    for(; iter != end; ++iter)
    {
//        std::cout << iter->str() << std::endl;
        list.push_back(iter->str());
    }
    return list;
}

std::vector<std::string> Tools::TexturePackage(const std::string &res_path, const std::string &output_path) {
    char str[2048];
    auto targetPath = output_path;
    auto appPath = DirHelper::GetAppPath();
    std::string cmdStr = std::string("Win32") == BOOST_PLATFORM ? std::string(appPath+"/TexturePacker/TexturePacker.exe ") : std::string("TexturePacker ");
    sprintf(str, "%s %s --sheet %s/__frame_{n1}.png --data %s/__frame_{n1}.plist --allow-free-size --no-trim --max-size 2048 --format cocos2d --multipack --extrude 4",
            cmdStr.c_str(), res_path.c_str(), targetPath.c_str(), targetPath.c_str());
    system(str);
    return DirHelper::GetFiles(targetPath, ".plist");
}

std::string Tools::GetPlistName(const std::vector<std::string> &files, const std::string &md5) {
    for (int i = 0; i < files.size(); ++i) {
        auto file = files[i];
        auto content = Tools::ReadFile(file);
        std::string::size_type idx = content.find(md5);
        if ( idx != std::string::npos )
        {
            return std::filesystem::path(file).stem().string();
        }
    }
    return std::string();
}

void Tools::WriteFile(const std::string &path, const std::string &content) {
    try {
        std::ofstream ofs(path, std::ios::out);
        ofs << content;
        ofs.close();
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
    }
}

void Tools::CopyJSFL() {
    auto list = DirHelper::GetFiles(DirHelper::GetAppPath(), ".jsfl");
    if (BOOST_PLATFORM == std::string("Win32")){
        auto user_name = DirHelper::GetUser();
        auto target_dir = std::string("C:\\Users\\")+user_name+"\\AppData\\Local\\Adobe\\Flash CS6\\zh_CN\\Configuration\\Commands";
        for(auto file : list){
            std::cout << "读取文件: "<<file << std::endl;
            auto file_name = std::filesystem::path(file).filename().string();
            auto new_path = target_dir + "\\" + file_name;
            std::ifstream ifs(file, std::ios::in);
            std::stringstream ss;
            ss << ifs.rdbuf();
            ifs.close();
            auto content = ss.str();
            Tools::replace_str(content, "$username$", user_name);
            Tools::WriteFile(new_path, content);
        }
    }

}

void Tools::AddWatcher() {
    DirWatcher watcher;
    std::cout << "Watch:" << DirHelper::GetCacheDir() << std::endl;
    watcher.setDir(DirHelper::GetCacheDir());
    watcher.setCallback([](std::string type, std::string filename) {
        std::cout << filename << std::endl;
        if (filename == "fla.txt"){
            std::filesystem::path path(DirHelper::GetCacheDir()+"/fla.txt");
            if (std::filesystem::is_regular_file(path)){
                auto content = Tools::ReadFile(path.string());
                std::string ret;
                char ch;
                int i, ii;
                for (i=0; i<content.length(); i++) {
                    if (int(content[i])==37) {
                        sscanf(content.substr(i+1,2).c_str(), "%x", &ii);
                        ch=static_cast<char>(ii);
                        ret+=ch;
                        i=i+2;
                    } else {
                        ret+=content[i];
                    }
                }
                Tools::replace_str(ret, "file:///", "");
                Tools::replace_str(ret, "|", ":");
                FlaExtension fla(ret);
                std::filesystem::remove(path);
            }
        }
    });
    watcher.start();
}

void Tools::init() {
    auto cacheDir = DirHelper::GetCacheDir();
    auto path = fs::path(cacheDir);
    path = path / "config.json";
    auto content = Tools::ReadFile(path.string());
    if (nlohmann::json::accept(content)){
        nlohmann::json json = nlohmann::json::parse(content);
        auto ccs_path = json["CCSPath"].get<std::string>();
        CCS = ccs_path;
    }
}

std::string Tools::GetCCSPath() {
    return CCS;
}
