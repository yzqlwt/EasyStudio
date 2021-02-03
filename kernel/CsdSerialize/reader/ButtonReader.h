//
// Created by yzqlwt on 2020/12/2.
//

#ifndef TESTCSD_BUTTONREADER_H
#define TESTCSD_BUTTONREADER_H

#include "NodeReaderProtocol.h"
#include "WidgetReader.h"

class  ButtonReader : public WidgetReader
{

public:

static ButtonReader* getInstance();
/** @deprecated Use method destroyInstance() instead */

flatbuffers::Offset<flatbuffers::Table> createOptionsWithFlatBuffers(const tinyxml2::XMLElement* objectData,
                                                                     flatbuffers::FlatBufferBuilder* builder);

int getResourceType(std::string key);

};


#endif //TESTCSD_BUTTONREADER_H
