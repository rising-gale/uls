#include "libmx.h"

int mx_read_line(char **lineptr, size_t buf_size, char delim, const int fd) {
    if (fd < 0)
        return -2;

    (*lineptr) = (char *) mx_realloc(*lineptr, buf_size);
    mx_memset((*lineptr), '\0', _msize((*lineptr)));
    size_t bytes = 0;
    char buf;

    if (read(fd, &buf, 1)) {
        if (buf == delim)
            return bytes;

        (*lineptr) = (char *) mx_realloc(*lineptr, bytes + 1);
        (*lineptr)[bytes] = buf;
        bytes++;
    }
    else
        return -1;

    while (read(fd, &buf, 1)) {
        if (buf == delim)
            break;
        
        if (bytes >= buf_size)
            (*lineptr) = (char *) mx_realloc(*lineptr, bytes + 1);

        (*lineptr)[bytes] = buf;

        bytes++;
    }

    (*lineptr) = (char *) mx_realloc(*lineptr, bytes + 1);

    size_t free_bytes = _msize((*lineptr)) - bytes;
    mx_memset(&(*lineptr)[bytes], '\0', free_bytes);

    return bytes + 1;
}
