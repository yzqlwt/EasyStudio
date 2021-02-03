//
// Created by yzqlwt on 2020/12/2.
//

#ifndef TESTCSD_SINGLENODEREADER_H
#define TESTCSD_SINGLENODEREADER_H

#include "NodeReaderProtocol.h"

class SingleNodeReader : public NodeReaderProtocol {

public:

    static SingleNodeReader *getInstance();


    flatbuffers::Offset<flatbuffers::Table> createOptionsWithFlatBuffers(const tinyxml2::XMLElement *objectData,
                                                                         flatbuffers::FlatBufferBuilder *builder);

};

#endif //TESTCSD_SINGLENODEREADER_H
