//
// Created by yzqlwt on 2020/12/1.
//

#include "ComAudioReader.h"
#include "flatbuffers/flatbuffers.h"
#include "CSParseBinary_generated.h"
#include "NodeReader.h"
using namespace flatbuffers;

static ComAudioReader* _instanceComAudioReader = nullptr;

ComAudioReader* ComAudioReader::getInstance()
{
    if (!_instanceComAudioReader)
    {
        _instanceComAudioReader = new ComAudioReader();
    }

    return _instanceComAudioReader;
}

Offset<Table> ComAudioReader::createOptionsWithFlatBuffers(const tinyxml2::XMLElement *objectData,
                                                           flatbuffers::FlatBufferBuilder *builder)
{
    auto temp = NodeReader::getInstance()->createOptionsWithFlatBuffers(objectData, builder);
    auto nodeOptions = *(Offset<WidgetOptions>*)(&temp);

    std::string name = "";
    bool enabled = false;
    bool loop = false;
    float volume = 0;

    std::string path = "";
    std::string plist = "";
    int resourceType = 0;

    const tinyxml2::XMLAttribute* attribute = objectData->FirstAttribute();
    while (attribute)
    {
        std::string attriname = attribute->Name();
        std::string value = attribute->Value();

        if (attriname == "Loop")
        {
            loop = (value == "True") ? true : false;
        }
        else if (attriname == "Volume")
        {
            volume = atof(value.c_str());
        }
        else if (attriname == "Name")
        {
            name = value;
        }

        attribute = attribute->Next();
    }

    // FileData
    const tinyxml2::XMLElement* child = objectData->FirstChildElement();
    while (child)
    {
        std::string attriname = child->Name();

        if (attriname == "FileData")
        {
            attribute = child->FirstAttribute();

            while (attribute)
            {
                attriname = attribute->Name();
                std::string value = attribute->Value();

                if (attriname == "Path")
                {
                    path = value;
                }
                else if (attriname == "Type")
                {
                    resourceType = 0;
                }
                else if (attriname == "Plist")
                {
                    plist = value;
                }

                attribute = attribute->Next();
            }
        }

        child = child->NextSiblingElement();
    }

    auto options = CreateComAudioOptions(*builder,
                                         nodeOptions,
                                         builder->CreateString(name),
                                         enabled,
                                         loop,
                                         volume,
                                         CreateResourceData(*builder,
                                                            builder->CreateString(path),
                                                            builder->CreateString(plist),
                                                            resourceType));

    return *(Offset<Table>*)(&options);
}