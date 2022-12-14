#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // read(), write(), close()

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <openssl/ssl.h>

#define SERVER_CERT_FILE    "../cert/server-cert.pem"
#define SERVER_KEY_FILE     "../cert/server-key.pem"

int main(void){
    SSL_CTX* ctx = NULL;
    SSL*     ssl = NULL;

    SSL_library_init();

    ctx = SSL_CTX_new(SSLv23_client_method());
    SSL_CTX_load_verify_locations(ctx, CA_CERT_FILE, NULL);

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

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);
    SSL_connect(ssl);

    char buf[80]="\0";

    strcpy(buf, "Client will connect to server.");
    // write(sockfd, buf, strlen(buf));
    SSL_write(ssl, buf, strlen(buf));
    memset(buf, '\0', sizeof(buf));
    // read(sockfd, buf, sizeof(buf));
    SSL_read(ssl, buf, sizeof(buf));
    printf("\n * message from server: %s\n", buf);

    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);

    close(sockfd);

    return 0;
}