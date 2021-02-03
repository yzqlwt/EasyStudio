//
// Created by yzqlwt on 2020/12/1.
//

#include "ImageViewReader.h"
#include "flatbuffers/flatbuffers.h"
#include "CSParseBinary_generated.h"
#include "ccTypes.h"
#include "FlatBuffersSerialize.h"
using namespace flatbuffers;

static ImageViewReader* instanceImageViewReader = nullptr;

ImageViewReader* ImageViewReader::getInstance()
{
    if (!instanceImageViewReader)
    {
        instanceImageViewReader = new (std::nothrow) ImageViewReader();
    }
    return instanceImageViewReader;
}


Offset<Table> ImageViewReader::createOptionsWithFlatBuffers(const tinyxml2::XMLElement *objectData,
                                                            flatbuffers::FlatBufferBuilder *builder)
{
    auto temp = WidgetReader::getInstance()->createOptionsWithFlatBuffers(objectData, builder);
    auto widgetOptions = *(Offset<WidgetOptions>*)(&temp);

    bool scale9Enabled = false;
    Rect capInsets;
    Size scale9Size;

    std::string path = "";
    std::string plistFile = "";
    int resourceType = 0;

    // attributes
    const tinyxml2::XMLAttribute* attribute = objectData->FirstAttribute();
    while (attribute)
    {
        std::string name = attribute->Name();
        std::string value = attribute->Value();

        if (name == "Scale9Enable")
        {
            if (value == "True")
            {
                scale9Enabled = true;
            }
        }
        else if (name == "Scale9OriginX")
        {
            capInsets.origin.x = atof(value.c_str());
        }
        else if (name == "Scale9OriginY")
        {
            capInsets.origin.y = atof(value.c_str());
        }
        else if (name == "Scale9Width")
        {
            capInsets.size.width = atof(value.c_str());
        }
        else if (name == "Scale9Height")
        {
            capInsets.size.height = atof(value.c_str());
        }

        attribute = attribute->Next();
    }

    // child elements
    const tinyxml2::XMLElement* child = objectData->FirstChildElement();
    while (child)
    {
        std::string name = child->Name();

        if (name == "Size" && scale9Enabled)
        {
            attribute = child->FirstAttribute();

            while (attribute)
            {
                name = attribute->Name();
                std::string value = attribute->Value();

                if (name == "X")
                {
                    scale9Size.width = atof(value.c_str());
                }
                else if (name == "Y")
                {
                    scale9Size.height = atof(value.c_str());
                }

                attribute = attribute->Next();
            }
        }
        else if (name == "FileData")
        {
            std::string texture = "";
            std::string texturePng = "";

            attribute = child->FirstAttribute();

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

        child = child->NextSiblingElement();
    }

    CapInsets f_capInsets(capInsets.origin.x, capInsets.origin.y, capInsets.size.width, capInsets.size.height);
    FlatSize f_scale9Size(scale9Size.width, scale9Size.height);

    auto options = CreateImageViewOptions(*builder,
                                          widgetOptions,
                                          CreateResourceData(*builder,
                                                             builder->CreateString(path),
                                                             builder->CreateString(plistFile),
                                                             resourceType),
                                          &f_capInsets,
                                          &f_scale9Size,
                                          scale9Enabled);

    return *(Offset<Table>*)(&options);
}

int ImageViewReader::getResourceType(std::string key)
{
    if(key == "Normal" || key == "Default" || key == "MarkedSubImage")
    {
        return 	0;
    }
    return 1;
}
