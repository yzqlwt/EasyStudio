//
// Created by yzqlwt on 2021/1/23.
//

#include "FlaExtension.h"
#include "Tools.h"
#include "DirHelper.h"


void FlaExtension::ReadTemplateXml(){
    std::filesystem::path path(DirHelper::GetAppPath());
    document.LoadFile((path.string()+"/Document.xml").c_str());
    Children = document.FirstChildElement("GameFile")->FirstChildElement("Content")->FirstChildElement("Content")->FirstChildElement("ObjectData")->FirstChildElement("Children");
    ImageView = queryNodeByName(Children, "ImageViewTemplate");
    Node = queryNodeByName(Children, "NodeTemplate");
    Button = queryNodeByName(Children, "ButtonTemplate");
}

void FlaExtension::ReadConfig(std::string path) {
    auto content = Tools::ReadFile(path);
    std::cout << path << std::endl;
    if (nlohmann::json::accept(content)){
        nlohmann::json json = nlohmann::json::parse(content);
        this->path = json["path"].get<std::string>();
        symbols = json["symbols"].get<std::vector<std::map<std::string, std::string>>>();
        this->Write(std::filesystem::path(std::filesystem::u8path(path)).parent_path().string());
    }
    else{
        std::cerr << "Error: 非json文件 " << path <<std::endl;
    }
}

void FlaExtension::Write(std::string dir_path){
    std::filesystem::path tmp(this->path);
    std::cout << tmp.filename() << std::endl;
    auto csd_path = dir_path + "/" +tmp.filename().string() +".csd";
    for (auto item : symbols){
        auto name = item["name"];
        auto image = item["image"];
        auto type = item["type"];
        auto x = item["x"];
        auto y = item["y"];
        auto file_path = this->path +"/"+ image;
        if (type.compare("ImageView") == 0){
            tinyxml2::XMLElement* imageview = (tinyxml2::XMLElement*)deepCopy(this->ImageView, &document);
            imageview->SetAttribute("Name", name.c_str());
            tinyxml2::XMLElement* FileData = imageview->FirstChildElement("FileData");
            FileData->SetAttribute("Path", file_path.c_str());
            tinyxml2::XMLElement* Position = imageview->FirstChildElement("Position");
            Position->SetAttribute("X", x.c_str());
            Position->SetAttribute("Y", y.c_str());
            tinyxml2::XMLElement* Size = imageview->FirstChildElement("Size");
            cv::Mat mat = cv::imread(dir_path+"/"+image, cv::IMREAD_UNCHANGED);
            Size->SetAttribute("X", mat.cols);
            Size->SetAttribute("Y", mat.rows);
            Children->LinkEndChild(imageview);
        }else if(type.compare("Node") == 0){
            tinyxml2::XMLElement* node = (tinyxml2::XMLElement*)deepCopy(this->Node, &document);
            node->SetAttribute("Name", name.c_str());
            tinyxml2::XMLElement* Position = node->FirstChildElement("Position");
            Position->SetAttribute("X", x.c_str());
            Position->SetAttribute("Y", y.c_str());
            Children->LinkEndChild(node);
        }else if(type.compare("Button") == 0){
            tinyxml2::XMLElement* button = (tinyxml2::XMLElement*)deepCopy(this->Button, &document);
            button->SetAttribute("Name", name.c_str());
            tinyxml2::XMLElement* NormalFileData = button->FirstChildElement("NormalFileData");
            NormalFileData->SetAttribute("Path", file_path.c_str());
            tinyxml2::XMLElement* Position = button->FirstChildElement("Position");
            Position->SetAttribute("X", x.c_str());
            Position->SetAttribute("Y", y.c_str());
            cv::Mat mat = cv::imread(dir_path+"/"+image, cv::IMREAD_UNCHANGED);
            tinyxml2::XMLElement* Size = button->FirstChildElement("Size");
            Size->SetAttribute("X", mat.cols);
            Size->SetAttribute("Y", mat.rows);
            Children->LinkEndChild(button);
        }
    }
    Children->DeleteChild(this->ImageView);
    Children->DeleteChild(this->Node);
    Children->DeleteChild(this->Button);
    document.SaveFile(csd_path.c_str());
    std::cout << "csd: " << csd_path << std::endl;
}