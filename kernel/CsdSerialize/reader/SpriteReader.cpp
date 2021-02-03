//
// Created by yzqlwt on 2020/12/2.
//

#include "SpriteReader.h"
#include "CSParseBinary_generated.h"
#include "ccTypes.h"
#include "FlatBuffersSerialize.h"
#include "NodeReader.h"
using namespace flatbuffers;

static SpriteReader* _instanceSpriteReader = nullptr;

SpriteReader* SpriteReader::getInstance()
{
    if (!_instanceSpriteReader)
    {
        _instanceSpriteReader = new (std::nothrow) SpriteReader();
    }

    return _instanceSpriteReader;
}

Offset<Table> SpriteReader::createOptionsWithFlatBuffers(const tinyxml2::XMLElement *objectData,
                                                         flatbuffers::FlatBufferBuilder *builder)
{
    auto temp = NodeReader::getInstance()->createOptionsWithFlatBuffers(objectData, builder);
    auto nodeOptions = *(Offset<WidgetOptions>*)(&temp);

    std::string path = "";
    std::string plistFile = "";
    int resourceType = 0;

    cocosstudio::BlendFunc blendFunc = cocosstudio::BlendFunc::ALPHA_PREMULTIPLIED;

    // FileData
    const tinyxml2::XMLElement* child = objectData->FirstChildElement();
    while (child)
    {
        std::string name = child->Name();

        if (name == "FileData")
        {
            std::string texture = "";
            std::string texturePng = "";

            const tinyxml2::XMLAttribute* attribute = child->FirstAttribute();

            while (attribute)
            {
                name = attribute->Name();
                std::string value = attribute->Value();

                if (name == "Path")
                {
                    path = value;
                }
                else if (name == "Type")
                {
                    resourceType = getResourceType(value);
                }
                else if (name == "Plist")
                {
                    plistFile = value;
                    texture = value;
                }

                attribute = attribute->Next();
            }

            if (resourceType == 1)
            {
                FlatBuffersSerialize* fbs = FlatBuffersSerialize::getInstance();
                fbs->_textures.push_back(builder->CreateString(texture));
            }
        }
        else if (name == "BlendFunc")
        {
            const tinyxml2::XMLAttribute* attribute = child->FirstAttribute();

            while (attribute)
            {
                name = attribute->Name();
                std::string value = attribute->Value();

                if (name == "Src")
                {
                    blendFunc.src = atoi(value.c_str());
                }
                else if (name == "Dst")
                {
                    blendFunc.dst = atoi(value.c_str());
                }

                attribute = attribute->Next();
            }
        }

        child = child->NextSiblingElement();
    }

    flatbuffers::BlendFunc f_blendFunc(blendFunc.src, blendFunc.dst);

    auto options = CreateSpriteOptions(*builder,
                                       nodeOptions,
                                       CreateResourceData(*builder,
                                                          builder->CreateString(path),
                                                          builder->CreateString(plistFile),
                                                          resourceType),
                                       &f_blendFunc);

    return *(Offset<Table>*)(&options);
}

int SpriteReader::getResourceType(std::string key)
{
    if(key == "Normal" || key == "Default" || key == "MarkedSubImage")
    {
        return 	0;
    }
    return 1;
}
