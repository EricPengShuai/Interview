#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    assert(argc == 2);
    char *host = argv[1];
    // 获取目标主机地址信息
    struct hostent *hostinfo = gethostbyname(host);
    assert(hostinfo);
    // 获取 daytime 服务信息
    struct servent *servinfo = getservbyname("daytime", "tcp");
    assert(servinfo);
    printf("daytime port is %d\n", ntohs(servinfo->s_port));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = servinfo->s_port;
    // 注意这里由于 h_addr_list 已经是网络字节序了
    address.sin_addr = *(struct in_addr *)*hostinfo->h_addr_list;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int result = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
    assert(result != -1);

    char buffer[128];
    result = read(sockfd, buffer, sizeof(buffer));
    assert(result > 0);
    buffer[result] = '\0';
    printf("the day tiem is: %s", buffer);
    close(sockfd);
    return 0;
}