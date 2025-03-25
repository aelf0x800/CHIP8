#include "Interpreter.h"
#include <print>

int main(int argc, const char **argv) 
{
    if (argc != 2)
    {
        std::println("usage: {} [ROM]", argv[0]);
        return 1;
    }

    Interpreter i(argv[1]);
    i.Run();
}
