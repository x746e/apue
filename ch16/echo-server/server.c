#define _BSD_SOURCE
#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "apue.h"
#include "common.h"


#define BUFSZ 256


int main() {
    int sock, client_sock, n;
    char in_buf[BUFSZ];
    char out_buf[BUFSZ];
    struct sockaddr_in addr = { 0 };
    struct sockaddr client_addr;
    socklen_t client_addr_len;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);

    sys_chk(sock = socket(AF_INET, SOCK_STREAM, 0));
    sys_chk(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)));
    sys_chk(bind(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)));
    sys_chk(listen(sock, 5));

    sys_chk(client_sock = accept(sock, &client_addr, &client_addr_len));
    sys_chk(n = read(client_sock, in_buf, BUFSZ - 1));
    in_buf[n] = '\0';
    sprintf(out_buf, "Got: %s", in_buf);
    sys_chk(write(client_sock, out_buf, strlen(out_buf)));

    return EXIT_SUCCESS;
}
