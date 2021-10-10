#include "httprequest.h"
using namespace std;

const unordered_set<string> HttpRequest::_DEFAULT_HTML {
    "/index", "/register", "/login", "/welcome", "/video", "/picture",};

const unordered_map<string, int> HttpRequest::_DEFAULT_HTML_TAG {
    {"/register.html", 0}, {"/login.html", 1}, };

string HttpRequest::GetPath() const{
    return _path;
}

string& HttpRequest::GetPath(){
    return _path;
}
string HttpRequest::GetMethod() const {
    return _method;
}

string HttpRequest::GetVersion() const {
    return _version;
}

string HttpRequest::GetPost(const string& key) const {
    assert(key != "");
    if(_post.count(key) == 1) {
        return _post.find(key)->second;
    }
    return "";
}

string HttpRequest::GetPost(const char* key) const {
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

string HttpRequest::GetPost(const string &key) const {
    assert(key != "");
    if (_post.count(key) == 1) {
        // []运算符不支持const修饰的变量，可以使用at()代替
        return _post.at(key);
    }
    return "";
}

string HttpRequest::GetPost(const char *key) const {
    assert(key != nullptr);
    if (_post.count(key) == 1) {
        return _post.at(key);
    }
}

bool HttpRequest::_ParseRequestLine(const string &line) {
    regex pattern("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");    // 匹配请求行
    smatch subMatch;
    if (regex_match(line, subMatch, pattern)) {     // 请求行匹配成功
        // subMatch[0]存储的是line本身
        _method = subMatch[1];
        _path = subMatch[2];
        _version = subMatch[3];
        _state = HEADERS;       // 状态转换
        return true;
    }
    LOG_ERROR("RequestLine Error");
    return false;
}

void HttpRequest::_ParsePath() {
    if (_path == "/") {
        _path = "/index.html";
    } else {
        for (auto &item : _DEFAULT_HTML) {
            // 若地址存在
            if (item == _path) {
                _path += ".html";
                break;
            }
        }
    }
}

void HttpRequest::_ParseHeader(const string &line) {
    regex pattern("^([^:]*): ?(.*)$");
    smatch subMatch;
    if (regex_match(line, subMatch, pattern)) {
        _header[subMatch[1]] = subMatch[2];
    } else {
        // HEADER全部解析完成，状态转换至BODY
        _state = BODY;
    }
}

// 解析URL中特殊字符的转义编码
int HttpRequest::_ConvertHex(char ch) {
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    return ch;
}

void HttpRequest::_ParseFromUrlencoded() {
    if (_body.size() == 0) return;

    string key, value;
    int num = 0;
    int n = _body.size();
    int i = 0, j = 0;

    for (; i < n; ++i) {
        char ch = _body[i];
        switch (ch) {
            case '=':   // 以=分隔键值对中的键和值
                key = _body.substr(j, i - j);   // =前边部分即为键
                j = i + 1;
                break;
            case '&':   // 以&分隔键值对
                value = _body.substr(j, i-j);
                j = i + 1;
                _post[key] = value;
                LOG_DEBUG("%s = %s", key.c_str(), value.c_str());
                break;
            case '+':   // 即空格
                _body[i] = ' ';
                break;
            case '%':   // 遇到了转义字符，注意转义字符格式都是“%xx”格式，xx为两位16进制数
                num = _ConvertHex(_body[i+1]) * 16 + _ConvertHex(_body[i+2]);
                _body[i + 2] = num % 10 + '0';
                _body[i + 1] = num / 10 + '0';  // 转换为ASCII码对应值，但仍保留了%
                i += 2;
                break;
            default:
                break;
        }
    }
    assert(i <= i);
    if (_post.count(key) == 0 && j < i) {
        value = _body.substr(j, i - j);
        _post[key] = value;
    }
}

bool HttpRequest::UserVerify(const string &name, const string &pwd, bool isLogin) {
    // 没有携带用户信息
    if (name == "" || pwd == "") return false;
    LOG_INFO("Verify name: %s pwd %s", name.c_str(), pwd.c_str());
    MYSQL *sql;
    SqlConnRAII(&sql, SqlConnPool::Instatnce());
    assert(sql);

    bool flag = false;              // 假设用户名可用
    // unsigned int fieldNum = 0;      // 结果集中列数
    char order[256] = {0};
    MYSQL_FIELD *fields = nullptr;  // 字段信息指针，即字段名、类型和大小
    MYSQL_RES *res = nullptr;       // mysql查询结果集

    if (!isLogin) flag = true;      // 不是登录，则为注册，并假设校验成功
    // 构建查询用户密码的sql指令
    snprintf(order, 256, "SELECT username, password FROM user WHERE username='%s' LIMIT 1",
                name.c_str());
    LOG_DEBUG("%s", order);
    // 使用获取的sql连接查询构建的指令
    if (mysql_query(sql, order)) {  // 成功返回零，失败则返回错位类型的非零值，进入下述语句
        mysql_free_result(res);     // 操作结束，释放结果集
        return false;
    }
    // 查询成功，将sql连接查询结果存入res，也即将所有username=name的记录都找出来
    res = mysql_store_result(sql);
    // fieldNum = mysql_num_fields(res);       // 结果集中列的数目
    // fields = mysql_fetch_fields(res);       // 获取全部字段信息

    // ?为什么用while？在注册逻辑层面应该已经要求了用户名不能重复啊？
    // while (MYSQL_ROW row = mysql_fetch_row(res)) {
    if (MYSQL_ROW row = mysql_fetch_row(res)) {
        // 遍历username==name的记录
        LOG_DEBUG("MYSQL ROW: %s %s", row[0], row[1]);
        string password(row[1]);    // 当前记录的密码
        // 是登录行为，则对比密码是否一致
        if (isLogin) {
            if (pwd == password) flag = true;
            else {
                flag = false;
                LOG_DEBUG("pwd error!");
            }
        } else {        // 是注册行为，且用户名已被占用！
            flag = false;       // 将用户名标记为不可用
            LOG_DEBUG("user used!");
        }
    }
    mysql_free_result(res);

    // 若是注册行为，且用户名可用
    if (!isLogin && flag == true) {
        LOG_DEBUG("register!");
        bzero(order, 256);      // 重置原命令
        snprintf(order, 256, "INSERT INTO user(username, password) VALUES('%s', '%s')", name.c_str(), pwd.c_str());
        LOG_DEBUG("%s", order);
        if (mysql_query(sql, order)) {
            LOG_DEBUG("Insert error!");
            flag = false;
        }
        flag = true;
    }
    LOG_DEBUG("UserVerify success!");
    return flag;

}

void HttpRequest::_ParsePost() {
    // application/x-www-form-urlencoded即最常用的POST方法
    if (_method == "POST" && _header["Content-Type"] == "application/x-www-form-urlencoded") {
        _ParseFromUrlencoded();
        // 若请求路径为注册或登录
        if (_DEFAULT_HTML_TAG.count(_path)) {
            int tag = _DEFAULT_HTML_TAG.at(_path);
            LOG_DEBUG("Tag:%d", tag);
            // 0为注册，1为登陆
            if (tag == 0 || tag == 1) {
                bool isLogin =  (tag == 1);
                if (UserVerify(_post["username"], _post["password"], isLogin)) {
                    // 不管是登录还是注册，成功便转到欢迎页
                    _path = "/welcome.html";
                } else {    // 登录或注册失败
                    _path = "/error.html";
                }
            }
        }
    }
}

void HttpRequest::_ParseBody(const string &line) {
    _body = line;
    _ParsePost();
    _state = FINISH;
    LOG_DEBUG("Body:%s, len:%d", line.c_str(), line.size());
}

bool HttpRequest::Parse(Buffer &buff) {
    const char CRLF[] = "\r\n";     // HTTP报文的换行标志
    if (buff.ReadableBytes() <= 0) return false;

    while (buff.ReadableBytes() && _state != FINISH) {
        const char *lineEnd = search(buff.Peek(), buff.BeginWriteConst(), CRLF, CRLF + 2);
        string line(buff.Peek(), lineEnd);
        switch (_state) {
            case REQUEST_LINE:
                if (!_ParseRequestLine(line)) return false;
                _ParsePath();
                break;
            case HEADERS:
                _ParseHeader(line);
                if (buff.ReadableBytes() <= 2) _state = FINISH;
                break;
            case BODY:
                _ParseBody(line);
                break;
            default:
                break;
        }
        if (lineEnd == buff.BeginWrite()) break;
        buff.RetrieveUntil(lineEnd + 2);
    }
    LOG_DEBUG("[%s], [%s], [%s]", _method.c_str(), _path.c_str(), _version.c_str());
    return true;
}