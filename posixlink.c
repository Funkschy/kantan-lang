#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <unistd.h>
#include <sys/wait.h>

#if defined (__linux__)

#define BUF_LEN sizeof("usr/lib/x86_64-linux-gnu/") + 6 + 1
#define OBJ_NAME_LEN 6

struct paths {
    char crt1[BUF_LEN];
    char crti[BUF_LEN];
    char crtn[BUF_LEN];
};

bool find_single(
        char buf[BUF_LEN],
        char crt_out[BUF_LEN],
        size_t dir_len,
        char const obj_name[OBJ_NAME_LEN]
        ) {
    memcpy(buf + dir_len, obj_name, OBJ_NAME_LEN);
    buf[dir_len + OBJ_NAME_LEN] = 0;

    if (access(buf, F_OK) != -1) {
        memcpy(crt_out, buf, BUF_LEN);
        return true;
    }

    return false;
}

bool find_linux_crt(struct paths *paths) {
    char const *const possible_dirs[] = {
        "/usr/lib/",
        "/usr/lib/x86_64-linux-gnu/",
    };
    size_t num_possible = sizeof(possible_dirs) / sizeof(const char *const);

    // has to be the size of the longest possible dir + strlen("crt_.o") + 1;
    char buf[BUF_LEN];

    bool crt1_found = false;
    bool crtn_found = false;
    bool crti_found = false;

    for (size_t i = 0; i < num_possible && (!crt1_found || !crti_found || !crtn_found); i++) {
        char const *const dir = possible_dirs[i];
        size_t j = 0;
        for (; j < BUF_LEN; j++) {
            if (dir[j] == 0) {
                break;
            }

            buf[j] = dir[j];
        }

        if (!crt1_found) {
            crt1_found = find_single(buf, paths->crt1, j, "crt1.o");
        }

        if (!crti_found) {
            crti_found = find_single(buf, paths->crti, j, "crti.o");
        }

        if (!crtn_found) {
            crtn_found = find_single(buf, paths->crtn, j, "crtn.o");
        }
    }

    return false;
}
#endif

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
    struct paths paths = {};
    find_linux_crt(&paths);

    // see https://dev.gentoo.org/~vapier/crt.txt
    char *const args[] = {
        "/usr/bin/ld",
        "-o",
        exe_name,
        "-dynamic-linker",
        "/lib64/ld-linux-x86-64.so.2",
        paths.crt1, // _start function (only exe)
        paths.crti, // prologs for .init & .fini sections
        "-lc",
        obj_name,
        paths.crtn, // epilogs for .init & .fini sections
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
