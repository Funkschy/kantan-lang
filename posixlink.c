#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int link_obj_file(char *const obj_name, char *const exe_name) {
#if defined(__APPLE__) || defined(__MACH__)
    char *const args[] = {
        "/usr/bin/ld",
        "-o",
        exe_name,
        obj_name,
        "/usr/lib/libSystem.B.dylib",
        NULL
    };
#endif

#if defined (__linux__)
    char *const args[] = {
        "/usr/bin/ld",
        "-o",
        exe_name,
        "-dynamic-linker",
        "/lib64/ld-linux-x86-64.so.2",
        "/usr/lib/x86_64-linux-gnu/crt1.o",
        "/usr/lib/x86_64-linux-gnu/crti.o",
        "-lc",
        obj_name,
        "/usr/lib/x86_64-linux-gnu/crtn.o",
        NULL
    };
#endif

    pid_t pid;
    if ((pid = fork()) == 0) {
        if (execve(args[0], args, NULL) == -1) {
            perror("ERROR");
            return -1;
        }
    }

    int status;
    waitpid(pid, &status, 0);

    return WEXITSTATUS(status);
}
