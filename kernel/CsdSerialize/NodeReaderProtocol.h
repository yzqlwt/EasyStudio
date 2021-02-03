//
// Created by yzqlwt on 2020/12/1.
//

#ifndef TESTCSD_NODEREADERPROTOCOL_H
#define TESTCSD_NODEREADERPROTOCOL_H
#include <tinyxml2.h>
#include "flatbuffers/util.h"
#include "flatbuffers/flatbuffers.h"
class NodeReaderProtocol {
public:
    NodeReaderProtocol() {};

    virtual ~NodeReaderProtocol() {};

    virtual flatbuffers::Offset <flatbuffers::Table>
    createOptionsWithFlatBuffers(const tinyxml2::XMLElement *objectData,
                                 flatbuffers::FlatBufferBuilder *builder) = 0;
};


#endif //TESTCSD_NODEREADERPROTOCOL_H
