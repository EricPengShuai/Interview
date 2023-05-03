#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <assert.h>
#include <sys/fcntl.h>

bool daemonize()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        return false;
    }
    else if (pid > 0)
    {
        exit(0);
    }

    umask(0);

    pid_t sid = setsid();
    if (sid < 0)
    {
        return false;
    }

    if ((chdir("/")) < 0)
    {
        /* Log the failure */
        return false;
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);
    return true;
}