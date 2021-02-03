//
// Created by yzqlwt on 2020/12/2.
//

#ifndef TESTCSD_TEXTREADER_H
#define TESTCSD_TEXTREADER_H
#include "NodeReaderProtocol.h"
#include "WidgetReader.h"
class  TextReader : public WidgetReader
{

public:

static TextReader* getInstance();
/** @deprecated Use method destroyInstance() instead */

flatbuffers::Offset<flatbuffers::Table> createOptionsWithFlatBuffers(const tinyxml2::XMLElement* objectData,
                                                                     flatbuffers::FlatBufferBuilder* builder);

};


#endif //TESTCSD_TEXTREADER_H
