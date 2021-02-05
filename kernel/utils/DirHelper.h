//
// Created by yzqlwt on 2020/11/5.
//

#ifndef EASYSTUDIO_DIRHELPER_H
#define EASYSTUDIO_DIRHELPER_H

#include "Tools.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include <boost/config.hpp>
#include <boost/format.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <regex>

#ifdef _WIN32
    #include <windows.h>
#elif __APPLE__

#endif

namespace fs = std::filesystem;


class DirHelper {

public:
    static std::string GetUser() {
        if (std::string("Win32") == BOOST_PLATFORM) {
            return getenv("USERNAME");
        } else {
            return getenv("USER");
        }
    }

    static std::string GetUserDir() {
        if (std::string("Win32") == BOOST_PLATFORM) {
            auto user = getenv("USERNAME");
            boost::format fmt("C:/Users/%s/AppData/Local");
            fmt % user;
            return fmt.str();
        } else if (std::string("Mac OS") == BOOST_PLATFORM) {
            auto user = getenv("USER");
            boost::format fmt("/Users/%s");
            fmt % user;
            return fmt.str();
        }
        return nullptr;
    }

    static std::string GetAppPath(){
        char szFilePath[1024]={0};
        GetModuleFileNameA(NULL, szFilePath, 1024);
        (strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串
        std::string path = szFilePath;
        std::cout << path << std::endl;
        return path;
    }

    static void cleanDir(const std::string& path){
        std::filesystem::remove_all(path);
        fs::create_directory(path);
    }

    static std::string GetCacheDir() {
        auto userDir = GetUserDir();
        auto path = fs::path(userDir);
        path = path / "EasyStudio";
        if (!fs::is_directory(path)) {
            fs::create_directories(path);
        }
        return path.string();
    }

    static std::string GetMangoCacheDir(const std::string& unzip_dir) {
        auto userDir = GetUserDir();
        auto path = fs::path(userDir);
        path = path / "mango/qmath-master_dev" / unzip_dir;
        if (!fs::is_directory(path)) {
            fs::create_directories(path);
        }
        return path.string();
    }

    static std::string GetDirByTemplateAndSkin(int template_id, int skin_id) {
        auto path = fs::path(GetCacheDir());
        path = path / "data" / std::to_string(template_id) / std::to_string(skin_id);
        if (!fs::is_directory(path)) {
            std::cout << path << std::endl;
            fs::create_directories(path);
        }
        return path.string();
    }

    static std::string GetResourceDir(std::string dir) {
        auto path = fs::path(dir);
        path = path / "Resource";
        if (!fs::is_directory(path)) {
            std::cout << path << std::endl;
            fs::create_directories(path);
        }
        return path.string();
    }

    static std::string GetOutputDir(std::string dir) {
        auto path = fs::path(dir);
        path = path / "Output";
        if (!fs::is_directory(path)) {
            std::cout << path << std::endl;
            fs::create_directories(path);
        }
        return path.string();
    }

    static std::string GetZipDir(std::string dir) {
        auto path = fs::path(dir);
        path = path / "Zip";
        if (!fs::is_directory(path)) {
            std::cout << path << std::endl;
            fs::create_directories(path);
        }
        return path.string();
    }

    static std::string GetTempDir(std::string dir) {
        auto path = fs::path(dir);
        path = path / "Temp";
        if (!fs::is_directory(path)) {
            std::cout << path << std::endl;
            fs::create_directories(path);
        }
        return path.string();
    }

    static std::string GetDownloadDir(std::string dir) {
        auto path = fs::path(dir);
        path = path / "Download";
        if (!fs::is_directory(path)) {
            std::cout << path << std::endl;
            fs::create_directories(path);
        }
        return path.string();
    }

    static std::string GetCCSDir(){

    }

    static std::string GetCsdDir(std::string unzip_path) {
        auto path = fs::path(Tools::GetCCSPath()).parent_path();
        path = path / "cocosstudio" / unzip_path;
        if (!fs::is_directory(path)) {
            std::cout << path << std::endl;
            fs::create_directories(path);
        }
        return path.string();
    }


    static std::vector<std::string> GetFiles(const std::string &path, const std::string &extension = "*") {
        fs::directory_iterator end;
        std::vector<std::string> list;
        if (fs::is_directory(path)){
            for (fs::directory_iterator pos(path); pos != end; pos++) {
                auto filePath = pos->path();
                if (extension.compare("*") == 0 || filePath.extension().compare(extension) == 0)
                    list.push_back(filePath.string());
            }
        }
        return list;
    }

    static std::vector<std::string> GetFilesRecursive(const std::string &path, const std::string &extension = "*") {
        std::vector<std::string> list;
        for (auto&fe : fs::recursive_directory_iterator(path))
        {
            auto filePath = fe.path();
            std::cout << filePath << std::endl;
            if (  std::filesystem::is_regular_file(filePath) && extension.compare("*") == 0 || filePath.extension().compare(extension) == 0)
                list.push_back(filePath.string());
        }

        return list;
    }
};

#endif //EASYSTUDIO_DIRHELPER_H
