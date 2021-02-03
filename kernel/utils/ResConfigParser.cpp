//
// Created by yzqlwt on 2020/12/29.
//

#include "ResConfigParser.h"
#include "./DirHelper.h"
#include "./ZipHelper.h"

ResConfigParser::ResConfigParser(const std::string &dir, const std::string &zipPath) {
    this->m_targetDir = dir;
    this->m_resDir = DirHelper::GetResourceDir(dir);
    this->m_zipDir = DirHelper::GetZipDir(dir);
    this->m_tempDir = DirHelper::GetTempDir(dir);
    ZipHelper::UnZip(zipPath, this->m_tempDir);
}

void ResConfigParser::Parse() {

}
