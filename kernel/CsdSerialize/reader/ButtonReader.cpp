//
// Created by yzqlwt on 2020/12/2.
//

#include "ButtonReader.h"
#include "CSParseBinary_generated.h"
#include "ccTypes.h"
#include "FlatBuffersSerialize.h"
#include "NodeReader.h"
using namespace flatbuffers;

static const char* P_Scale9Enable = "scale9Enable";
static const char* P_NormalData = "normalData";
static const char* P_PressedData = "pressedData";
static const char* P_DisabledData = "disabledData";
static const char* P_Text = "text";
static const char* P_CapInsetsX = "capInsetsX";
static const char* P_CapInsetsY = "capInsetsY";
static const char* P_CapInsetsWidth = "capInsetsWidth";
static const char* P_CapInsetsHeight = "capInsetsHeight";
static const char* P_Scale9Width = "scale9Width";
static const char* P_Scale9Height = "scale9Height";
static const char* P_TextColorR = "textColorR";
static const char* P_TextColorG = "textColorG";
static const char* P_TextColorB = "textColorB";
static const char* P_FontSize = "fontSize";
static const char* P_FontName = "fontName";


static ButtonReader* instanceButtonReader = nullptr;



ButtonReader* ButtonReader::getInstance()
{
    if (!instanceButtonReader)
    {
        instanceButtonReader = new (std::nothrow) ButtonReader();
    }
    return instanceButtonReader;
}

