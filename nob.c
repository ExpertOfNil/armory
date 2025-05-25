#define NOB_IMPLEMENTATION
#include "nob.h"

#define COMMON_CFLAGS \
    "-std=c99", "-Wall", "-Wextra", "-pedantic", "-ggdb"
#define BUILD_DIR "build/"
#define SRC_DIR "src/"

int main(int argc, char** argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    Nob_Cmd cmd = {0};
    if (!nob_mkdir_if_not_exists(BUILD_DIR)) return 1;

    nob_cmd_append(&cmd, "clang", COMMON_CFLAGS);
    nob_cmd_append(&cmd, "-Iinclude");
    nob_cmd_append(&cmd, SRC_DIR "tests.c");
    nob_cmd_append(&cmd, "-o", BUILD_DIR "tests");
    nob_cmd_append(&cmd, "-lm");
    if (!nob_cmd_run_sync(cmd)) return 1;
    return 0;
}
