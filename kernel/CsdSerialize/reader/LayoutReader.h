//
// Created by yzqlwt on 2020/12/2.
//

#ifndef TESTCSD_LAYOUTREADER_H
#define TESTCSD_LAYOUTREADER_H
#include "NodeReaderProtocol.h"
#include "WidgetReader.h"

class  LayoutReader : public WidgetReader
{


public:

static LayoutReader* getInstance();
/** @deprecated Use method destroyInstance() instead */
static void destroyInstance();

flatbuffers::Offset<flatbuffers::Table> createOptionsWithFlatBuffers(const tinyxml2::XMLElement* objectData,
                                                                     flatbuffers::FlatBufferBuilder* builder);

int getResourceType(std::string key);
};


#endif //TESTCSD_LAYOUTREADER_H
