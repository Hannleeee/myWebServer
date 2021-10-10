#ifndef HTTP_RESPONSE_H
#define HHTP_RESPONSE_H

#include <unordered_map>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "../buffer/buffer.h"
#include "../log/log.h"

class HttpResponse {
public:
    HttpResponse();
    ~HttpResponse();

    void Init(const std::string &srcDir, std::string &path, bool isKeepAlive = false, int code = -1);
    void MakeResponse(Buffer &buff);
    void UnmapFile();
    char *File();
    size_t FileLen() const;
    void ErrorContent(Buffer &buff, std::string message);
    int Code() const { return _code; }
private:
    void _AddStateLine(Buffer &buff);
    void _AddHeader(Buffer &buff);
    void _AddContent(Buffer &buff);

    void _ErrorHtml();
    std::string _GetFileType();

    int _code;
    bool _isKeepAlive;

    std::string _path;
    std::string _srcDir;

    char *_mmFile;
    struct stat _mmFileStat;        // 文件元数据

    static const std::unordered_map<std::string, std::string> SUFFIX_TYPE;
    static const std::unordered_map<int, std::string> CODE_STATUS;
    static const std::unordered_map<int, std::string> CODE_PATH;
};

#endif