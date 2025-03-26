#include "Interpreter.h"
#include <print>

int main(int argc, const char **argv) 
{
    // Ensure enough arguments were provided
    if (argc != 3)
    {
        std::println("usage: {} [rom] [cycles per second/max/min]", argv[0]);
        return 1;
    }

    // Parse the cycles per second argument
    int cyclesPerSec{};
    if (strncmp(argv[2], "max", 3) == 0)
        cyclesPerSec = Interpreter::MaxCycles;
    else if (strncmp(argv[2], "min", 3) == 0)
        cyclesPerSec = Interpreter::MinCycles;
    else
        cyclesPerSec = std::stoi(argv[2]);

    // Run the interpreter
    Interpreter interpreter(cyclesPerSec, argv[1]);
    interpreter.Run();
}
