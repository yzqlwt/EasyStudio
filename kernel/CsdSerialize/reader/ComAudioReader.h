//
// Created by yzqlwt on 2020/12/1.
//

#ifndef TESTCSD_COMAUDIOREADER_H
#define TESTCSD_COMAUDIOREADER_H

#include "NodeReaderProtocol.h"

class ComAudioReader : public NodeReaderProtocol {
public:

    static ComAudioReader *getInstance();

    flatbuffers::Offset<flatbuffers::Table> createOptionsWithFlatBuffers(const tinyxml2::XMLElement *objectData,
                                                                         flatbuffers::FlatBufferBuilder *builder);

};
#endif //TESTCSD_COMAUDIOREADER_H
