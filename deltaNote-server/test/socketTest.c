#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    //创建套接字
    int sock=socket(AF_INET,SOCK_STREAM,0);

    //向特定的服务器发起请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr,0,sizeof(serv_addr));//用0填充每个字节
    serv_addr.sin_family=AF_INET; //使用ipv4
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");        //你将要连接的ip地址
    serv_addr.sin_port=htons(8888); //端口
    connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    //关闭套接字
    //close(sock);

    return 0;
}