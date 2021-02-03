//
// Created by yzqlwt on 2020/12/2.
//

#include "SingleNodeReader.h"
#include "CSParseBinary_generated.h"
#include "ccTypes.h"
#include "FlatBuffersSerialize.h"
#include "NodeReader.h"
using namespace flatbuffers;

static SingleNodeReader* _instanceSingleNodeReader = nullptr;

SingleNodeReader* SingleNodeReader::getInstance()
{
    if (!_instanceSingleNodeReader)
    {
        _instanceSingleNodeReader = new (std::nothrow) SingleNodeReader();
    }

    return _instanceSingleNodeReader;
}

Offset<Table> SingleNodeReader::createOptionsWithFlatBuffers(const tinyxml2::XMLElement *objectData,
                                                             flatbuffers::FlatBufferBuilder *builder)
{
    auto temp = NodeReader::getInstance()->createOptionsWithFlatBuffers(objectData, builder);
    auto nodeOptions = *(Offset<WidgetOptions>*)(&temp);

    auto options = CreateSingleNodeOptions(*builder,
                                           nodeOptions);

    return *(Offset<Table>*)(&options);
}