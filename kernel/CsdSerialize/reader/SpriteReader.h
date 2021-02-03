//
// Created by yzqlwt on 2020/12/2.
//

#ifndef TESTCSD_SPRITEREADER_H
#define TESTCSD_SPRITEREADER_H

#include "NodeReaderProtocol.h"


class  SpriteReader : public NodeReaderProtocol
{

public:

static SpriteReader* getInstance();
/** @deprecated Use method destroyInstance() instead */

flatbuffers::Offset<flatbuffers::Table> createOptionsWithFlatBuffers(const tinyxml2::XMLElement* objectData,
                                                                     flatbuffers::FlatBufferBuilder* builder);
int getResourceType(std::string key);
};


#endif //TESTCSD_SPRITEREADER_H
