#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// server
int main(int argc, const char* argv[])
{
    // 创建监听的套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1)
    {
        perror("socket error");
        exit(1);
    }

    // 绑定
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9999);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // 本地多有的ＩＰ
    // 127.0.0.1
    // inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);
    
    // 设置端口复用 
    // 需要在ｂｉｎｄ函数之前设置
    int opt = 1;
//    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(opt));

    // 绑定端口
    int ret = bind(lfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(ret == -1)
    {
        perror("bind error");
        exit(1);
    }

    // 监听
    ret = listen(lfd, 64);
    if(ret == -1)
    {
        perror("listen error");
        exit(1);
    }

    // 阻塞等待连接请求，　并接受连接请求
    struct sockaddr_in clien_addr;
    socklen_t clien_len = sizeof(clien_addr);
    int cfd = accept(lfd, (struct sockaddr*)&clien_addr, &clien_len);
    if(cfd == -1)
    {
        perror("accetp error");
        exit(1);
    }

    char ipbuf[128];
    printf("client iP: %s, port: %d\n", inet_ntop(AF_INET, &clien_addr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)),
           ntohs(clien_addr.sin_port));

    char buf[1024] = {0};
    while(1)
    {
        // read data
        // int len = read(cfd, buf, sizeof(buf));
        int len = recv(cfd, buf, sizeof(buf), 0);
        if(len == -1)
        {
            perror("recv error");
            exit(1);
        }
        else if(len == 0)
        {
            printf("客户端已经断开连接。。。\n");
            break;
        }
        printf("read buf = %s\n", buf);
        // 小写转大写
        for(int i=0; i<len; ++i)
        {
            buf[i] = toupper(buf[i]);
        }
        printf("after buf = %s\n", buf);

        // 大写串发给客户端
        // write(cfd, buf, strlen(buf)+1);
        ret = send(cfd, buf, strlen(buf)+1, 0);
        if(ret == -1)
        {
            perror("send error");
            exit(1);
        }
    }

    close(cfd);
    close(lfd);

    return 0;

}
