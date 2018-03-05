#include "util.h"
#include "homer.h"

int main(int argc, char* argv[])
{
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);

    Homer* homer = homer_build();
    if (argc <= 1) {
        homer_parse(homer, stdin);
    } else {
        for (int j = 1; j < argc; ++j) {
            FILE* fp = fopen(argv[j], "r");
            if (!fp) {
                fprintf(stderr, "Could not open %s for reading\n", argv[j]);
                continue;
            }
            homer_parse(homer, fp);
            fclose(fp);
        }
    }
    homer_destroy(homer);
    return 0;
}
