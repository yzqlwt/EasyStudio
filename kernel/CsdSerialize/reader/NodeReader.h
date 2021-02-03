//
// Created by yzqlwt on 2020/12/1.
//

#ifndef TESTCSD_NODEREADER_H
#define TESTCSD_NODEREADER_H

#include "NodeReaderProtocol.h"

class NodeReader : public NodeReaderProtocol {

public:

    static NodeReader *getInstance();

    flatbuffers::Offset<flatbuffers::Table> createOptionsWithFlatBuffers(const tinyxml2::XMLElement *objectData,
                                                                         flatbuffers::FlatBufferBuilder *builder);

};

#endif //TESTCSD_NODEREADER_H
