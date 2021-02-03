//
// Created by yzqlwt on 2020/12/1.
//

#ifndef TESTCSD_IMAGEVIEWREADER_H
#define TESTCSD_IMAGEVIEWREADER_H

#include "NodeReaderProtocol.h"
#include "WidgetReader.h"

class ImageViewReader : public WidgetReader {

public:


    static ImageViewReader *getInstance();

    flatbuffers::Offset<flatbuffers::Table> createOptionsWithFlatBuffers(const tinyxml2::XMLElement *objectData,
                                                                         flatbuffers::FlatBufferBuilder *builder);

    int getResourceType(std::string key);
};


#endif //TESTCSD_IMAGEVIEWREADER_H
