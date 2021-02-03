//
// Created by yzqlwt on 2020/11/5.
//

#ifndef EASYSTUDIO_TOOLS_H
#define EASYSTUDIO_TOOLS_H
#include <string>
#include <vector>


class Tools {
public:
    static std::string GetMd5(const std::string& file);

    static void replace_str(std::string& str, const std::string& before, const std::string& after);
    static std::string ReplaceStr(std::string& str, const std::string& before, const std::string& after);
    static void SaveToken(std::string& token);
    static std::string ReadFile(std::string path);
    static void WriteFile(const std::string& path, const std::string& content);
    static std::vector<std::string> GetRegexStr(const std::string& content, const std::string& regex);
    static std::vector<std::string> TexturePackage(const std::string& res_path, const std::string& output_path);
    static std::string GetPlistName(const std::vector<std::string>& files, const std::string& md5);
    static std::string RefreshToken();
    static std::string GetToken();
    static void CopyJSFL();
    static void AddWatcher();
};


#endif //EASYSTUDIO_TOOLS_H
