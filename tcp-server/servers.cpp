// TCP通信服务端

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <thread>

void test_threadListen(int cfd)
{
    while (1)
    {
        // 5.
        // 获取客户端的数据
        char recvbuf[512] = {0};
        int lens = read(cfd, recvbuf, sizeof(recvbuf));
        if (lens == -1)
        {
            perror("read");
            exit(-1);
        }
        else if (lens > 0)
        {
            printf("recv client data: %s\n", recvbuf);
        }
        else if (lens == 0)
        { // 表示客户端断开连接
            printf("client closed...");
            break;
        }
    }
}

int main()
{
    // 1.创建socket(用于监听的套接字)
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    if (lfd == -1)
    {
        perror("socket");
        exit(-1);
    }

    // 2.绑定
    struct sockaddr_in saddr;
    saddr.sin_family = PF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    saddr.sin_port = htons(8090);
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));

    if (ret == -1)
    {
        perror("bind");
        exit(-1);
    }

    // 3.监听
    listen(lfd, 5);
    if (ret == -1)
    {
        perror("listen");
        exit(-1);
    }

    // 4.接受客户端连接
    struct sockaddr_in caddr;
    socklen_t len = sizeof(caddr);
    int cfd = accept(lfd, (struct sockaddr *)&caddr, &len);

    if (cfd == -1)
    {
        perror("accept");
        exit(-1);
    }

    // 输出客户端的信息
    char cip[16];
    inet_ntop(AF_INET, &caddr.sin_addr.s_addr, cip, sizeof(cip));
    unsigned short cport = ntohs(caddr.sin_port);
    printf("client ip is %s,port is %d\n", cip, cport);

    std::thread th1(test_threadListen, cfd); // 启动线程
    th1.detach();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    while (1)
    {
        // 给客户端发送数据
        char sendBuf[512];
        memset(sendBuf, 0, sizeof(sendBuf));
        fgets(sendBuf, 512, stdin);
        send(cfd, sendBuf, strlen(sendBuf), 0);
    }

    // 关闭文件描述符
    close(lfd);
    close(cfd);

    return 0;
}
