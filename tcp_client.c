#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <arpa/inet.h>


int main(int argc, const char* argv[])
{
    // 创建一个通信的套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        perror("socket error");
        exit(1);
    }

    // 连接服务器
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);

    int ret = connect(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(ret == -1)
    {
        perror("connect error");
        exit(1);
    }

    // 通信
    while(1)
    {
        // 从终端读一个字符串
        char buf[1024] = {0};
        fgets(buf, sizeof(buf), stdin);
        // 发数据
        write(fd, buf, strlen(buf)+1);

        // 接收
        // 阻塞等待
        int len = read(fd, buf, sizeof(buf));
        if(len == -1)
        {
            perror("read error");
            exit(1);
        }
        printf("read buf = %s\n", buf);
        
    }

    close(fd);

    return 0;
}