Offset<Table> ButtonReader::createOptionsWithFlatBuffers(const tinyxml2::XMLElement *objectData, flatbuffers::FlatBufferBuilder *builder)
{
    auto temp = WidgetReader::getInstance()->createOptionsWithFlatBuffers(objectData, builder);
    auto widgetOptions = *(Offset<WidgetOptions>*)(&temp);

    bool displaystate = true;
    bool scale9Enabled = false;
    Rect capInsets;
    std::string text = "";
    bool isLocalized = false;
    int fontSize = 14;
    std::string fontName = "";
    Size scale9Size;
    Color4B textColor(255, 255, 255, 255);

    std::string normalPath = "";
    std::string normalPlistFile = "";
    int normalResourceType = 0;

    std::string pressedPath = "";
    std::string pressedPlistFile = "";
    int pressedResourceType = 0;

    std::string disabledPath = "";
    std::string disabledPlistFile = "";
    int disabledResourceType = 0;

    std::string fontResourcePath = "";
    std::string fontResourcePlistFile = "";
    int fontResourceResourceType = 0;

    bool outlineEnabled = false;
    Color4B outlineColor = Color4B(  0,   0,   0, 255);
    int outlineSize = 1;
    bool shadowEnabled = false;
    Color4B shadowColor = Color4B(  0,   0,   0, 255);
    Size shadowOffset = Size(2, -2);
    int shadowBlurRadius = 0;

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
        else if (name == "ButtonText")
        {
            text = value;
        }
        else if (name == "IsLocalized")
        {
            isLocalized = (value == "True") ? true : false;
        }
        else if (name == "FontSize")
        {
            fontSize = atoi(value.c_str());
        }
        else if (name == "FontName")
        {
            fontName = value;
        }
        else if (name == "DisplayState")
        {
            displaystate = (value == "True") ? true : false;
        }
        else if (name == "OutlineEnabled")
        {
            outlineEnabled = (value == "True") ? true : false;
        }
        else if (name == "OutlineSize")
        {
            outlineSize = atoi(value.c_str());
        }
        else if (name == "ShadowEnabled")
        {
            shadowEnabled = (value == "True") ? true : false;
        }
        else if (name == "ShadowOffsetX")
        {
            shadowOffset.width = atof(value.c_str());
        }
        else if (name == "ShadowOffsetY")
        {
            shadowOffset.height = atof(value.c_str());
        }
        else if (name == "ShadowBlurRadius")
        {
            shadowBlurRadius = atoi(value.c_str());
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
        else if (name == "TextColor")
        {
            attribute = child->FirstAttribute();
            while (attribute)
            {
                name = attribute->Name();
                std::string value = attribute->Value();

                if (name == "R")
                {
                    textColor.r = atoi(value.c_str());
                }
                else if (name == "G")
                {
                    textColor.g = atoi(value.c_str());
                }
                else if (name == "B")
                {
                    textColor.b = atoi(value.c_str());
                }

                attribute = attribute->Next();
            }
        }
        else if (name == "DisabledFileData")
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
                    disabledPath = value;
                }
                else if (name == "Type")
                {
                    disabledResourceType = getResourceType(value);
                }
                else if (name == "Plist")
                {
                    disabledPlistFile = value;
                    texture = value;
                }

                attribute = attribute->Next();
            }

            if (disabledResourceType == 1)
            {
                FlatBuffersSerialize* fbs = FlatBuffersSerialize::getInstance();
                fbs->_textures.push_back(builder->CreateString(texture));
            }
        }
        else if (name == "PressedFileData")
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
                    pressedPath = value;
                }
                else if (name == "Type")
                {
                    pressedResourceType = getResourceType(value);
                }
                else if (name == "Plist")
                {
                    pressedPlistFile = value;
                    texture = value;
                }

                attribute = attribute->Next();
            }

            if (pressedResourceType == 1)
            {
                FlatBuffersSerialize* fbs = FlatBuffersSerialize::getInstance();
                fbs->_textures.push_back(builder->CreateString(texture));
            }
        }
        else if (name == "NormalFileData")
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
                    normalPath = value;
                }
                else if (name == "Type")
                {
                    normalResourceType = getResourceType(value);
                }
                else if (name == "Plist")
                {
                    normalPlistFile = value;
                    texture = value;
                }

                attribute = attribute->Next();
            }

            if (normalResourceType == 1)
            {
                FlatBuffersSerialize* fbs = FlatBuffersSerialize::getInstance();
                fbs->_textures.push_back(builder->CreateString(texture));
            }
        }
        else if (name == "FontResource")
        {
            attribute = child->FirstAttribute();

            while (attribute)
            {
                name = attribute->Name();
                std::string value = attribute->Value();

                if (name == "Path")
                {
                    fontResourcePath = value;
                }
                else if (name == "Type")
                {
                    fontResourceResourceType = getResourceType(value);
                }
                else if (name == "Plist")
                {
                    fontResourcePlistFile = value;
                }

                attribute = attribute->Next();
            }
        }
        else if (name == "OutlineColor")
        {
            attribute = child->FirstAttribute();

            while (attribute)
            {
                name = attribute->Name();
                std::string value = attribute->Value();

                if (name == "A")
                {
                    outlineColor.a = atoi(value.c_str());
                }
                else if (name == "R")
                {
                    outlineColor.r = atoi(value.c_str());
                }
                else if (name == "G")
                {
                    outlineColor.g = atoi(value.c_str());
                }
                else if (name == "B")
                {
                    outlineColor.b = atoi(value.c_str());
                }

                attribute = attribute->Next();
            }
        }
        else if (name == "ShadowColor")
        {
            attribute = child->FirstAttribute();

            while (attribute)
            {
                name = attribute->Name();
                std::string value = attribute->Value();

                if (name == "A")
                {
                    shadowColor.a = atoi(value.c_str());
                }
                else if (name == "R")
                {
                    shadowColor.r = atoi(value.c_str());
                }
                else if (name == "G")
                {
                    shadowColor.g = atoi(value.c_str());
                }
                else if (name == "B")
                {
                    shadowColor.b = atoi(value.c_str());
                }

                attribute = attribute->Next();
            }
        }

        child = child->NextSiblingElement();
    }

    Color f_textColor(255, textColor.r, textColor.g, textColor.b);
    CapInsets f_capInsets(capInsets.origin.x, capInsets.origin.y, capInsets.size.width, capInsets.size.height);
    FlatSize f_scale9Size(scale9Size.width, scale9Size.height);
    flatbuffers::Color f_outlineColor(outlineColor.a, outlineColor.r, outlineColor.g, outlineColor.b);
    flatbuffers::Color f_shadowColor(shadowColor.a, shadowColor.r, shadowColor.g, shadowColor.b);

    auto options = CreateButtonOptions(*builder,
                                       widgetOptions,
                                       CreateResourceData(*builder,
                                                          builder->CreateString(normalPath),
                                                          builder->CreateString(normalPlistFile),
                                                          normalResourceType),
                                       CreateResourceData(*builder,
                                                          builder->CreateString(pressedPath),
                                                          builder->CreateString(pressedPlistFile),
                                                          pressedResourceType),
                                       CreateResourceData(*builder,
                                                          builder->CreateString(disabledPath),
                                                          builder->CreateString(disabledPlistFile),
                                                          disabledResourceType),
                                       CreateResourceData(*builder,
                                                          builder->CreateString(fontResourcePath),
                                                          builder->CreateString(fontResourcePlistFile),
                                                          fontResourceResourceType),
                                       builder->CreateString(text),
                                       builder->CreateString(fontName),
                                       fontSize,
                                       &f_textColor,
                                       &f_capInsets,
                                       &f_scale9Size,
                                       scale9Enabled,
                                       displaystate,
                                       outlineEnabled,
                                       &f_outlineColor,
                                       outlineSize,
                                       shadowEnabled,
                                       &f_shadowColor,
                                       shadowOffset.width,
                                       shadowOffset.height,
                                       shadowBlurRadius,
                                       isLocalized);

    return *(Offset<Table>*)(&options);
}

int ButtonReader::getResourceType(std::string key)
{
    if(key == "Normal" || key == "Default" || key == "MarkedSubImage")
    {
        return 	0;
    }
    return 1;
}