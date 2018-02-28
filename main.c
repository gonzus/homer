#include "util.h"
#include "homer.h"

int main(int argc, char* argv[])
{
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);

    Homer* homer = homer_build();
    homer_parse(homer);
    homer_destroy(homer);
    return 0;
}
