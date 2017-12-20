#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include "server.h"
#include "rio.h"


int writeTo(int fd, char * string)
{
    return write(fd, string, strlen(string));
}

int err_dump(int fd, int status, char * err_msg)
{
    char line[LINE_SIZE];


}

void sig_int(int signo)
{
    exit(0);
}

void sig_child(int signo)
{
    signal(SIGCHLD, sig_child);
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
}

void initServer()
{
    /* 忽略 sigpipe 信号 */
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, sig_int);
    signal(SIGCHLD, sig_child);
}

/* 打开监听 */
int open_listenfd(int port)
{
    int sockfd, res;
    struct sockaddr_in addr;

    /* 创建socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "socket error\n");
        exit(1);
    }
    printf("创建socket成功\n");

    /* 初始化地址 */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    printf("初始化地址成功\n");

    /* 绑定地址 */
    res = bind(sockfd, (const struct sockaddr *)&addr, sizeof(addr));
    if (res < 0) {
        fprintf(stderr, "bind error\n");
        exit(1);
    }
    printf("绑定地址 %s:%d 成功\n", inet_ntoa(addr.sin_addr), PORT);

    /* 监听 */
    res = listen(sockfd, 50);
    if (res < 0) {
        fprintf(stderr, "listen error\n");
        exit(1);
    }
    printf("监听成功\n");
   
    return sockfd;
}


void doit(int fd)
{
    char line[LINE_SIZE];
    char method[10], uri[100], version[10];
    rio_t rio;

    rio_init(&rio, fd);
    
    if (rio_readline(&rio, line, LINE_SIZE) <= 0)
    {
        writeTo(fd, "HTTP/1.1 400 Bad Request\r\n");
        writeTo(fd, "\r\n");
        return;
    }

    if (sscanf(line, "%s %s %s", method, uri, version) != 3) {
        writeTo(fd, "HTTP/1.1 400 Bad Request\r\n");
        writeTo(fd, "\r\n");
        return;
    }

    

    while(rio_readline(&rio, line, LINE_SIZE) > 0) {
        if (strcmp(line, "\r\n") == 0) {
           break;
        }
    }

    if (strcmp(method, "GET") != 0) {
        
    }

    line = getFileContent(uri);

    writeTo(fd, "HTTP/1.1 200 OK\r\n");
    writeTo(fd, "\r\n");
    writeTo(fd, line);

}

int main()
{
    int fd, sockfd, pid, num;
    socklen_t client_len;
    struct sockaddr_in client_addr;
    char * client_ip;

    initServer();

    sockfd = open_listenfd(PORT);

    num = 0;
    /* 等待请求 */
    while (1) {
        while ((fd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len)) < 0) {
            if (errno != EINTR) {
                /* 不是被信号处理函数中断 */
                fprintf(stderr, "accept error\n");
                exit(1);
            }
        }
        ++num;
        client_ip = inet_ntoa(client_addr.sin_addr);
        printf("请求 %d: %s\n", num, client_ip);
        
        if ((pid = fork()) < 0) {
            fprintf(stderr, "fork error\n");
            exit(1);
        } else if (pid == 0) {
            /* child */
            close(sockfd);
            doit(fd);
            printf("结束 %d: %s\n", num, client_ip);
            exit(0);
        }

        close(fd);
    }

    return 0;
}
