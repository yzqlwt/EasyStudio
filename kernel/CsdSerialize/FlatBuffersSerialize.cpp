//
// Created by yzqlwt on 2020/11/30.
//

#include "FlatBuffersSerialize.h"
#include "NodeReaderProtocol.h"
#include "ObjectFactory.h"
#include "reader/ProjectNodeReader.h"
#include "reader/ComAudioReader.h"
#include "reader/NodeReader.h"
#include "reader/WidgetReader.h"
#include "reader/ImageViewReader.h"
#include "reader/SingleNodeReader.h"
#include "reader/LayoutReader.h"
#include "reader/TextReader.h"
#include "reader/ButtonReader.h"
#include "reader/SpriteReader.h"
#include <new>
#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>

using namespace flatbuffers;


static FlatBuffersSerialize *_instanceFlatBuffersSerialize = nullptr;

#define CREATE_CLASS_NODE_READER_INFO(className) \
ObjectFactory::TInfo(#className, &className::getInstance) \

FlatBuffersSerialize *FlatBuffersSerialize::getInstance() {
    if (!_instanceFlatBuffersSerialize) {
        _instanceFlatBuffersSerialize = new(std::nothrow) FlatBuffersSerialize();
    }

    return _instanceFlatBuffersSerialize;
}

std::string FlatBuffersSerialize::serializeFlatBuffersWithXMLFile(const std::string &xmlPath,
                                                                  const std::string &flatbuffersFileName) {
    if (!std::filesystem::is_regular_file(xmlPath)){
        return ".csd file does not exist.";
    }
    std::ifstream ifs(xmlPath, std::ios::in);
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string content(ss.str());
    ifs.close();
    FlatBuffersSerialize::serializeFlatBuffersWithXML(content, flatbuffersFileName);
}

std::string FlatBuffersSerialize::serializeFlatBuffersWithXML(const std::string &content,
                                                                  const std::string &flatbuffersFileName) {
    // xml parse
    tinyxml2::XMLDocument* document = new (std::nothrow) tinyxml2::XMLDocument();
    document->Parse(content.c_str());

    const tinyxml2::XMLElement* rootElement = document->RootElement();// Root

    const tinyxml2::XMLElement* element = rootElement->FirstChildElement();

    bool serializeEnabled = false;
    std::string rootType = "";

    while (element)
    {
        if (strcmp("PropertyGroup", element->Name()) == 0)
        {
            const tinyxml2::XMLAttribute* attribute = element->FirstAttribute();
            while (attribute && strcmp("Version", attribute->Name()) != 0)
                attribute = attribute->Next();
            if (attribute)
                _csdVersion = attribute->Value();

            _csdVersion = "2.1.0.0";
        }

        if (strcmp("Content", element->Name()) == 0)
        {
            const tinyxml2::XMLAttribute* attribute = element->FirstAttribute();

            //
            if (!attribute)
            {
                serializeEnabled = true;
                rootType = "NodeObjectData";
            }
        }

        if (serializeEnabled)
        {
            break;
        }

        const tinyxml2::XMLElement* child = element->FirstChildElement();
        if (child)
        {
            element = child;
        }
        else
        {
            element = element->NextSiblingElement();
        }
    }

    if (serializeEnabled)
    {
        _builder = new (std::nothrow) FlatBufferBuilder();

        Offset<NodeTree> nodeTree;
        Offset<NodeAction> aciton;
        std::vector<Offset<flatbuffers::AnimationInfo>> animationInfos;

        const tinyxml2::XMLElement* child = element->FirstChildElement();

        while (child)
        {
            std::string name = child->Name();

            if (name == "Animation") // action
            {
                const tinyxml2::XMLElement* animation = child;
                //todo bugfix 不支持Animation
//                aciton = createNodeAction(animation);
            }
            else if (name == "ObjectData") // nodeTree
            {
                const tinyxml2::XMLElement* objectData = child;

                auto nameElem = objectData->FirstAttribute();
                while (nameElem)
                {
                    if (0 == strcmp("ctype", nameElem->Name()))
                    {
                        rootType = nameElem->Value();
                        break;
                    }
                    else
                        nameElem = nameElem->Next();
                }
                if (rootType == "GameNodeObjectData" || rootType == "GameLayerObjectData")  // for adaptate old version
                    rootType = "NodeObjectData";

                nodeTree = createNodeTree(objectData, rootType);
            }
            else if (name == "AnimationList") // animation list
            {
                //todo bugfix 不支持AnimationList
                const tinyxml2::XMLElement* animationinfoElement = child->FirstChildElement();
                while (animationinfoElement)
                {
                    //todo createAnimationInfo
//                    auto animationinfo = createAnimationInfo(animationinfoElement);
//                    animationInfos.push_back(animationinfo);
//                    animationinfoElement = animationinfoElement->NextSiblingElement();
                }
            }
            child = child->NextSiblingElement();
        }


        auto csparsebinary = CreateCSParseBinary(*_builder,
                                                 _builder->CreateString(_csdVersion),
                                                 _builder->CreateVector(_textures),
                                                 _builder->CreateVector(_texturePngs),
                                                 nodeTree,
                                                 aciton,
                                                 _builder->CreateVector(animationInfos));
        _builder->Finish(csparsebinary);

        _textures.clear();
        _texturePngs.clear();


        std::string outFullPath = flatbuffersFileName;
        size_t pos = outFullPath.find_last_of('.');
        std::string convert = outFullPath.substr(0, pos).append(".csb");
        auto save = flatbuffers::SaveFile(convert.c_str(),
                                          reinterpret_cast<const char *>(_builder->GetBufferPointer()),
                                          _builder->GetSize(),
                                          true);
        if (!save)
        {
            return "couldn't save files!";
        }

        deleteFlatBufferBuilder();
    }

    return "";
}

flatbuffers::Offset<flatbuffers::NodeTree>
FlatBuffersSerialize::createNodeTree(const tinyxml2::XMLElement *objectData, std::string classType) {
    std::string classname = classType.substr(0, classType.find("ObjectData"));
//    CCLOG("classname = %s", classname.c_str());

    std::string name = "";

    Offset<Options> options;
    std::vector<Offset<NodeTree>> children;

    if (classname == "ProjectNode")
    {
        auto reader = ProjectNodeReader::getInstance();
        options = CreateOptions(*_builder, reader->createOptionsWithFlatBuffers(objectData, _builder));
    }
    else if (classname == "SimpleAudio")
    {
        auto reader = ComAudioReader::getInstance();
        options = CreateOptions(*_builder, reader->createOptionsWithFlatBuffers(objectData, _builder));
    }
    else
    {
        std::string readername = getGUIClassName(classname);
        readername.append("Reader");
        NodeReaderProtocol* reader = dynamic_cast<NodeReaderProtocol*>(ObjectFactory::getInstance()->createObject(readername));
        if (reader != nullptr)
        {
            options = CreateOptions(*_builder, reader->createOptionsWithFlatBuffers(objectData, _builder));
        }else{
            std::cout << "not support \t" << readername << std::endl;
        }
    }


    // children
    bool containChildrenElement = false;
    const tinyxml2::XMLElement* child = objectData->FirstChildElement();

    while (child)
    {
        if (strcmp("Children", child->Name()) == 0)
        {
            containChildrenElement = true;
            break;
        }
        child = child->NextSiblingElement();
    }

    if (containChildrenElement)
    {
        child = child->FirstChildElement();
//        CCLOG("element name = %s", child->Name());

        while (child)
        {
            const tinyxml2::XMLAttribute* attribute = child->FirstAttribute();
            bool bHasType = false;
            while (attribute)
            {
                std::string attriname = attribute->Name();
                std::string value = attribute->Value();

                if (attriname == "ctype")
                {
                    children.push_back(createNodeTree(child, value));

                    bHasType = true;
                    break;
                }

                attribute = attribute->Next();
            }

            if(!bHasType)
            {
                children.push_back(createNodeTree(child, "NodeObjectData"));
            }

            child = child->NextSiblingElement();
        }
    }
    //

    std::string customClassName = "";
    const tinyxml2::XMLAttribute* attribute = objectData->FirstAttribute();
    while (attribute)
    {
        std::string attriname = attribute->Name();
        std::string value = attribute->Value();

        if (attriname == "CustomClassName")
        {
            customClassName = value;
            break;
        }

        attribute = attribute->Next();
    }

    return CreateNodeTree(*_builder,
                          _builder->CreateString(classname),
                          _builder->CreateVector(children),
                          options,
                          _builder->CreateString(customClassName));

}

std::string FlatBuffersSerialize::getGUIClassName(const std::string &name)
{
    std::string convertedClassName = name;
    if (name == "Panel")
    {
        convertedClassName = "Layout";
    }
    else if (name == "TextArea")
    {
        convertedClassName = "Text";
    }
    else if (name == "TextButton")
    {
        convertedClassName = "Button";
    }
    else if (name == "Label")
    {
        convertedClassName = "Text";
    }
    else if (name == "LabelAtlas")
    {
        convertedClassName = "TextAtlas";
    }
    else if (name == "LabelBMFont")
    {
        convertedClassName = "TextBMFont";
    }


    return convertedClassName;
}

void FlatBuffersSerialize::deleteFlatBufferBuilder()
{
    if (_builder != nullptr)
    {
        _builder->Clear();
        delete _builder;
        _builder = nullptr;
    }
}

FlatBuffersSerialize::FlatBuffersSerialize() {
    CREATE_CLASS_NODE_READER_INFO(NodeReader);
    CREATE_CLASS_NODE_READER_INFO(SingleNodeReader);
    CREATE_CLASS_NODE_READER_INFO(SpriteReader);
//    CREATE_CLASS_NODE_READER_INFO(ParticleReader);
//    CREATE_CLASS_NODE_READER_INFO(GameMapReader);
//
    CREATE_CLASS_NODE_READER_INFO(ButtonReader);
//    CREATE_CLASS_NODE_READER_INFO(CheckBoxReader);
    CREATE_CLASS_NODE_READER_INFO(ImageViewReader);
//    CREATE_CLASS_NODE_READER_INFO(TextBMFontReader);
    CREATE_CLASS_NODE_READER_INFO(TextReader);
//    CREATE_CLASS_NODE_READER_INFO(TextFieldReader);
//    CREATE_CLASS_NODE_READER_INFO(TextAtlasReader);
//    CREATE_CLASS_NODE_READER_INFO(LoadingBarReader);
//    CREATE_CLASS_NODE_READER_INFO(SliderReader);
    CREATE_CLASS_NODE_READER_INFO(LayoutReader);
//    CREATE_CLASS_NODE_READER_INFO(ScrollViewReader);
//    CREATE_CLASS_NODE_READER_INFO(PageViewReader);
//    CREATE_CLASS_NODE_READER_INFO(ListViewReader);
}
