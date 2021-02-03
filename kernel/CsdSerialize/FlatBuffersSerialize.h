//
// Created by yzqlwt on 2020/11/30.
//

#ifndef TESTCSD_FLATBUFFERSSERIALIZE_H
#define TESTCSD_FLATBUFFERSSERIALIZE_H

#include "flatbuffers/flatbuffers.h"
#include "CSParseBinary_generated.h"

#include <string>
#include <tinyxml2.h>


class FlatBuffersSerialize {
public:
    static FlatBuffersSerialize* getInstance();
    std::string serializeFlatBuffersWithXMLFile(const std::string& xmlPath,
                                                const std::string& flatbuffersFileName);

    std::string serializeFlatBuffersWithXML(const std::string &content,
                                                                  const std::string &flatbuffersFileName);
    flatbuffers::Offset<flatbuffers::NodeTree> createNodeTree(const tinyxml2::XMLElement* objectData,
                                                              std::string classType);
    std::string getGUIClassName(const std::string &name);

    FlatBuffersSerialize();

    void deleteFlatBufferBuilder();

public:
    std::vector<flatbuffers::Offset<flatbuffers::String>> _textures;
    std::vector<flatbuffers::Offset<flatbuffers::String>> _texturePngs;

private:
    flatbuffers::FlatBufferBuilder* _builder;
    flatbuffers::Offset<flatbuffers::CSParseBinary>* _csparsebinary;
    std::string _csdVersion;
};


#endif //TESTCSD_FLATBUFFERSSERIALIZE_H
