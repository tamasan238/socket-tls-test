#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // read(), write(), close()

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>

int main(void){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address_s;

    address_s.sin_family = AF_INET;
    address_s.sin_addr.s_addr = inet_addr("127.0.0.1");
    address_s.sin_port = 50000;

    int res = connect(sockfd, (struct sockaddr *)&address_s, sizeof(address_s));
    if(res == -1){
        perror("接続エラー\n");
        exit(1);
    }

    printf("\n * server IP: %s, port: %d\n", inet_ntoa(address_s.sin_addr), ntohs(address_s.sin_port));

    char buf[80]="\0";

    strcpy(buf, "Client will connect to server.");
    write(sockfd, buf, strlen(buf));
    memset(buf, '\0', sizeof(buf));
    read(sockfd, buf, sizeof(buf));
    printf("\n * message from server: %s\n", buf);

    close(sockfd);

    return 0;
}