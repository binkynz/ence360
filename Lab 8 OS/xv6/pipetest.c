#include "types.h"
#include "stat.h"
#include "user.h"

#define NULL 0

int main(void)
{
    char buf[512];
    int n;

    int pr[2], pw[2];
    pipe(pr);
    pipe(pw);

    int stdout = dup(0);

    if (!fork())
    {
        close(0); // stdin
        dup(pr[0]);
        close(1); // stdout
        dup(pw[1]);

        close(pr[0]);
        close(pr[1]);
        close(pw[0]);
        close(pw[1]);

        char *argv[] = {"wc", NULL};
        exec("wc", argv);
    }
    else
    {
        close(pr[0]);
        write(pr[1], "hello world\n", 12);
        close(pr[1]);

        wait();

        printf(stdout, "recieved:\n");

        close(pw[1]);

        while ((n = read(pw[0], buf, sizeof(buf))) > 0)
            printf(stdout, buf);

        close(pw[0]);
    }

    exit();
}
