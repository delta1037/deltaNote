#include<thread>
#include "http.h"
#include "base64.h"
#include "log.h"

bool HTTP::s_encode = false;
std::map<std::string, SERVER_CALLBACK> HTTP::m_server_handle_map;

HTTP::HTTP(HTTPType http_type, const std::string &address, int server_port, bool encode) {
    m_http_type = http_type;
    m_address = address;
    m_server_port = server_port;
    s_encode = encode;

    m_client = nullptr;
    m_server = nullptr;

    if(m_address.empty()){
        d_net_error("%s", "http address is empty !!!")
        return;
    }
    if(m_http_type == HTTP_client){
        init_client();
    }else if(m_http_type == HTTP_server){
        init_server();
    }
}

void HTTP::init_client(){
    m_client = new httplib::Client(m_address);
    m_client->set_connection_timeout(0, 300000);
}

void HTTP::init_server(){
    if(m_server_port == 0){
        d_net_error("%s", "server init need port")
        return ;
    }
    m_server = new httplib::Server();
}

int HTTP::c_get(const std::string &handle, const std::string &req, std::string &res, ErrorCode &error_code) {
    if(!check_http_status()){
        d_net_error("%s", "http check fail")
        return RET_FAILED;
    }

    // 计算路径
    //std::string t_path = handle;
    // 投递参数 TODO Post之后服务端是怎么处理参数的
    httplib::Headers headers;
    if(s_encode){
        headers.emplace("head", base64_encode(req, true));
    }else{
        headers.emplace("head", req);
    }
    //headers.emplace("path", t_path);
    d_net_debug("client start to get %s, addr:%s", handle.c_str(), m_address.c_str())
    auto http_res = m_client->Get(handle.c_str(), headers); // , headers
    if(http_res == nullptr){
        d_net_error("client get %s return NULL", handle.c_str())
        return RET_FAILED;
    }
    d_net_debug("client get return %s, status :%d", handle.c_str(), http_res->status)
    if(http_res->status == 200){
        d_net_debug("handle %s get result success", handle.c_str())
        if(s_encode){
            res = base64_decode(http_res->body);
        }else{
            res = http_res->body;
        }
        return RET_SUCCESS;
    }
    // 处理遇到的错误
    d_net_error("handle %s get result fail, status is %d", handle.c_str(), http_res->status)
    return RET_FAILED;
}


int HTTP::c_post(const std::string &handle, const std::string &req, std::string &res, ErrorCode &error_code) {
    if(!check_http_status()){
        d_net_error("%s", "http check fail")
        return RET_FAILED;
    }

    // 计算路径
    //std::string t_path = "/" + handle;
    // 投递参数 TODO Post之后服务端是怎么处理参数的
    httplib::Params params;
    if(s_encode){
        params.emplace("body", base64_encode(req, true));
    }else{
        params.emplace("body", req);
    }

    auto http_res = m_client->Post(handle.c_str(), params);
    if(http_res->status == 200){
        d_net_debug("handle %s get result success", handle.c_str())
        if(s_encode){
            res = base64_decode(http_res->body);
        }else{
            res = http_res->body;
        }
        return RET_SUCCESS;
    }
    // 处理遇到的错误
    d_net_error("handle %s get result fail, status is %d", handle.c_str(), http_res->status)
    return RET_FAILED;
}

int HTTP::s_register_callback(const std::string &handle, SERVER_CALLBACK handle_callback, ErrorCode &error_code) {
    if(!check_http_status()){
        d_net_error("%s", "http check fail")
        return RET_FAILED;
    }
    // 注册到统一接口，之后在统一接口里进行多线程分流
    m_server->Get(handle, s_handle_ctrl);
    m_server_handle_map[handle] = handle_callback;
    d_net_debug("server register handle %s", handle.c_str())
    return RET_SUCCESS;
}

void HTTP::s_handle_ctrl(const httplib::Request &req, httplib::Response &res) {
    d_net_debug("get request, request headers size:%d", req.headers.size())
    d_net_debug("get request, request params size:%d", req.params.size())
    d_net_debug("get request, request version:%s", req.version.c_str())
    d_net_debug("get request, request target:%s", req.target.c_str())
    d_net_debug("get request, request method:%s", req.method.c_str())
    d_net_debug("get request, request path:%s", req.path.c_str())
    d_net_debug("get request, request body:%s", req.body.c_str())

    // 对处理程序进行分流
    std::string t_handle = req.path;
    //t_handle = t_handle.erase(0);
    // 检测对应的处理有没有注册
    if(m_server_handle_map.find(t_handle) == m_server_handle_map.end()){
        d_net_error("handle %s process not register", t_handle.c_str())
        res.status = 404;
        return;
    }

    SERVER_CALLBACK t_handle_func = m_server_handle_map[t_handle];
    ErrorCode error_code;
    // 分清楚是Post还是Get类型
    std::string s_req;
    if(!req.headers.empty()){
        // GET类型，数据放head了
        auto it = req.headers.find("head");
        if(it != req.headers.end()){
            s_req = it->second;
        }
    }
    if(!req.params.empty() && s_req.empty()){
        // POST类型放param了
        auto it = req.params.find("body");
        if(it != req.params.end()){
            s_req = it->second;
        }
    }
    if(s_req.empty()){
        d_net_error("path %s get no param", req.path.c_str())
    }
    if(s_encode){
        s_req = base64_decode(s_req);
    }

    std::string s_res;
    int ret = t_handle_func(s_req, s_res, error_code);
    if(ret == RET_FAILED){
        d_net_error("handle %s process error", t_handle.c_str())
    }
    if(s_encode){
        s_res = base64_encode(s_res);
    }
    res.body = s_res;
    res.status = 200;
    d_net_debug("handle %s process success", t_handle.c_str())
}

int HTTP::s_start_listen() {
    if(!check_http_status()){
        d_net_error("%s", "http check fail")
        return RET_FAILED;
    }
    if(!m_server->listen(m_address.c_str(), m_server_port)){
        d_net_error("server listen %s:%d error", m_address.c_str(), m_server_port)
    }
    d_net_debug("server listen %s:%d success", m_address.c_str(), m_server_port)
    return RET_SUCCESS;
}

bool HTTP::check_http_status() {
    if(m_http_type == HTTP_unknown){
        return false;
    }
    if(m_http_type == HTTP_client && m_client == nullptr){
        return false;
    }
    if(m_http_type == HTTP_server && (m_server == nullptr || m_server_port == 0)){
        return false;
    }
    return true;
}
