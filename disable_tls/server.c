#include <stdio.h>

// #include <stdlib.h>
#include <string.h> // strlen()
// #include <fcntl.h>  // open(), creat()
#include <unistd.h> // read(), write(), close()

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void){
    int sockfd_s = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address_s;

    address_s.sin_family = AF_INET;
    address_s.sin_addr.s_addr = inet_addr("127.0.0.1");
    address_s.sin_port = 50000;
    bind(sockfd_s, (struct sockaddr *)&address_s, sizeof(address_s));
    listen(sockfd_s, 5);
    printf("server waits\n");

    struct sockaddr_in address_c;

    int length_c = sizeof(address_c);

    int sockfd_c = accept(sockfd_s, (struct sockaddr *)&address_c, &length_c);
    printf("\n * request from client IP: %s, port: %d\n", inet_ntoa(address_c.sin_addr), ntohs(address_c.sin_port));

    char buf[80] = "\0";

    memset(buf, '\0', sizeof(buf));
    read(sockfd_c, buf, sizeof(buf));
    printf("\n * message from client: %s \n", buf);
    strcpy(buf, "From Server via socket");
    write(sockfd_c, buf, strlen(buf));

    close(sockfd_s);
    close(sockfd_c);

    return 0;
}