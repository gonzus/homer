#include "util.h"
#include "homer.h"

int main(int argc, char* argv[])
{
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);

    Homer* homer = homer_build();
    if (argc <= 1) {
        homer_process(homer, "<STDIN>", stdin);
    } else {
        for (int j = 1; j < argc; ++j) {
            homer_process(homer, argv[j], 0);
        }
    }
    homer_destroy(homer);
    return 0;
}
