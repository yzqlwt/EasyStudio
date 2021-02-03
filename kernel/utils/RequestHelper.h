//
// Created by yzqlwt on 2020/11/28.
//

#ifndef EASYSTUDIO_REQUESTHELPER_H
#define EASYSTUDIO_REQUESTHELPER_H


#include <string>

class RequestHelper {
public:
    static std::string Login(const std::string& mobile, const std::string& password, const std::string& nonce);
    static std::string RefreshToken(std::string &token);
    static std::string GetTemplates(std::string type, std::string count = "10", std::string name = "");
    static std::string GetSkins(const std::string& templateId, const std::string& templateType);
    static std::string GetAttachments(const std::string& itemId, const std::string& itemType);
    static std::string GetSkinPath(const std::string& itemId);
    static std::string DownloadZip(const std::string& uri, const std::string& savePath);
    static std::string GetResZipUri(std::string& attachments);
    static void UploadFile(const std::string &path, const std::string &itemId);
    static void        AddResConfig(const std::string& itemId, const std::string& attachmentId);
    static void        ModifyResConfig(const std::string& id, const std::string& itemId, const std::string& attachmentId);
    static std::string GetResConfigId(const std::string &itemId, const std::string &itemType);

private:
    static std::string Get(const std::string& uri);
};

#endif //EASYSTUDIO_REQUESTHELPER_H
