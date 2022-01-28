#ifndef NOTE_CLIENT_CLION_HTTP_H
#define NOTE_CLIENT_CLION_HTTP_H

#include "httplib.h"
#include "inter_var.h"

enum HTTPType{
    HTTP_unknown = 0,
    HTTP_client,
    HTTP_server,
};

typedef int (*SERVER_CALLBACK)(const std::string& req, std::string& res, ErrorCode &error_code);

class HTTP {
public:
    HTTP(HTTPType http_type, const std::string &address, int server_port = 0, bool encode = false);

    // 客户端获取大量数据或者少量数据
    int c_get(const std::string &handle, const std::string &req, std::string &res, ErrorCode &error_code);
    // 客户端提交大量数据
    int c_post(const std::string &handle, const std::string &req, std::string &res, ErrorCode &error_code);

    // 服务端接口
    int s_start_listen();
    int s_register_callback(const std::string &handle, SERVER_CALLBACK handle_callback, ErrorCode &error_code);

private:
    void init_client();
    void init_server();
    bool check_http_status();

    // 服务端多线程管理
    static void s_handle_ctrl(const httplib::Request& req, httplib::Response& res);
    // 调用注册好的函数
    static void s_handle_process(const std::string &handle, const httplib::Request& req, httplib::Response& res);
private:
    HTTPType m_http_type;
    httplib::Client *m_client;
    httplib::Server *m_server;
    std::string m_address;
    int m_server_port;

    static bool s_encode;
    static std::map<std::string, SERVER_CALLBACK> m_server_handle_map;
};

#endif //NOTE_CLIENT_CLION_HTTP_H
