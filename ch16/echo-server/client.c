#define _BSD_SOURCE
#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include "apue.h"
#include "common.h"


#define BUFSZ 256


int main() {
    int s;
    char buf[BUFSZ];
    struct sockaddr_in addr;

    sys_chk(s = socket(AF_INET, SOCK_STREAM, 0));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(22);
    assert(inet_aton("127.0.0.1", &(addr.sin_addr)) == 1);
    sys_chk(connect(s, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)));
    read(s, buf, BUFSZ);
    printf("%s\n", buf);
}
