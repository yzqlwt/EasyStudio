//
// Created by yzqlwt on 2020/11/28.
//
#include "RequestHelper.h"
#include "httplib.h"
#include "Tools.h"
#include <curl/curl.h>
#include <fstream>
#include <boost/format.hpp>
#include <nlohmann/json.hpp>

static std::string BaseUrl = "https://gate2.betamath.com";
static std::string ResUrl = "https://gstatic-cn.oss-cn-hangzhou.aliyuncs.com";

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

std::string RequestHelper::Get(const std::string& uri){
    httplib::Client cli(BaseUrl.c_str());
    cli.set_default_headers({
                                    {"Accept",     "*/*"},
                                    {"User-Agent", "Chrome/86.0.4240.198 Safari/537.36"}
                            });
    cli.set_bearer_token_auth(Tools::GetToken().c_str());
    std::cout << uri << std::endl;
    auto res = cli.Get(uri.c_str());
    return res->body;
}



/**
 * @brief        accessToken失效后通过refreshToken来刷新
 * @param token  refreshToken
 * @return       包含accessToken，refreshToken, expires_in的json字符串
 *
 */
std::string RequestHelper::RefreshToken(std::string &token) {
    httplib::Client cli(BaseUrl.c_str());
    httplib::Params params{
            {"refresh_token", token},
    };
    cli.set_default_headers({
                                    {"Accept",     "*/*"},
                                    {"User-Agent", "Chrome/86.0.4240.198 Safari/537.36"}
                            });
    auto res = cli.Post("/a/user/refresh", params);
    std::cout << res->body << std::endl;
    return res->body;
}

/**
 * @brief           登录
 * @param mobile    手机号
 * @param password  密码
 * @param nonce     谷歌验证码
 * @return          json字符串
 */
std::string RequestHelper::Login(const std::string& mobile, const std::string& password, const std::string& nonce) {
    httplib::Client cli(BaseUrl.c_str());
    httplib::Params params{
            {"mobile",   mobile},
            {"password", password},
            {"nonce",    nonce}
    };
    cli.set_default_headers({
                                    {"Accept",     "*/*"},
                                    {"User-Agent", "Chrome/86.0.4240.198 Safari/537.36"}
                            });
    auto res = cli.Post("/a/user/login", params);
    return res->body;
}

/**
 * @brief         获取Templates(互动或者习题)
 * @param type    互动或者习题(activityTemplates or exerciseTemplates)
 * @param count   请求数量(默认为10个)
 * @param name    按名称请求(默认为"")
 * @return        返回json字符串
 */
std::string RequestHelper::GetTemplates(std::string type, std::string count, std::string name){
    boost::format fmt("/admin-course/%1%?count=%2%&name=%3%&start=0");
    fmt % type % count % name;
    return RequestHelper::Get(fmt.str());
}


/**
 * @brief              获取模板skins
 * @param templateId   模板id
 * @param templateType 模板类型(ActivityTemplate or ExerciseTemplate)
 * @return             返回json字符串
 */
std::string RequestHelper::GetSkins(const std::string &templateId, const std::string& templateType) {
    boost::format fmt("/admin-course/skin?templateId=%1%&templateType=%2%");
    fmt % templateId % templateType;
    return RequestHelper::Get(fmt.str());
}

/**
 * @brief             获取attachments
 * @param itemId      itemType是ActivityTemplate,为模板ID, itemType是Skin，为SkinId
 * @param itemType    ActivityTemplate模板所有skin共用, Skin为skin独享
 * @return            返回json字符串(list)
 */
std::string RequestHelper::GetAttachments(const std::string &itemId, const std::string &itemType) {
    boost::format fmt("/admin-course/item/attachment?itemType=%1%&itemId=%2%");
    fmt % itemType % itemId;
    return RequestHelper::Get(fmt.str());
}


/**
 * @brief             获取attachments
 * @param itemId      itemType是ActivityTemplate,为模板ID, itemType是Skin，为SkinId
 * @param itemType    ActivityTemplate模板所有skin共用, Skin为skin独享
 * @return            返回json字符串(list)
 */
std::string RequestHelper::GetResConfigId(const std::string &itemId, const std::string &itemType) {
    auto res = RequestHelper::GetAttachments(itemId, itemType);
    if (nlohmann::json::accept(res)){
        auto json = nlohmann::json::parse(res);
        auto arr = json.get<std::vector<std::map<std::string, nlohmann::json>>>();
        for (int i = 0; i < arr.size(); ++i) {
            auto name = arr[i]["name"].get<std::string>();
            if (name.compare("ResConfig")==0){
                return std::to_string(arr[i]["id"].get<int>());
            }
        }
    }
    return "";
}


/**
 * @brief          获取skin的解压路径（res/ui/games/game01/skin01）
 * @param itemId   skinId
 * @return         string(res/ui/games/game01/skin01)
 */
