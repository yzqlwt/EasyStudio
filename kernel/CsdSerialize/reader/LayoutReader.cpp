//
// Created by yzqlwt on 2020/12/2.
//

#include "LayoutReader.h"
#include "CSParseBinary_generated.h"
#include "ccTypes.h"
#include "FlatBuffersSerialize.h"
#include "NodeReader.h"
using namespace flatbuffers;

static const char* P_CapInsetsX = "capInsetsX";
static const char* P_CapInsetsY = "capInsetsY";
static const char* P_CapInsetsWidth = "capInsetsWidth";
static const char* P_CapInsetsHeight = "capInsetsHeight";
static const char* P_ClipAble = "clipAble";
static const char* P_BackGroundScale9Enable = "backGroundScale9Enable";
static const char* P_BgColorR = "bgColorR";
static const char* P_BgColorG = "bgColorG";
static const char* P_BgColorB = "bgColorB";
static const char* P_BgStartColorR = "bgStartColorR";
static const char* P_BgStartColorG = "bgStartColorG";
static const char* P_BgStartColorB = "bgStartColorB";
static const char* P_BgEndColorR = "bgEndColorR";
static const char* P_BgEndColorG = "bgEndColorG";
static const char* P_BgEndColorB = "bgEndColorB";
static const char* P_VectorX = "vectorX";
static const char* P_VectorY = "vectorY";
static const char* P_BgColorOpacity = "bgColorOpacity";
static const char* P_ColorType = "colorType";
static const char* P_BackGroundImageData = "backGroundImageData";
static const char* P_LayoutType = "layoutType";

static LayoutReader* instanceLayoutReader = nullptr;


LayoutReader* LayoutReader::getInstance()
{
    if (!instanceLayoutReader)
    {
        instanceLayoutReader = new (std::nothrow) LayoutReader();
    }
    return instanceLayoutReader;
}

Offset<Table> LayoutReader::createOptionsWithFlatBuffers(const tinyxml2::XMLElement *objectData,
                                                         flatbuffers::FlatBufferBuilder *builder)
{
    auto temp = WidgetReader::getInstance()->createOptionsWithFlatBuffers(objectData, builder);
    auto widgetOptions = *(Offset<WidgetOptions>*)(&temp);

    std::string path = "";
    std::string plistFile = "";
    int resourceType = 0;

    bool clipEnabled = false;
    Color3B bgColor;
    Color3B bgStartColor;
    Color3B bgEndColor;
    int colorType = 0;
    unsigned char bgColorOpacity = 255;
    Vec2 colorVector(0.0f, -0.5f);
    Rect capInsets;
    Size scale9Size;
    bool backGroundScale9Enabled = false;


    // attributes
    const tinyxml2::XMLAttribute* attribute = objectData->FirstAttribute();
    while (attribute)
    {
        std::string name = attribute->Name();
        std::string value = attribute->Value();

        if (name == "ClipAble")
        {
            clipEnabled = (value == "True") ? true : false;
        }
        else if (name == "ComboBoxIndex")
        {
            colorType = atoi(value.c_str());
        }
        else if (name == "BackColorAlpha")
        {
            bgColorOpacity = atoi(value.c_str());
        }
        else if (name == "Scale9Enable")
        {
            if (value == "True")
            {
                backGroundScale9Enabled = true;
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

        if (name == "Size" && backGroundScale9Enabled)
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
        else if (name == "SingleColor")
        {
            attribute = child->FirstAttribute();

            while (attribute)
            {
                name = attribute->Name();
                std::string value = attribute->Value();

                if (name == "R")
                {
                    bgColor.r = atoi(value.c_str());
                }
                else if (name == "G")
                {
                    bgColor.g = atoi(value.c_str());
                }
                else if (name == "B")
                {
                    bgColor.b = atoi(value.c_str());
                }

                attribute = attribute->Next();
            }
        }
        else if (name == "EndColor")
        {
            attribute = child->FirstAttribute();

            while (attribute)
            {
                name = attribute->Name();
                std::string value = attribute->Value();

                if (name == "R")
                {
                    bgEndColor.r = atoi(value.c_str());
                }
                else if (name == "G")
                {
                    bgEndColor.g = atoi(value.c_str());
                }
                else if (name == "B")
                {
                    bgEndColor.b = atoi(value.c_str());
                }

                attribute = attribute->Next();
            }
        }
        else if (name == "FirstColor")
        {
            attribute = child->FirstAttribute();

            while (attribute)
            {
                name = attribute->Name();
                std::string value = attribute->Value();

                if (name == "R")
                {
                    bgStartColor.r = atoi(value.c_str());
                }
                else if (name == "G")
                {
                    bgStartColor.g = atoi(value.c_str());
                }
                else if (name == "B")
                {
                    bgStartColor.b = atoi(value.c_str());
                }

                attribute = attribute->Next();
            }
        }
        else if (name == "ColorVector")
        {
            attribute = child->FirstAttribute();
            while (attribute)
            {
                name = attribute->Name();
                std::string value = attribute->Value();

                if (name == "ScaleX")
                {
                    colorVector.x = atof(value.c_str());
                }
                else if (name == "ScaleY")
                {
                    colorVector.y = atof(value.c_str());
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

    Color f_bgColor(255, bgColor.r, bgColor.g, bgColor.b);
    Color f_bgStartColor(255, bgStartColor.r, bgStartColor.g, bgStartColor.b);
    Color f_bgEndColor(255, bgEndColor.r, bgEndColor.g, bgEndColor.b);
    ColorVector f_colorVector(colorVector.x, colorVector.y);
    CapInsets f_capInsets(capInsets.origin.x, capInsets.origin.y, capInsets.size.width, capInsets.size.height);
    FlatSize f_scale9Size(scale9Size.width, scale9Size.height);

    auto options = CreatePanelOptions(*builder,
                                      widgetOptions,
                                      CreateResourceData(*builder,
                                                         builder->CreateString(path),
                                                         builder->CreateString(plistFile),
                                                         resourceType),
                                      clipEnabled,
                                      &f_bgColor,
                                      &f_bgStartColor,
                                      &f_bgEndColor,
                                      colorType,
                                      bgColorOpacity,
                                      &f_colorVector,
                                      &f_capInsets,
                                      &f_scale9Size,
                                      backGroundScale9Enabled);

    return *(Offset<Table>*)(&options);
}

int LayoutReader::getResourceType(std::string key)
{
    if(key == "Normal" || key == "Default" || key == "MarkedSubImage")
    {
        return 	0;
    }
    return 1;
}