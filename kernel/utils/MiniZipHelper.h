//
// Created by yzqlwt on 2021/1/8.
//

#ifndef EASYSTUDIO_MINIZIPHELPER_H
#define EASYSTUDIO_MINIZIPHELPER_H
#include "Tools.h"
#include <iostream>
#include <zlib.h>
#include <minizip/zip.h>
#include <minizip/unzip.h>
#include <map>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
using namespace  std;
namespace fs = std::filesystem;


class MiniZipHelper {
public:
    MiniZipHelper(string src, string dest){
        this->_path = src;
        this->GetFiles(src);
        Minizip(src,dest);
    }
private:
    std::string _path;
    std::vector<std::string> _files;
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
                    this->_files.push_back(filePath);
                }
            }

            if (fs::is_directory(iter->status()))
            {
                GetFiles(iter->path().string());
            }
        }
    }
    //压缩码流
    int WriteInZipFile(zipFile zFile,const string& file)
    {
        fstream f(file.c_str(),std::ios::binary | std::ios::in);
        f.seekg(0, std::ios::end);
        long size = f.tellg();
        f.seekg(0, std::ios::beg);
        if ( size <= 0 )
        {
            return zipWriteInFileInZip(zFile,NULL,0);
        }
        char* buf = new char[size];
        f.read(buf,size);
        int ret = zipWriteInFileInZip(zFile,buf,size);
        delete[] buf;
        return ret;
    }
    //minizip 压缩
    int Minizip(string src, string dest) {
        if (src.find_last_of("/") == src.length() - 1)
            src = src.substr(0,src.length()-1);

        if (fs::is_regular_file(src)) {
            zipFile zFile = zipOpen(dest.c_str(),APPEND_STATUS_CREATE);
            if (zFile == NULL) {
                cout<<"openfile failed"<<endl;
                return -1;
            }
            zip_fileinfo zFileInfo = { 0 };
            int ret = zipOpenNewFileInZip(zFile, src.c_str(), &zFileInfo, NULL, 0, NULL, 0, NULL, 0, 0);
            if (ret != ZIP_OK) {
                cout<<"openfile in zip failed"<<endl;
                zipClose(zFile, NULL);
                return -1;
            }
            ret = WriteInZipFile(zFile,src);
            if (ret != ZIP_OK) {
                cout<<"write in zip failed"<<endl;
                zipClose(zFile,NULL);
                return -1;
            }
            zipClose(zFile, NULL);
            cout<<"zip ok"<<endl;
        }
        else if (fs::is_directory(src)) {
            size_t pos = src.find_last_of("/");
            string dirName = src.substr(pos + 1);
            string dirPrefix = src.substr(0, pos);

            zipFile zFile = zipOpen(dest.c_str(), APPEND_STATUS_CREATE);
            if (zFile == NULL) {
                cout<<"openfile failed"<<endl;
                return -1;
            }

            for (auto file : this->_files) {
                zip_fileinfo zFileInfo = { 0 };
                int ret = zipOpenNewFileInZip(zFile,file.c_str(),&zFileInfo,NULL,0,NULL,0,NULL,0,0);
                if (ret != ZIP_OK) {
                    cout<<"openfile in zip failed"<<endl;
                    zipClose(zFile,NULL);
                    return -1;
                }
                ret = WriteInZipFile(zFile, dirPrefix + "/" + file);
                if (ret != ZIP_OK) {
                    cout<<"write in zip failed"<<endl;
                    zipClose(zFile,NULL);
                    return -1;
                }
            }

            zipClose(zFile,NULL);
            cout<<"zip ok"<<endl;
        }
        return 0;
    }
};






//int main(int argc, char** argv) {
//    Minizip("/home/ubuntu/test", "/home/ubuntu/test.zip");
//    return 0
//}

#endif //EASYSTUDIO_MINIZIPHELPER_H
