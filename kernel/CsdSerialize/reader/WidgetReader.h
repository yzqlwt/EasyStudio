//
// Created by yzqlwt on 2020/12/1.
//

#ifndef TESTCSD_WIDGETREADER_H
#define TESTCSD_WIDGETREADER_H

#include "NodeReaderProtocol.h"

class  WidgetReader : public NodeReaderProtocol
{

public:

static WidgetReader* getInstance();



/* flatbuffers refactoring */
flatbuffers::Offset<flatbuffers::Table> createOptionsWithFlatBuffers(const tinyxml2::XMLElement* objectData,
                                                                     flatbuffers::FlatBufferBuilder* builder);
/**/

};


#endif //TESTCSD_WIDGETREADER_H
