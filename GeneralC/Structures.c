#include <stdint.h>

typedef struct base {
    int x;
    int y;
} struct_t;


// Read Only
const struct base_const {
    int x;
    int y;
} my_const_struct = {10, 25};
// my_const_struct.x = 10    // NOK
// base_const cst = {10, 25} // OK


// Only visible in translation unit (file)
static struct base_static {
    int x;
    int y;
} my_static_struct = {10, 25};
// my_static_struct.x = 10   // OK
// my_static_struct cst = {10, 25} // NOK


// Read Only Only visible in translation unit (file)
static const struct base_static
{
    int x;
    int y;
} my_static_const_struct = {10, 25};



/// Example of usint static const struct :

void input_cb(int argc, char* argv)
{

}

static const struct cmd
{
    const char* cmd;
    void (*cb)(int argc, char* argv);
} cmds[] = {
    {"input", input_cb},
    {"input_2", input_cb},
    { 0 }
};


int main(int argc, char* argv[])
{
    const struct cmd* c;

        for (c = cmds; c->cmd; ++c)
    {
        if (matches(argv[0], c->cmd) == 0)
            return c->cb(argc - 1, argv + 1);
    }

    return 0;
}

// ----------------------------------------------------------------------------

typedef struct base __attribute__((__packed__))
{
    struct __attribute__((__packed__))
    {
        uint64_t data1;
        uint32_t data2;
        uint64_t data3;
    } header;
    uint64_t un1;
    uint8_t un2;
    char un3;
} base_t;