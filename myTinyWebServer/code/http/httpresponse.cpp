#include "httpresponse.h"
using namespace std;

const unordered_map<string, string> HttpResponse::SUFFIX_TYPE = {
    { ".html",  "text/html" },
    { ".xml",   "text/xml" },
    { ".xhtml", "application/xhtml+xml" },
    { ".txt",   "text/plain" },
    { ".rtf",   "application/rtf" },
    { ".pdf",   "application/pdf" },
    { ".word",  "application/nsword" },
    { ".png",   "image/png" },
    { ".gif",   "image/gif" },
    { ".jpg",   "image/jpeg" },
    { ".jpeg",  "image/jpeg" },
    { ".au",    "audio/basic" },
    { ".mpeg",  "video/mpeg" },
    { ".mpg",   "video/mpeg" },
    { ".avi",   "video/x-msvideo" },
    { ".gz",    "application/x-gzip" },
    { ".tar",   "application/x-tar" },
    { ".css",   "text/css "},
    { ".js",    "text/javascript "},
};

const unordered_map<int, string> HttpResponse::CODE_STATUS = {
    { 200, "OK" },
    { 400, "Bad Request" },
    { 403, "Forbidden" },
    { 404, "Not Found" },
};

const unordered_map<int, string> HttpResponse::CODE_PATH = {
    { 400, "/400.html" },
    { 403, "/403.html" },
    { 404, "/404.html" },
};

HttpResponse::HttpResponse() {
    _code = -1;
    _path = _srcDir = "";
    _isKeepAlive = false;
    _mmFile = nullptr;
    _mmFileStat = {0};
}

void HttpResponse::Init(const string &srcDir, string &path, bool isKeepAlive, int code) {
    assert(srcDir != "");
    if (_mmFile) UnmapFile();       // 重置已映射的文件指针
    _code = code;
    _isKeepAlive = isKeepAlive;
    _path = path;
    _srcDir = srcDir;
    _mmFile = nullptr;
    _mmFileStat = {0};
}

char *HttpResponse::File() {
    return _mmFile;
}

size_t HttpResponse::FileLen() const {
    return _mmFileStat.st_size;
}

string HttpResponse::_GetFileType() {
    // 判断文件类型
    size_t idx = _path.find_last_of('.');
    if (idx == string::npos) return "text/plain";
    // .后边的即为后缀名
    string suffix = _path.substr(idx);
    if (SUFFIX_TYPE.count(suffix) == 1) {
        return SUFFIX_TYPE.at(suffix);
    }
    return "text/plain";
}

void HttpResponse::_ErrorHtml() {
    if (CODE_PATH.count(_code) == 1) {
        _path = CODE_PATH.at(_code);
        stat((_srcDir + _path).data(), &_mmFileStat);
    }
}

void HttpResponse::_AddStateLine(Buffer &buff) {
    string status;
    if (CODE_STATUS.count(_code) == 1) {
        status = CODE_STATUS.at(_code);
    } else {
        _code = 400;
        status = CODE_STATUS.at(400);
    }
    buff.Append("HTTP/1.1 " + to_string(_code) + " " + status + "\r\n");
}

void HttpResponse::_AddHeader(Buffer &buff) {
    buff.Append("Connection: ");
    if (_isKeepAlive) {
        buff.Append("keep-alive\r\n");
        buff.Append("keep-alive: max=6, timeout=120\r\n");
    } else {
        buff.Append("close\r\n");
    }
    buff.Append("Content-type: " + _GetFileType() + "\r\n");
}

void HttpResponse::ErrorContent(Buffer &buff, string message) {
    string body, status;
    // 直接构造error page的body
    body += "<html><title>Error</title>";
    body += "<body bgcolor=\"ffffff\">";
    if (CODE_STATUS.count(_code) == 1) {
        status = CODE_STATUS.at(_code);
    } else {
        status = "Bad Request";
    }
    body += to_string(_code) + " : " + status + "\n";
    body += "<p>" + message + "</p>";
    body += "<hr><em>TinyWebServer</em></body></html>";

    buff.Append("Content-length: " + to_string(body.size()) + "\r\n\r\n");
    buff.Append(body);
}

void HttpResponse::_AddContent(Buffer &buff) {
    int srcFd = open((_srcDir + _path).data(), O_RDONLY);
    if (srcFd < 0) {
        ErrorContent(buff, "File Not Found!");
        return;
    }

    // 
    LOG_DEBUG("file path %s", (_srcDir + _path).data());
    int *mmRet = (int *)mmap(0, _mmFileStat.st_size, PROT_READ, MAP_PRIVATE, srcFd, 0);
    if (*mmRet == -1) {
        ErrorContent(buff, "File Not Found!");
        return;
    }
    _mmFile = (char *)mmRet;
    close(srcFd);
    buff.Append("Content-length: " + to_string(_mmFileStat.st_size) + "\r\n\r\n");
}

void HttpResponse::UnmapFile() {
    if (_mmFile) {
        munmap(_mmFile, _mmFileStat.st_size);
        _mmFile = nullptr;
    }
}

void HttpResponse::MakeResponse(Buffer &buff) {
    // 判断请求资源是否存在，S_ISDIR表示是否是一个目录
    if (stat((_srcDir + _path).data(), &_mmFileStat) < 0 || S_ISDIR(_mmFileStat.st_mode)) {
        _code = 404;
    }
    else if (!(_mmFileStat.st_mode & S_IROTH)) {
        _code = 403;
    }
    else if (_code == -1) {
        _code = 200;
    }
    _ErrorHtml();
    _AddStateLine(buff);
    _AddHeader(buff);
    _AddContent(buff);
}

HttpResponse::~HttpResponse() {
    UnmapFile();
}