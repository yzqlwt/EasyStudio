//
// Created by yzqlwt on 2020/12/2.
//

#include "TextReader.h"
#include "CSParseBinary_generated.h"
#include "ccTypes.h"
#include "FlatBuffersSerialize.h"
#include "NodeReader.h"
using namespace flatbuffers;

static const char* P_TouchScaleEnable = "touchScaleEnable";
static const char* P_Text = "text";
static const char* P_FontSize = "fontSize";
static const char* P_FontName = "fontName";
static const char* P_AreaWidth = "areaWidth";
static const char* P_AreaHeight = "areaHeight";
static const char* P_HAlignment = "hAlignment";
static const char* P_VAlignment = "vAlignment";

static TextReader* instanceTextReader = nullptr;


TextReader* TextReader::getInstance()
{
    if (!instanceTextReader)
    {
        instanceTextReader = new (std::nothrow) TextReader();
    }
    return instanceTextReader;
}

Offset<Table> TextReader::createOptionsWithFlatBuffers(const tinyxml2::XMLElement *objectData,
                                                       flatbuffers::FlatBufferBuilder *builder)
{
    auto temp = WidgetReader::getInstance()->createOptionsWithFlatBuffers(objectData, builder);
    auto widgetOptions = *(Offset<WidgetOptions>*)(&temp);

    bool touchScaleEnabled = false;
    bool isCustomSize = false;
    std::string fontName = "";
    int fontSize = 20;
    std::string text = "Text Label";
    bool isLocalized = false;
    int areaWidth = 0;
    int areaHeight = 0;
    int h_alignment = 0;
    int v_alignment = 0;
    bool outlineEnabled = false;
    Color4B outlineColor = Color4B::BLACK;
    int outlineSize = 1;
    bool shadowEnabled = false;
    Color4B shadowColor = Color4B::BLACK;
    Size shadowOffset = Size(2, -2);
    int shadowBlurRadius = 0;

    std::string path = "";
    std::string plistFile = "";
    int resourceType = 0;

    // attributes
    const tinyxml2::XMLAttribute* attribute = objectData->FirstAttribute();
    while (attribute)
    {
        std::string name = attribute->Name();
        std::string value = attribute->Value();

        if (name == "TouchScaleChangeAble")
        {
            touchScaleEnabled = (value == "True") ? true : false;
        }
        else if (name == "LabelText")
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
        else if (name == "AreaWidth")
        {
            areaWidth = atoi(value.c_str());
        }
        else if (name == "AreaHeight")
        {
            areaHeight = atoi(value.c_str());
        }
        else if (name == "HorizontalAlignmentType")
        {
            if (value == "HT_Left")
            {
                h_alignment = 0;
            }
            else if (value == "HT_Center")
            {
                h_alignment = 1;
            }
            else if (value == "HT_Right")
            {
                h_alignment = 2;
            }
        }
        else if (name == "VerticalAlignmentType")
        {
            if (value == "VT_Top")
            {
                v_alignment = 0;
            }
            else if (value == "VT_Center")
            {
                v_alignment = 1;
            }
            else if (value == "VT_Bottom")
            {
                v_alignment = 2;
            }
        }
        else if (name == "IsCustomSize")
        {
            isCustomSize = (value == "True") ? true : false;
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

        if (name == "FontResource")
        {
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
                    resourceType = 0;
                }
                else if (name == "Plist")
                {
                    plistFile = value;
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

    flatbuffers::Color f_outlineColor(outlineColor.a, outlineColor.r, outlineColor.g, outlineColor.b);
    flatbuffers::Color f_shadowColor(shadowColor.a, shadowColor.r, shadowColor.g, shadowColor.b);

    auto options = CreateTextOptions(*builder,
                                     widgetOptions,
                                     CreateResourceData(*builder,
                                                        builder->CreateString(path),
                                                        builder->CreateString(plistFile),
                                                        resourceType),
                                     builder->CreateString(fontName),
                                     fontSize,
                                     builder->CreateString(text),
                                     areaWidth,
                                     areaHeight,
                                     h_alignment,
                                     v_alignment,
                                     touchScaleEnabled,
                                     isCustomSize,
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