std::string RequestHelper::GetSkinPath(const std::string &itemId) {
    boost::format fmt(BaseUrl+"/admin-course/properties?itemType=Skin&itemId=%1%");
    fmt % itemId;
    auto res =  RequestHelper::Get(fmt.str());
    auto json = nlohmann::json::parse(res);
    for (auto elem : json){
        auto name = elem["name"].get<std::string>();
        if (name == "path"){
            return elem["content"].get<std::string>();
        }
    }
    return nullptr;
}

/**
 * @brief      上传文件
 * @param path 文件路径
 */

void RequestHelper::UploadFile(const std::string &path, const std::string &itemId) {
    httplib::Client cli(BaseUrl.c_str());
    cli.set_default_headers({
                                    {"Accept",     "*/*"},
                                    {"User-Agent", "Chrome/86.0.4240.198 Safari/537.36"}
                            });
    cli.set_bearer_token_auth(Tools::GetToken().c_str());
    std::ifstream ifs(path, std::ios::in | std::ios::binary);
    std::stringstream ss;
    ss << ifs.rdbuf();
    ifs.close();
    std::string content(ss.str());
    httplib::MultipartFormDataItems items = {
            { "file", content, "res2-0.zip", "application/zip" },
    };
    auto res = cli.Post("/admin-course/asset/uploadSingle", items);
    std::cout << "文件上传:\t" << res->body << std::endl;
    if (nlohmann::json::accept(res->body)){
        auto json = nlohmann::json::parse(res->body);
        auto id = std::to_string(json["id"].get<int>());
        auto attachment_id = RequestHelper::GetResConfigId(itemId, "Skin");
        if (attachment_id.compare("")==0){
            RequestHelper::AddResConfig(itemId, id);
        }else {
            RequestHelper::ModifyResConfig(attachment_id, itemId, id);
        }
    }
}

/**
 * @brief 创建ResConfig
 * @param itemId: skin_id
 * @param attachmentId: 上传文件返回的id
 */

void RequestHelper::AddResConfig(const std::string& itemId, const std::string& attachmentId) {
    httplib::Client cli(BaseUrl.c_str());
    char attachment[100];
    sprintf(attachment, "[%s]", attachmentId.c_str());
    httplib::Params params{
            {"itemId",   itemId},
            {"attachmentId", attachmentId},
            {"itemType",    "Skin"},
            {"name",    "ResConfig"},
    };
    cli.set_default_headers({
                                    {"Accept",     "*/*"},
                                    {"User-Agent", "Chrome/86.0.4240.198 Safari/537.36"}
                            });
    cli.set_bearer_token_auth(Tools::GetToken().c_str());
    auto res = cli.Post("/admin-course/item/attachment", params);
    std::cout << "新增ResConfig：" << res->body << std::endl;
}

void RequestHelper::ModifyResConfig(const std::string& id, const std::string& itemId, const std::string& attachmentId) {
    httplib::Client cli(BaseUrl.c_str());
    char attachment[100];
    sprintf(attachment, "[%s]", attachmentId.c_str());
    httplib::Params params{
            {"id",   id},
            {"itemId",   itemId},
            {"attachmentId", attachmentId},
            {"itemType",    "Skin"},
            {"name",    "ResConfig"},
    };
    cli.set_default_headers({
                                    {"Accept",     "*/*"},
                                    {"User-Agent", "Chrome/86.0.4240.198 Safari/537.36"}
                            });
    cli.set_bearer_token_auth(Tools::GetToken().c_str());
    boost::format fmt("/admin-course/item/attachment/%1%");
    fmt % id;
    auto res = cli.Put(fmt.str().c_str(), params);
    std::cout << "修改ResConfig：" << res->body << std::endl;
}


std::string RequestHelper::DownloadZip(const std::string &uri, const std::string& zipPath) {
    auto url = ResUrl + uri;
    auto savePath = zipPath + "/" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ".zip";
//    url = "https://mirrors.163.com/debian/dists/Debian10.7/Release";
    FILE *fp = fopen(savePath.c_str(), "wb");

    //curl初始化
    CURL *curl = curl_easy_init();
    // curl返回值
    CURLcode res;
    if (curl)
    {
        //设置curl的请求头
        struct curl_slist* header_list = NULL;
        header_list = curl_slist_append(header_list, "User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

        //不接收响应头数据0代表不接收 1代表接收
        curl_easy_setopt(curl, CURLOPT_HEADER, 0);

        //设置请求的URL地址
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        //设置ssl验证
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);

        //CURLOPT_VERBOSE的值为1时，会显示详细的调试信息
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);

        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);

        //设置数据接收函数
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

        //设置超时时间
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 6); // set transport and time out time
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 6);

        // 开启请求
        res = curl_easy_perform(curl);
    }
    // 释放curl
    curl_easy_cleanup(curl);
    //释放文件资源
    fclose(fp);
    return savePath;
}

std::string RequestHelper::GetResZipUri(std::string &attachments) {
    auto json = nlohmann::json::parse(attachments);
    if (json.is_array()){
        for (auto& element : json) {
            auto name = element["name"].get<std::string>();
            if (name.compare("ResConfig") == 0){
                return element["attachments"]["uri"].get<std::string>();
            }
        }
    }
    return "";
}




