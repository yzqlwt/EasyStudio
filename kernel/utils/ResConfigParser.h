//
// Created by yzqlwt on 2020/12/29.
//

#ifndef EASYSTUDIO_RESCONFIGPARSER_H
#define EASYSTUDIO_RESCONFIGPARSER_H

#include <string>

class ResConfigParser {

public:
    ResConfigParser(const std::string& dir, const std::string& zipPath);
    void Parse();
private:
    std::string m_targetDir;
    std::string m_resDir;
    std::string m_tempDir;
    std::string m_zipDir;
};


#endif //EASYSTUDIO_RESCONFIGPARSER_H
