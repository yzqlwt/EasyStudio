//
// Created by yzqlwt on 2020/11/11.
//

#ifndef EASYSTUDIO_ZIPHELPER_H
#define EASYSTUDIO_ZIPHELPER_H


#include "bundle.h"
#include "Tools.h"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

namespace ZipHelper{
    class Zip{
    private:
        std::string _path;
        std::vector<std::map<std::string, std::string>> _files;
        void GetFiles(const std::string& dir){
            fs::path path(dir);
            if (!fs::exists(path))
            {
                return;
            }
            fs::directory_iterator end_iter;
            for (fs::directory_iterator iter(path); iter!=end_iter; ++iter)
            {
                if (fs::is_regular_file(iter->status()))
                {
                    auto filePath = iter->path().string();
                    if (filePath.find("DS_Store") == std::string::npos){
                        std::ifstream ifs(filePath, std::ios::binary);
                        std::stringstream buffer;
                        buffer << ifs.rdbuf();
                        std::string content(buffer.str());
                        Tools::replace_str(filePath, this->_path+"\\", "");
                        Tools::replace_str(filePath, this->_path+"/", "");
                        std::map<std::string, std::string> map;
                        map.insert(std::make_pair("name", filePath));
                        map.insert(std::make_pair("data", content));
                        this->_files.push_back(map);
                    }
                }

                if (fs::is_directory(iter->status()))
                {
                    GetFiles(iter->path().string());
                }
            }
        }
        void zip(const std::string& path){
            this->GetFiles(this->_path);
            bundle::archive package;
//            package.resize(this->_files.size());
            for (auto map : this->_files){
                bundle::file file;
                file["name"] = map["name"];
                file["data"] = map["data"];
                package.push_back(file);
            }
            std::string binary = package.zip(100); // compression level = 60 (of 100)
            //std::cout << "saving test:\n" << pak.toc() << std::endl;
            std::ofstream ofs(path, std::ios::out | std::ios::binary);
            ofs << binary;
            ofs.close();
        }
    public:
        Zip(const std::string& dirPath, const std::string& zipPath){
            this->_path = dirPath;
            this->zip(zipPath);
        }
    };
    class UnZip{
    private:
        void unzip(const std::string& zipPath, const std::string& dirPath){
            std::ifstream ifs(zipPath, std::ios::in | std::ios::binary);
            std::string binary;
            if (ifs)
            {
                std::stringstream stringStream;
                stringStream << ifs.rdbuf();
                binary = stringStream.str();
                ifs.close();
            }
            std::cout << dirPath << std::endl;
            bundle::archive package;
            package.zip(binary);

            for (auto file : package){
                auto filePath = dirPath + "\\" + file["name"];
                Tools::replace_str(filePath, "/", "\\");
                fs::path fsPath(filePath);
                if(!fs::exists(fsPath.parent_path())){
                    fs::create_directory(fsPath.parent_path());
                }
                std::ofstream fout(filePath, std::ofstream::binary);
                fout << file["data"] << std::endl;
                fout.close();
            }
        }
    public:
        UnZip(const std::string& zipPath, const std::string& dirPath){
            this->unzip(zipPath, dirPath);
        }
    };
}


#endif //EASYSTUDIO_ZIPHELPER_H
