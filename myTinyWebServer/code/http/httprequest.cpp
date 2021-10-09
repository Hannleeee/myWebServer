#include "httprequest.h"
using namespace std;

const unordered_set<string> HttpRequest::_DEFAULT_HTML {
    "/index", "/register", "/login", "/welcome", "/video", "/picture",};

const unordered_map<string, int> HttpRequest::_DEFAULT_HTML_TAG {
    {"/register.html", 0}, {"/login.html", 1}, };

std::string HttpRequest::GetPath() const{
    return _path;
}

std::string& HttpRequest::GetPath(){
    return _path;
}
std::string HttpRequest::GetMethod() const {
    return _method;
}

std::string HttpRequest::GetVersion() const {
    return _version;
}

std::string HttpRequest::GetPost(const std::string& key) const {
    assert(key != "");
    if(_post.count(key) == 1) {
        return _post.find(key)->second;
    }
    return "";
}

std::string HttpRequest::GetPost(const char* key) const {
    assert(key != nullptr);
    if(_post.count(key) == 1) {
        return _post.find(key)->second;
    }
    return "";
}

void HttpRequest::Init() {
    _method = _path = _version = _body = "";
    _state = REQUEST_LINE;
    _header.clear();
    _post.clear();
}

bool HttpRequest::IsKeepAlive() const {
    if (_header.count("Connection") == 1) {
        return _header.find("Connection")->second == "keep-alive" && _version == "1.1";
    }
    return false;
}

void HttpRequest::_ParsePath() {

}

bool HttpRequest::_ParseRequestLine(const string &line) {

}

void HttpRequest::_ParseHeader(const string &line) {

}

int HttpRequest::_ConvertHex(char ch) {

}

void HttpRequest::_ParseFromUrlencoded() {

}

bool HttpRequest::UserVerify(const string &name, const string &pwd, bool isLogin) {
    
}

void HttpRequest::_ParsePost() {

}

void HttpRequest::_ParseBody(const string &line) {

}

