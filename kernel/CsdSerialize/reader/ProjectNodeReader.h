//
// Created by yzqlwt on 2020/12/1.
//

#ifndef TESTCSD_PROJECTNODEREADER_H
#define TESTCSD_PROJECTNODEREADER_H

#include "NodeReaderProtocol.h"

class ProjectNodeReader : public NodeReaderProtocol {

public:

    static ProjectNodeReader *getInstance();

    flatbuffers::Offset<flatbuffers::Table> createOptionsWithFlatBuffers(const tinyxml2::XMLElement *objectData,
                                                                         flatbuffers::FlatBufferBuilder *builder);
};

#endif //TESTCSD_PROJECTNODEREADER_H
