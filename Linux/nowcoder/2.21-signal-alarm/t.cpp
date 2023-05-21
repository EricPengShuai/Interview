#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

static jmp_buf jmpbuffer;

int main()
{
    // 捕捉信号
    signal(SIGALRM, [](int){
        longjmp(jmpbuffer, 1);
    });

    alarm(1);
    volatile size_t i = 0;

    // longjmp 跳转到这里
    if (setjmp(jmpbuffer) != 0)
    {
        printf("sum = %lu\n", i);
        _exit(0);
    }

    while (true)
        ++i;
}
