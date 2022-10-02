#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char buf[512];

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf(1, "usage: cp src_file dest_file\n");
        exit();
    }

    int src_fd = open(argv[1], O_RDONLY);
    int dest_fd = open(argv[2], O_CREATE | O_RDWR);

    int n;
    while ((n = read(src_fd, buf, sizeof(buf))) > 0)
    {
        if (write(dest_fd, buf, n) != n)
        {
            printf(1, "cp: write error\n");
            exit();
        }
    }

    if (n < 0)
    {
        printf(1, "cp: read error\n");
        exit();
    }

    close(src_fd);
    close(dest_fd);

    exit();
}
