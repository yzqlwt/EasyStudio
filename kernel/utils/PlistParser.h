//
// Created by yzqlwt on 2020/11/6.
//

#ifndef EASYSTUDIO_PLISTPARSER_H
#define EASYSTUDIO_PLISTPARSER_H
#include "Tools.h"
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <regex>
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
namespace pt = boost::property_tree;

class Frame{
public:
    std::string key;
    int frame[4];
    int offset[2];
    bool rotated;
    int sourceColorRect[4];
    int sourceSize[2];

    Frame(std::map<std::string, std::string> map){
        this->key = map["key"];
        auto frames = Frame::GetNumbers(map["frame"]);
        auto offsets = Frame::GetNumbers(map["offset"]);
        auto sourceColorRects = Frame::GetNumbers(map["sourceColorRect"]);
        auto sourceSizes = Frame::GetNumbers(map["sourceSize"]);

        std::copy(frames.begin(), frames.end(), this->frame);
        std::copy(offsets.begin(), offsets.end(), this->offset);
        std::copy(sourceColorRects.begin(), sourceColorRects.end(), this->sourceColorRect);
        std::copy(sourceSizes.begin(), sourceSizes.end(), this->sourceSize);
        this->rotated = map["rotated"] == "true" ? true : false;
    }
public:
    cv::Mat convertToMat(cv::Mat& image){
        if (this->rotated){
            cv::Rect rect(this->frame[0], this->frame[1],this->frame[3],this->frame[2]);
            std::cout << this->key << std::endl;
            auto newImage = image(rect);
            cv::Mat temp,dst;
            cv::transpose(newImage, temp);
            cv::flip(temp,dst,0);
            return dst;
        }else{
            cv::Rect rect(this->frame[0], this->frame[1],this->frame[2],this->frame[3]);
            std::cout << this->key << std::endl;
            auto newImage = image(rect);
            return newImage;
        }
    }

    static std::vector<int> GetNumbers(std::string str){
        std::regex port("[0-9]+");
        std::sregex_token_iterator iter(str.begin(), str.end(), port);
        std::sregex_token_iterator end;
        std::vector<int> list;
        for(; iter != end; ++iter)
        {
            list.push_back(boost::lexical_cast<int>(iter->str()));
        }
        return list;
    }
};

class PlistParser {
private:
    std::string _imagePath;
    std::string _plistPath;
    std::vector<Frame> _frames;
public:
    void load(std::string imagePath, std::string plistPath){
        pt::ptree tree;
        std::ifstream wif(plistPath, std::ios::in);
        pt::read_xml(wif, tree);
        auto dict = tree.get_child("plist.dict.dict");
        std::vector<Frame> frames;
        for (auto pos = dict.begin(); pos != dict.end();)
        {
            auto key = pos->second.get_value<std::string>();
            pos++;
            std::vector<std::string> list;
            std::map<std::string, std::string> map;
            auto child = pos->second;
            for (auto index = child.begin(); index != child.end() ;) {
                auto childKey = index->second.get_value<std::string>();
                index++;
                if (childKey == "rotated"){
                    map[childKey]= index->first;
                }else{
                    map[childKey]= index->second.get_value<std::string>();
                }
                index++;
            }
            map["key"] = key;
            this->_frames.push_back(Frame(map));
            pos++;
        }
        this->_imagePath = std::move(imagePath);
        this->_plistPath = std::move(plistPath);
    }
    void save( const std::string& dirPath){
         cv::Mat origin = cv::imread(this->_imagePath, cv::IMREAD_UNCHANGED);
         auto parentDir = std::filesystem::path(dirPath);
         for (auto frame : this->_frames){
            auto region = frame.convertToMat(origin);
            auto path = parentDir / frame.key;
            std::cout << path << std::endl;
            cv::imwrite(path.string(), region);
         }
    }
};


#endif //EASYSTUDIO_PLISTPARSER_H
