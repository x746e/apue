#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>


bool is_terminal(FILE *stream) {
    return isatty(fileno(stream));
}


void mysetbuf(FILE *stream, char *buf) {
    if (buf == NULL) {
        // If buf == NULL, disable buffering
        setvbuf(stream, buf, _IONBF, BUFSIZ);
    } else if (is_terminal(stream)) {
        // If stream is wrapping a termnial, set line buffering
        setvbuf(stream, buf, _IOLBF, BUFSIZ);
    } else {
        // Else set full buffering
        setvbuf(stream, buf, _IOFBF, BUFSIZ);
    }
}

void main() {}


