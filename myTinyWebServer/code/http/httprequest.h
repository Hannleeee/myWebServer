#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <regex>
#include <errno.h>
#include <mysql/mysql.h>

#include "../buffer/buffer.h"
#include "../log/log.h"
#include "../pool/sqlconnpool.h"
#include "../pool/sqlconnRAII.h"

class HttpRequest {
public:
    enum PARSE_STATE {
        REQUEST_LINE,
        HEADERS,
        BODY,
        FINISH,
    };

    enum HTTP_CODE {
        NO_REQUEST = 0,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURCE,
        FORBIDDENT_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONNECTION,
    };

    HttpRequest() { Init(); }
    ~HttpRequest() = default;

    void Init();
    bool Parse(Buffer &buff);

    std::string GetPath() const;
    std::string &GetPath();
    std::string GetMethod() const;
    std::string GetVersion() const;
    std::string GetPost(const std::string &key) const;
    std::string GetPost(const char *key) const;

    bool IsKeepAlive() const;

private:
    bool _ParseRequestLine(const std::string &line);
    void _ParseHeader(const std::string &line);
    void _ParseBody(const std::string &line);

    void _ParsePath();
    void _ParsePost();
    void _ParseFromUrlencoded();

    static bool UserVerify(const std::string &name, const std::string &pwd, bool isLogin);

    PARSE_STATE _state;
    std::string _method, _path, _version, _body;
    std::unordered_map<std::string, std::string> _header;
    std::unordered_map<std::string, std::string> _post;

    static const std::unordered_set<std::string> _DEFAULT_HTML;     // 页面路径集合
    static const std::unordered_map<std::string, int> _DEFAULT_HTML_TAG;    // 注册和登录页面地址集合
    static int _ConvertHex(char ch);
};

#endif