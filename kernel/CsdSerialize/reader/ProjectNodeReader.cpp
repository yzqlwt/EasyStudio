//
// Created by yzqlwt on 2020/12/1.
//

#include "ProjectNodeReader.h"
#include "flatbuffers/flatbuffers.h"
#include "CSParseBinary_generated.h"
#include "NodeReader.h"
using namespace flatbuffers;

static ProjectNodeReader* _instanceProjectNodeReader = nullptr;

ProjectNodeReader* ProjectNodeReader::getInstance()
{
    if (!_instanceProjectNodeReader)
    {
        _instanceProjectNodeReader = new (std::nothrow) ProjectNodeReader();
    }

    return _instanceProjectNodeReader;
}

Offset<Table> ProjectNodeReader::createOptionsWithFlatBuffers(const tinyxml2::XMLElement *objectData,
                                                              flatbuffers::FlatBufferBuilder *builder)
{
    auto temp = NodeReader::getInstance()->createOptionsWithFlatBuffers(objectData, builder);
    auto nodeOptions = *(Offset<WidgetOptions>*)(&temp);

    std::string filename = "";
    float innerspeed = 1.0f;

    const tinyxml2::XMLAttribute* objattri = objectData->FirstAttribute();
    // inneraction speed
    while (objattri)
    {
        std::string name = objattri->Name();
        std::string value = objattri->Value();
        if (name == "InnerActionSpeed")
        {
            innerspeed = atof(objattri->Value());
            break;
        }
        objattri = objattri->Next();
    }

    // FileData
    const tinyxml2::XMLElement* child = objectData->FirstChildElement();
    while (child)
    {
        std::string name = child->Name();

        if (name == "FileData")
        {
            const tinyxml2::XMLAttribute* attribute = child->FirstAttribute();

            while (attribute)
            {
                name = attribute->Name();
                std::string value = attribute->Value();

                if (name == "Path")
                {
                    size_t pos = value.find_last_of('.');
                    std::string convert = value.substr(0, pos).append(".csb");
                    filename = convert;
                }

                attribute = attribute->Next();
            }
        }

        child = child->NextSiblingElement();
    }

    auto options = CreateProjectNodeOptions(*builder,
                                            nodeOptions,
                                            builder->CreateString(filename),
                                            innerspeed);

    return *(Offset<Table>*)(&options);
}